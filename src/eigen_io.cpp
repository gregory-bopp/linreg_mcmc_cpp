#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


MatrixXd readMatrix(const std::string filename)
{
     
    ifstream file_obj(filename);         // File containing matrix
    string row_string;                   // Single row as string    
    std::vector<double> data_vec;        // Fill vector and use to convert to Matrix
    string x;                            // Single matrix element             
    int row_idx = 0;                     // Row index

    while (getline(file_obj, row_string)) 
    {
        stringstream row_stringstream(row_string);  
        while (getline(row_stringstream, x, ','))   // separate on comma
        {
            data_vec.push_back(stod(x));            // string to double
        }
        row_idx++;                            
    }
    return Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(data_vec.data(), row_idx, data_vec.size() / row_idx);
 
}



void writeMatrix(const string filename, const MatrixXd  &X)
{
    const static IOFormat csv_format(FullPrecision, DontAlignCols, ",", "\n");
    ofstream file(filename);
    if (file.is_open())
    {
        file << X.format(csv_format);
        file.close();
    }
}