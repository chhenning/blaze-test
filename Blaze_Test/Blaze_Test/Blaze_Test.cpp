// Blaze_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include "SDR.hpp"

using namespace blaze;

void first_example()
{
    //// Instantiation of a static 3D column vector. The vector is directly initialized as
    ////    ( 4 -2  5 )
    //StaticVector<int, 3UL> a{ 4, -2, 5 };

    //// Instantiation of a dynamic 3D column vector. Via the subscript operator the values are set to
    ////    ( 2  5 -3 )
    //DynamicVector<int> b(3UL);
    //b[0] = 2;
    //b[1] = 5;
    //b[2] = -3;

    //// Adding the vectors a and b
    //DynamicVector<int> c = a + b;

    //// Printing the result of the vector addition
    //std::cout << "c =\n" << c << "\n";
}

void matrix_example()
{
    //// Instantiating a dynamic 3D column vector
    //DynamicVector<int> x{ 4, -1, 3 };
    //x[0] = 4;
    //x[1] = -1;
    //x[2] = 3;

    //// Instantiating a dynamic 2x3 row-major matrix, preinitialized with 0. Via the function
    //// call operator three values of the matrix are explicitly set to get the matrix
    ////   ( 1  0  4 )
    ////   ( 0 -2  0 )
    //DynamicMatrix<int> A(2UL, 3UL, 0);
    //A(0, 0) = 1;
    //A(0, 2) = 4;
    //A(1, 1) = -2;

    //// Performing a dense matrix/dense vector multiplication
    //DynamicVector<int> y = A * x;

    //// Printing the resulting vector
    //std::cout << "y =\n" << y << "\n";

    //// Instantiating a static column-major matrix. The matrix is directly initialized as
    ////   (  3 -1 )
    ////   (  0  2 )
    ////   ( -1  0 )
    //StaticMatrix<int, 3UL, 2UL, columnMajor> B{ { 3, -1 },{ 0, 2 },{ -1, 0 } };

    //// Performing a dense matrix/dense matrix multiplication
    //DynamicMatrix<int> C = A * B;

    //// Printing the resulting matrix
    //std::cout << "C =\n" << C << "\n";
}

void matrix_view_example()
{
    //// Creating a 'const'-qualified row-major dense 3x3 matrix
    //const blaze::DynamicMatrix<int> A{ { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };

    //// Extracting the 1st row into a row vector
    //blaze::DynamicVector<int, rowVector> x;
    //x = row(A, 1UL);
    //std::cerr << " x = " << x << "\n";
}

void sparse_vector_example()
{
    //int Size = 128;
    //int Non_Zero_Size = static_cast<int>(Size * 0.02);

    //CompressedVector<int> a(Size, Non_Zero_Size);
    //a[0] = 1;

    //CompressedVector<int> b {0, 0, 0, 1, 0};

    //// since there is only one non-zero element in b the following loop is only run once
    //std::generate(b.begin(), b.end(), []()
    //{ 
    //    return 5; 
    //});

    //std::cout << "b =\n" << b << "\n";

    //CompressedVector<int> c(128);
    //c[99] = 1;
}

void SDR_example()
{
    // https://bitbucket.org/blaze-lib/blaze/wiki/Matrix%20Operations

    typedef bool element_t;
    typedef blaze::CompressedMatrix<element_t, blaze::rowMajor> sparse_matrix_t;
    typedef blaze::DynamicMatrix<int, blaze::rowMajor> dense_matrix_t;

    sparse_matrix_t a({ {0,1,1},{1,1,1},{0,0,0} });
    
    assert(a.nonZeros(1) == 3);
    assert(a.nonZeros(2) == 0);

    auto b = a.transpose();
    assert(b.nonZeros(0) == 1);

    //////////////////////////
    // from dense to sparse binary

    dense_matrix_t d1 { {1,2,3}, {0,0,0}, {4, 0, 6} };
    sparse_matrix_t s1(d1);

    std::cout << "s1 =\n" << s1 << "\n";

    blaze::DynamicMatrix<float, blaze::rowMajor> d2{ {0.f, 1.f, 2.f},{0.f, 0.f, 5.f},{8.f, 7.f, 0.f} };
    sparse_matrix_t s2(d2);
    assert(s2.nonZeros(2) == 2);
    
    std::cout << "s2 =\n" << s2 << "\n";

    //////////////////////////
    // from sparse binary to dense
    sparse_matrix_t s3{ {0,1,1}, {0,0,1} };
    blaze::DynamicMatrix<float, blaze::rowMajor> d3(s3);

    std::cout << "d3 =\n" << d3 << "\n";
}

void create_random_sparse_matrix()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::uint32_t size = 2048;
    auto max_num_values_per_row = static_cast<std::uint32_t>(size * 0.02);

    std::srand(0);

    typedef blaze::CompressedMatrix<bool, blaze::rowMajor> sparse_binary_matrix_t;
    sparse_binary_matrix_t a(size, size);

    for (std::uint32_t r = 0; r < size; ++r)
    {
        // reserve enough space for n values in the current row
        a.reserve(r, max_num_values_per_row);

        // add 2% values in each row at random columns
        for (std::uint32_t v = 0; v < max_num_values_per_row; ++v)
        {
            auto random = std::rand() % size;
            a.append(r, random, 1);
        }

        a.finalize(r);
    }

    auto finish = std::chrono::high_resolution_clock::now();

    auto microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << microseconds.count() << "ms\n";
}

void create_random_sparse_matrix_2()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::uint32_t size = 2048;
    auto max_num_values = static_cast<std::uint32_t>((size * size) * 0.02);

    std::srand(0);

    typedef blaze::CompressedMatrix<bool, blaze::rowMajor> sparse_binary_matrix_t;
    sparse_binary_matrix_t a(size, size);

    a.reserve(max_num_values);

    for (std::uint32_t v = 0; v < max_num_values; ++v)
    {
        auto r = std::rand() % size;
        auto c = std::rand() % size;

        a.append(r, c, 1);
    }

    auto finish = std::chrono::high_resolution_clock::now();

    auto microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << microseconds.count() << "ms\n";
}



int main()
{
    //first_example();

    //matrix_example();

    //sparse_vector_example();
    
    //SDR_example();

    //create_random_sparse_matrix();

    create_random_sparse_matrix_2();

    return 0;
}

