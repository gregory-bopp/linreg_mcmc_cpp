#include <iostream>
#include <time.h>
#include <Eigen/Dense>
#include <boost/math/distributions/normal.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/program_options.hpp>
#include "eigen_io.hpp"
#include "dist_util.hpp"
#include "common.hpp"

namespace mcmc {

class MCMC
    {
    public:
                        MCMC();
                        ~MCMC();
        void            procCommandLineArgs(int argc, const char * argv[]);

        void            run();


    private:

        template<class T>
        void            updateOneBeta(T &beta_prop_rng,
                                        unif_gen_type &runif,                                    
                                        const dnorm_dist_type &eps_dist,
                                        const int &beta_idx);
        template <class T>
        void            updateSigmaSq(T &sigma_sq_gibbs_rng);

        unsigned            _random_seed;
        int                 _n_mcmc;
        int                 _thin_int;
        std::string         _x_filename;
        std::string         _y_filename;
        std::string         _post_beta_filename;
        std::string         _post_sigma_sq_filename;

        // Data
        MatrixXd _X;
        MatrixXd _y; 
        int _n_beta;
        int _n_obs;

        // Model Parameters
        VectorXd _beta_cur;   
        double _sigma_sq_cur;   

        // Model Hyper Parameters
        double _sigma_sq_beta;
        double _a_sigma_sq;
        double _b_sigma_sq;

        // Prior distributions
        dnorm_dist_type     _beta_prior_dist;



    };

inline MCMC::MCMC()
    {
    std::cout << "Starting MCMC" <<std::endl;
    }

inline MCMC::~MCMC()
    {
    std::cout << "Stopping MCMC" << std::endl;
    }

void MCMC::procCommandLineArgs(int argc, const char * argv[])
    {

    boost::program_options::variables_map      vm;
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "print help message")
        // ("version,v", "print program version")
        ("seed,z",     boost::program_options::value(&_random_seed)->default_value(1),                                 "seed for pseudorandom number generator")
        ("n_mcmc,n",   boost::program_options::value(&_n_mcmc)->default_value(10000),                                  "number of mcmc iterations to run")
        ("thin_int,t", boost::program_options::value(&_thin_int)->default_value(100),                                  "mcmc thinning interval") 
        ("x_filename,x",    boost::program_options::value(&_x_filename)->default_value("data/X.csv"),                  "name of X data file in csv format")
        ("y_filename,y",    boost::program_options::value(&_y_filename)->default_value("data/y.csv"),                  "name of y data file in csv format")        
        ("post_beta_filename,p", boost::program_options::value(&_post_beta_filename)->default_value("data/posterior_beta.csv"),    "name of posterior beta samples data files in csv format")
        ("post_sigma_sq_filename,p", boost::program_options::value(&_post_sigma_sq_filename)->default_value("data/posterior_sigma_sq.csv"),    "name of posterior sigma_sq samples data files in csv format")
        ("sigma_sq_beta",   boost::program_options::value(&_sigma_sq_beta)->default_value(10.0),                       "beta normal prior variance")
        ("a_sigma_sq",   boost::program_options::value(&_a_sigma_sq)->default_value(0.1),                              "sigma_sq inverse gamma prior shape")
        ("b_sigma_sq",   boost::program_options::value(&_b_sigma_sq)->default_value(1.0),                              "sigma_sq inverse gamma prior scale")        
        ;
    // Store command line options in vm       
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    try
        {
        // Read config file
        const boost::program_options::parsed_options & parsed = boost::program_options::parse_config_file< char >("mcmc.conf", desc, false);
        // Overwrite config file options with command line options
        boost::program_options::store(parsed, vm);
        }
    catch(boost::program_options::reading_file & x)
        {
        std::cout << "Warning: configuration file (mcmc.conf) not found" << std::endl;
        }
    boost::program_options::notify(vm);

    // Display help
    if (vm.count("help") > 0)
        {
        std::cout << desc << "\n";
        std::exit(1);
        }

    };


