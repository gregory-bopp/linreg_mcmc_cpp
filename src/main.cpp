#include <iostream>
#include "mcmc.hpp"

using namespace mcmc;

int main(int argc, const char * argv[])
{
    try {        
        MCMC mcmc;
        // Process Command Line Arguments 
        mcmc.procCommandLineArgs(argc, argv);
        // Run MCMC
        mcmc.run();
    }
    catch(std::exception & x) {
        std::cerr << "Exception Message: " << x.what() << std::endl;
        std::cerr << "Quitting." << std::endl;
    }
    catch(...) {
        std::cerr << "Unknown Exception.\n";
    }

    return 0;
}