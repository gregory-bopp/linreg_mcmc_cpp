# A C++ MCMC Sampler using Eigen and Boost
The aim of this project was to build an MCMC sampler for a simple problem that uses the Eigen and Boost c++ libraries. 

# Installation Requirements
Start by cloning this git repository.

## Dependencies and Prerequisites
### Eigen
- Place or symlink the Eigen c++ header-only library in the `include/` directory after unzipping: 
[https://eigen.tuxfamily.org](https://eigen.tuxfamily.org)

### Boost
1. Download boost c++ library: [https://www.boost.org/](https://www.boost.org/).
2. This project primarily makes use of the header-only libraries in boost with one exception: parsing program options.
To compile the `program_options` boost shared library, 
   
   1. Unzip the boost tarball into the `include/` project directory
   ```
   $ tar -xzvf boost_1_78_0.tar.gz
   ```
   1. Compile the boost shared library `program_options`. This can be compiled into a directory that you specify with the `--prefix` option.   Navigate to the unzipped boost directory and run the following, specifying the path to your preferred installation location in `--prefix`:
   ```
    $ ./bootstrap.sh --prefix=<path/to/lib> --with-libraries=program_options
   ```
   Finally, create the shared boost library with
   ```
   $ ./b2
   ```
   - The included Makefile with this project assumes this library has been placed in `${HOME}/lib/boost_1_78_0/stage/lib`, but the `BOOST` make variable should be changed in accordance with where this library has been created (see `--prefix` option above).

    Note: to view other available boost libraries, you can use
    ```
    ./bootstrap.sh --show-libraries 
    ```

### R
R is used in this project for simulating example data and plotting posteior samples, but is otherwise not needed. 

See R installation instructions here: [https://cran.r-project.org/](https://cran.r-project.org/).

Once R is installed, two R libraries can be installed from the R console with
```
> install.packages(`tidyverse` and `here`)
```

# MCMC Sampler Program Compilation
- Note: this makefile specifies `g++` as the compiler and also has variables for the Eigen and Boost library locations (`EIGEN` and `BOOST` respectively). Adjust these accordingly.
- Compile the MCMC sampler using the makefile by running `make` from the top-level of the project directory.

# Example
1. Simulate example data from the linear model by running the R script `r/simulate_data.R`
2. Run the MCMC sampler with the compiled binary `./bin/mcmc`
3. Plot the posterior samples with the R script `r/summarize_posterior_samples.R`

- Note the `mcmc` program takes several optional arguments (run `./bin/mcmc --help` for details)
- Those program options can also be specified in the configuration file `mcmc.conf`
```
Allowed options:
  -h [ --help ]                         print help message
  -z [ --seed ] arg (=1)                seed for pseudorandom number generator
  -n [ --n_mcmc ] arg (=10000)          number of mcmc iterations to run
  -t [ --thin_int ] arg (=100)          mcmc thinning interval
  -x [ --x_filename ] arg (=data/X.csv) name of X data file in csv format
  -y [ --y_filename ] arg (=data/y.csv) name of y data file in csv format
  -p [ --post_beta_filename ] arg (=data/posterior_beta.csv)
                                        name of posterior beta samples data 
                                        files in csv format
  -p [ --post_sigma_sq_filename ] arg (=data/posterior_sigma_sq.csv)
                                        name of posterior sigma_sq samples data
                                        files in csv format
  --sigma_sq_beta arg (=10)             beta normal prior variance
  --a_sigma_sq arg (=0.10000000000000001)
                                        sigma_sq inverse gamma prior shape
  --b_sigma_sq arg (=1)                 sigma_sq inverse gamma prior scale
```