// Update one beta (Gibbs updates are available for beta
// but here we will use a Metropolis update)
template <class T>
void MCMC::updateOneBeta(T &beta_prop_rng,
                        unif_gen_type &runif,                     
                        const dnorm_dist_type &eps_dist,
                        const int &beta_idx)
{
    VectorXd beta_prop = _beta_cur;

    // Current log-likelihood
    VectorXd eps_cur = _y - _X * _beta_cur;
    double log_full_cond_cur = calcLogPdfVec(eps_dist, eps_cur) +  
                    calcLogPdfVec(_beta_prior_dist, _beta_cur);

    // Update rng dist
    rnorm_dist_type prop_norm_dist(_beta_cur[beta_idx], 1);
    beta_prop_rng.distribution() = prop_norm_dist;

    // Propose beta update (this is a bad, independent, update)
    beta_prop[beta_idx] = callGen(beta_prop_rng);
    // Proposal log-likelihood
    VectorXd eps_prop = _y - _X * beta_prop;
    double log_full_cond_prop = calcLogPdfVec(eps_dist, eps_prop) +  
                    calcLogPdfVec(_beta_prior_dist, _beta_cur);

    double l_mratio = log_full_cond_prop - log_full_cond_cur;

    if (log(callGen(runif)) < l_mratio)
    {
        _beta_cur = beta_prop;
    }
};

//
// Gibbs update for sigma sq
template <class T>
void MCMC::updateSigmaSq(T &sigma_sq_gibbs_rng){

    VectorXd eps_cur = _y - _X * _beta_cur;
    double a_n = (_n_obs/2.0) + _a_sigma_sq;
    double b_n = ((eps_cur.dot(eps_cur))/(2.0)) + _b_sigma_sq;

    rgamma_dist_type gibbs_gamma_dist(a_n, 1.0/b_n);
    sigma_sq_gibbs_rng.distribution() = gibbs_gamma_dist;
    _sigma_sq_cur = 1.0/callGen(sigma_sq_gibbs_rng);
};

void MCMC::run()
{
    using namespace Eigen;
    
    // MCMC Settings
    int n_post = ceil(_n_mcmc/_thin_int);

    // Data
    _X = readMatrix(_x_filename);
    _y = readMatrix(_y_filename);
    
    _n_beta = _X.cols();
    _n_obs = _X.rows();
        
    // Initial Values 
    _beta_cur = VectorXd::Zero(_n_beta);
    _sigma_sq_cur = 1.0;
    

    ///////////////////////////////////////////////////////////////
    // Distributions and generators
    // Beta prior distribution
    _beta_prior_dist = dnorm_dist_type(0.0, std::sqrt(_sigma_sq_beta));
    // Set epsilon distribution 
    dnorm_dist_type eps_dist(0.0, std::sqrt(_sigma_sq_cur));
    // Uniform RNG
    unif_gen_type unif_rng(boost::mt19937(time(0)), boost::uniform_01<>());
    // Random normal generator
    gen_alg_type base_gen(_random_seed);
    rnorm_dist_type std_norm(0,1);                    // Standard normal
    rgamma_dist_type gamma_1_1(1, 1);
    norm_var_gen_type beta_prop_rng(base_gen, std_norm);  // Normal RNG
    gamma_var_gen_type sigma_sq_gibbs_rng(base_gen, gamma_1_1);


    // MCMC Samples
    MatrixXd beta_samples(n_post, _n_beta);
    VectorXd sigma_sq_samples(n_post);

    // Main MCMC loop
    int sample_idx = 0;
    for (int mcmc_idx = 0; mcmc_idx < _n_mcmc; mcmc_idx++)
    {

        // Update betas individually
        for (int beta_idx = 0; beta_idx < _n_beta; beta_idx++)
        {
            updateOneBeta(beta_prop_rng,
                          unif_rng,
                          eps_dist,
                          beta_idx);            
        }

        
        // Update inverse gamma sigma_sq
        updateSigmaSq(sigma_sq_gibbs_rng);

        // Update likelihood
        dnorm_dist_type eps_dist(0.0, std::sqrt(_sigma_sq_cur));

        // Save samples
        if (mcmc_idx % _thin_int == 0)
            {
                beta_samples.row(sample_idx) = _beta_cur;
                sigma_sq_samples(sample_idx) = _sigma_sq_cur;
                sample_idx++;
            }
    }    
    

    // Write Posterior Samples to File
    // beta
    writeMatrix(_post_beta_filename, beta_samples);
    // sigma_sq
    writeMatrix(_post_sigma_sq_filename, sigma_sq_samples);


};

}