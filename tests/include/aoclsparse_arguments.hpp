/* ************************************************************************
 * Copyright (c) 2020-2025 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ************************************************************************ */

/*! \file
 *  \brief aoclsparse_arguments.hpp provides a class to parse command arguments in clients
 */

#pragma once
#ifndef AOCLSPARSE_ARGUMENTS_HPP
#define AOCLSPARSE_ARGUMENTS_HPP

#include "aoclsparse.h"
#include "aoclsparse_datatype2string.hpp"

#include <cmath>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

struct Arguments
{
    aoclsparse_int M;
    aoclsparse_int N;
    aoclsparse_int K;
    aoclsparse_int nnz;
    aoclsparse_int nnzB;
    aoclsparse_int blk;
    aoclsparse_int block_dim;

    double alpha;
    double beta;

    aoclsparse_operation   transA;
    aoclsparse_operation   transB;
    aoclsparse_matrix_type mattypeA;
    aoclsparse_matrix_type mattypeB;
    aoclsparse_index_base  baseA;
    aoclsparse_index_base  baseB;
    aoclsparse_diag_type   diag;
    aoclsparse_fill_mode   uplo;
    aoclsparse_order       order;
    aoclsparse_int         stage;

    aoclsparse_matrix_init matrix;
    aoclsparse_matrix_init matrixB;

    aoclsparse_int          unit_check;
    aoclsparse_int          timing;
    aoclsparse_int          iters;
    aoclsparse_matrix_sort  sort;
    aoclsparse_memory_usage mem;

    std::string filename;
    std::string filenameB;
    char        function[64];

    aoclsparse_int x_stride, y_stride;
    char           output;

    std::vector<aoclsparse_int> kid_list;

private:
    // Function to read Structures data from stream
    friend std::istream &operator>>(std::istream &str, Arguments &arg)
    {
        str.read(reinterpret_cast<char *>(&arg), sizeof(arg));
        return str;
    }

    // print_value is for formatting different data types

    // Default output
    template <typename T>
    static void print_value(std::ostream &str, const T &x)
    {
        str << x;
    }

    // Floating-point output
    static void print_value(std::ostream &str, double x)
    {
        if(std::isnan(x))
            str << ".nan";
        else if(std::isinf(x))
            str << (x < 0 ? "-.inf" : ".inf");
        else
        {
            char s[32];
            snprintf(s, sizeof(s) - 2, "%.17g", x);

            // If no decimal point or exponent, append .0
            char *end = s + strcspn(s, ".eE");
            if(!*end)
                strcat(end, ".0");
            str << s;
        }
    }

    // Character output
    static void print_value(std::ostream &str, char c)
    {
        char s[]{c, 0};
        str << std::quoted(s, '\'');
    }

    // bool output
    static void print_value(std::ostream &str, bool b)
    {
        str << (b ? "true" : "false");
    }

    // string output
    static void print_value(std::ostream &str, const char *s)
    {
        str << std::quoted(s);
    }
};

/* Common data used for API benchmarking
 *
 * This structure is used across all APIs, it comprises of the input data
 * (e.g., input matrix A and vectors x and y for SPMV alpha*A*x+beta*y)
 * and space for the outputs (results) which then might be checked against
 * the reference results. It works in conjunction with Arguments
 * which specifies exactly what operation (e.g., transposed) and descriptor
 * is used. The sizes are already adjusted to match the operations.
 */
template <typename T>
struct testdata
{
    // problem sizes, e.g., for non-transposed SPMV the dimension of A is m x n.
    aoclsparse_int m;
    aoclsparse_int n;
    aoclsparse_int k;

    aoclsparse_int bsr_dim;

    aoclsparse_mat_descr descr;

    //Used for trsm: leading dimensions of dense matrices 'x'/'y', provides means to access the correct elements of the matrix within the larger memory layout.
    aoclsparse_int ldx;
    aoclsparse_int ldy;

    // A matrix - Used for all APIs
    aoclsparse_int              nnzA;
    std::vector<aoclsparse_int> csr_row_ptrA;
    std::vector<aoclsparse_int> csr_col_indA;
    std::vector<T>              csr_valA;

    // B matrix - Used for spadd, 2m, sypr, add
    // Its dimension is automatically derived from m/n/k,
    // the API type and operation or as given in --mtxB file.
    aoclsparse_int              mB;
    aoclsparse_int              nB;
    aoclsparse_int              nnzB;
    std::vector<aoclsparse_int> csr_row_ptrB;
    std::vector<aoclsparse_int> csr_col_indB;
    std::vector<T>              csr_valB;

    // multipliers
    T alpha;
    T beta;

    // vectors compatible with (including sizes): op(A) . x = y
    std::vector<T> x_in; // input x
    std::vector<T> x; // input
    std::vector<T> y_in; // input y
    std::vector<T> y; // result of this computation

    //Level-1
    std::vector<aoclsparse_int>
        indx; //index array to store the indices of the nnz elements in the sparse vector
    T   s; // scalar used to store result in doti, dotci, dotui, dotmv

    //Solvers, Level-3
    std::vector<T> b; // rhs in case of symgs, trsm

    // C matrix (computed result) for validation
    aoclsparse_int              mC;
    aoclsparse_int              nC;
    aoclsparse_int              nnzC;
    aoclsparse_index_base       baseC;
    std::vector<aoclsparse_int> csr_row_ptrC;
    std::vector<aoclsparse_int> csr_col_indC;
    std::vector<T>              csr_valC;
};

/* Type for any test function to be added to the testqueue, either internal (AOCL)
 * or for external benchmarking.
 * Arguments:
 * - arg [input] - to be used to understand the parameters of the tests
 *     (descriptor, transpose, etc.)
 * - td [input/output] - generated data (such as matrices and input vectors),
 *     these shouldn't be changed by the individual test, and workspace and
 *     output data. The last result of the computation should be preserved
 *     to be check against reference results.
 * - timings[arg.iter] - cpu clock measurements for each computation over
 *     the arg.iter iterations
 * Returns: 0 on success, an error otherwise
 */
template <typename T>
using testfunc = std::function<int(const Arguments &, testdata<T> &, double[])>;

/* structure to join test name and test function into one element to build a test queue */
template <typename T>
struct testsetting
{
    std::string name; // name of the test for printing purposes
    testfunc<T> tf; // test function which runs the computation over several iterations
};

// Add to test queue all variants of test_function with varying kid,
// in the same order as given in --kid= argument by the user
template <typename T, typename FN>
void populate_queue_kid(std::vector<testsetting<T>> &testqueue,
                        const Arguments             &arg,
                        FN                           test_function)
{
    std::string prob_name;

    for(size_t i = 0; i < arg.kid_list.size(); ++i)
    {
        auto wrapper
            = [i, test_function](const Arguments &arg, testdata<T> &td, double timings[]) -> int {
            return test_function(arg, td, timings, arg.kid_list[i]);
        };

        if(arg.kid_list[i] == -1)
            prob_name = "aocl";
        else
            prob_name = "aocl/kid=" + std::to_string(arg.kid_list[i]);

        testqueue.push_back({prob_name, wrapper});
    }
}

#endif // AOCLSPARSE_ARGUMENTS_HPP
