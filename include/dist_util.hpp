#pragma once

#include "common.hpp"


// Calculating sum(log(pdf(v))) for a vector v
template <class disttype>
double calcLogPdfVec(const disttype &dist,
               const VectorXd &x)
{

    double ll = 0;
    for (auto xi : x)
    {
        ll += log(pdf(dist, xi));
    }
    return ll;
}

// Templated generator
template <class T>
double callGen(T &generator)
{
    return generator();
}

// Fill vector res with generator calls
// // Example:
//  VectorXd res(10);
//  fillVecGen(unif_rng, res);
//  std::cout << res << std::endl;
template <class T>
void fillVecGen(T &generator,
                  VectorXd &res)
{
    for (size_t i = 0; i < res.size(); ++i)
        res[i] = generator();
}
