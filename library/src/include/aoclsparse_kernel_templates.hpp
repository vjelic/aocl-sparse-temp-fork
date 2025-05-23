/* ************************************************************************
 * Copyright (c) 2023-2024 Advanced Micro Devices, Inc. All rights reserved.
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
 * ************************************************************************
 */
#ifndef AOCLSPARSE_KERNEL_TEMPLATES_T_HPP
#define AOCLSPARSE_KERNEL_TEMPLATES_T_HPP

#include "aoclsparse.h"

// Set the kt_int
using kt_int_t = aoclsparse_int;

#ifdef AOCLSPARSE_AVX2_INSTANT
#define KT_AVX2_BUILD
#endif

#include "kernel-templates/kernel_templates.hpp"

// Helper functions and macros that assist in instantiation of KT-based functions
// ------------------------------------------------------------------------------

// Instantiates a kernel template function defined by "FUNC"
#define KT_INSTANTIATE(FUNC, BSZ)   \
    FUNC(BSZ, float);               \
    FUNC(BSZ, double);              \
    FUNC(BSZ, std::complex<float>); \
    FUNC(BSZ, std::complex<double>);

// Instantiates a kernel template function defined by "FUNC"
#define KT_INSTANTIATE_EXT(FUNC, BSZ, EXT) \
    FUNC(BSZ, float, EXT);                 \
    FUNC(BSZ, double, EXT);                \
    FUNC(BSZ, std::complex<float>, EXT);   \
    FUNC(BSZ, std::complex<double>, EXT);

//--------------------------------------------------------------------------------

#endif // AOCLSPARSE_KERNEL_TEMPLATES_T_HPP
