//
// Created by rob-ot on 15.2.19.
//

#ifndef REALCOLLABORATIONCAL_EIGENFILEEXTENSION_H
#define REALCOLLABORATIONCAL_EIGENFILEEXTENSION_H

#include <fstream>
/**
 * Handles the reading and writing of eigen matrices representing the transformation matrices for each camera to file
 * The matrices are written in binary format and saved to the location as discribed in
 * */
namespace EigenFile{
    template<class Matrix>
    void write_binary(const char* filename, const Matrix& matrix){
        std::ofstream out(filename, std::ios::out | std::ios::binary);
        typename Matrix::Index rows=matrix.rows(), cols=matrix.cols();
        out.write((char*) (&rows), sizeof(typename Matrix::Index));
        out.write((char*) (&cols), sizeof(typename Matrix::Index));
        out.write((char*) matrix.data(), rows*cols*sizeof(typename Matrix::Scalar) );
        out.close();
    }
    template<class Matrix>
    void read_binary(const char* filename, Matrix& matrix){
        std::ifstream in(filename, std::ios::in | std::ios::binary);
        typename Matrix::Index rows=0, cols=0;
        in.read((char*) (&rows),sizeof(typename Matrix::Index));
        in.read((char*) (&cols),sizeof(typename Matrix::Index));
        matrix.resize(rows, cols);
        in.read( (char *) matrix.data() , rows*cols*sizeof(typename Matrix::Scalar) );
        in.close();
    }
} // Eigen::
#endif //REALCOLLABORATIONCAL_EIGENFILEEXTENSION_H
