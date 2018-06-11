#pragma once

#include <blaze/math/CompressedMatrix.h>


/**
 * A sparse matrix of 0s and 1s.
 */
class Sparse_Binary_Matrix : blaze::CompressedMatrix<bool, blaze::rowMajor>
{
public:

    typedef blaze::CompressedMatrix<bool, blaze::rowMajor> base_t;

    //////////////////
    // Constructors

    // Default Constructor
    Sparse_Binary_Matrix()
    {}

    Sparse_Binary_Matrix(size_t m, size_t n)
        : base_t(n, m)
    {}

    // 
    Sparse_Binary_Matrix(std::initializer_list<std::initializer_list<typename base_t::ElementType>> m)
        : base_t(m)
    {}
};

