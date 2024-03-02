#pragma once

#include <iostream>
#include <time.h>
#include <Eigen/Dense>
#include <boost/math/distributions.hpp>
#include <boost/random.hpp>

// Densities 
typedef boost::math::normal_distribution<> dnorm_dist_type;

//
// Variable Generation
//
// Random Number generator algorithm
typedef boost::mt19937 gen_alg_type;
// Distributions
typedef boost::random::normal_distribution<> rnorm_dist_type;
typedef boost::random::gamma_distribution<> rgamma_dist_type;
// RNG for each distribution type
typedef boost::variate_generator<gen_alg_type &, rnorm_dist_type> norm_var_gen_type;
typedef boost::variate_generator<gen_alg_type &, rgamma_dist_type> gamma_var_gen_type;
typedef boost::variate_generator<boost::mt19937, boost::random::uniform_01<>> unif_gen_type;