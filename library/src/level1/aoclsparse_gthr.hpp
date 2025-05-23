/* ************************************************************************
 * Copyright (c) 2023-2025 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef AOCLSPARSE_GTHR_HPP
#define AOCLSPARSE_GTHR_HPP

#include "aoclsparse.h"
#include "aoclsparse_cntx_dispatcher.hpp"
#include "aoclsparse_kernel_templates.hpp"
#include "aoclsparse_l1_kt.hpp"
#include "aoclsparse_utils.hpp"

/* Gather and gather_zero reference implementations with stride or indexing
 * It is assumed that all pointers and data are valid.
 */
template <typename T, gather_op OP, Index::type I>
inline aoclsparse_status
    gthr_ref(const aoclsparse_int nnz, y_type<T, OP> y, T *x, Index::index_t<I> xi)
{
    y_type<T, OP> yp;

    for(aoclsparse_int i = 0; i < nnz; ++i)
    {
        if constexpr(I == Index::type::strided)
        {
            // treat "xi" as a stride distance
            yp = &y[xi * i];
        }
        else if constexpr(I == Index::type::indexed)
        {
            // treat "xi" as an indexing array
            if(xi[i] < 0)
            {
                return aoclsparse_status_invalid_index_value;
            }

            yp = &y[xi[i]];
        }

        x[i] = *yp; // copy out

        if constexpr(OP == gather_op::gatherz)
        {
            *yp = aoclsparse_numeric::zero<T>(); // zero out
        }
    }

    return aoclsparse_status_success;
}

/*
 * aoclsparse_gthrs dispatcher with strided or indexed array access
 * handles both cases gather and gatherz
 * Note that y is inout for gatherz and in otherwise.
 */
template <typename T, gather_op OP, Index::type I>
aoclsparse_status aoclsparse_gthr_t(
    aoclsparse_int nnz, y_type<T, OP> y, T *x, Index::index_t<I> xi, aoclsparse_int kid = -1)
{
    // Check size
    if(nnz < 0)
    {
        return aoclsparse_status_invalid_size;
    }

    // Quick return if possible
    if(nnz == 0)
    {
        return aoclsparse_status_success;
    }

    // Check pointer arguments
    if(y == nullptr || x == nullptr)
    {
        return aoclsparse_status_invalid_pointer;
    }

    if constexpr(I == Index::type::strided)
    {
        // xi is a stride distance, check distance
        if(xi < 0)
            return aoclsparse_status_invalid_size;
    }
    else
    {
        // xi is an index array, check pointer
        if(xi == nullptr)
        {
            return aoclsparse_status_invalid_pointer;
        }
    }

    using namespace aoclsparse;
    using namespace Dispatch;
    using namespace kernel_templates;

    // Creating pointer to the kernel
    using K = decltype(&gthr_ref<T, OP, I>);

    // clang-format off
    // Table of available kernels
    static constexpr Table<K> tbl[]{
           {gthr_ref<T, OP, I>,                                context_isa_t::GENERIC, 0U | archs::ALL},
           {gthr_kt<bsz::b256, T, OP, kt_avxext::AVX2, I>,     context_isa_t::AVX2,    0U | archs::ALL},
    ORL<K>({gthr_kt<bsz::b256, T, OP, kt_avxext::AVX512VL, I>, context_isa_t::AVX512VL,0U | archs::ALL}),
    ORL<K>({gthr_kt<bsz::b512, T, OP, kt_avxext::AVX512F, I>,  context_isa_t::AVX512F, 0U | archs::ALL})
    };
    // clang-format on

    // Thread local kernel cache
    thread_local K kache  = nullptr;
    K              kernel = Oracle<K>(tbl, kache, kid);

    if(!kernel)
        return aoclsparse_status_invalid_kid;

    // Invoke the kernel
    return kernel(nnz, y, x, xi);
}

#endif /* AOCLSPARSE_GTHR_HPP */
