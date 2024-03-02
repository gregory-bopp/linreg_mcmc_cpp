#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


MatrixXd readMatrix(const std::string filename);

void writeMatrix(const string filename, const MatrixXd  &X);