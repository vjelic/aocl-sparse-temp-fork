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
 * ************************************************************************ */
#include "aoclsparse.h"
#include "common_data_utils.h"

#include <iostream>
#include <vector>

template <>
aoclsparse_status itsol_solve(
    aoclsparse_itsol_handle    handle,
    aoclsparse_int             n,
    aoclsparse_matrix          mat,
    const aoclsparse_mat_descr descr,
    const double              *b,
    double                    *x,
    double                     rinfo[100],
    aoclsparse_int             precond(
        aoclsparse_int flag, aoclsparse_int n, const double *u, double *v, void *udata),
    aoclsparse_int monit(
        aoclsparse_int n, const double *x, const double *r, double rinfo[100], void *udata),
    void *udata)
{
    return aoclsparse_itsol_d_solve(handle, n, mat, descr, b, x, rinfo, precond, monit, udata);
}

template <>
aoclsparse_status itsol_solve(
    aoclsparse_itsol_handle    handle,
    aoclsparse_int             n,
    aoclsparse_matrix          mat,
    const aoclsparse_mat_descr descr,
    const float               *b,
    float                     *x,
    float                      rinfo[100],
    aoclsparse_int             precond(
        aoclsparse_int flag, aoclsparse_int n, const float *u, float *v, void *udata),
    aoclsparse_int monit(
        aoclsparse_int n, const float *x, const float *r, float rinfo[100], void *udata),
    void *udata)
{
    return aoclsparse_itsol_s_solve(handle, n, mat, descr, b, x, rinfo, precond, monit, udata);
}

template <>
aoclsparse_status itsol_rci_solve(aoclsparse_itsol_handle   handle,
                                  aoclsparse_itsol_rci_job *ircomm,
                                  double                  **u,
                                  double                  **v,
                                  double                   *x,
                                  double                    rinfo[100])
{
    return aoclsparse_itsol_d_rci_solve(handle, ircomm, u, v, x, rinfo);
}

template <>
aoclsparse_status itsol_rci_solve(aoclsparse_itsol_handle   handle,
                                  aoclsparse_itsol_rci_job *ircomm,
                                  float                   **u,
                                  float                   **v,
                                  float                    *x,
                                  float                     rinfo[100])
{
    return aoclsparse_itsol_s_rci_solve(handle, ircomm, u, v, x, rinfo);
}

template <>
aoclsparse_status itsol_init<double>(aoclsparse_itsol_handle *handle)
{
    return aoclsparse_itsol_d_init(handle);
}

template <>
aoclsparse_status itsol_init<float>(aoclsparse_itsol_handle *handle)
{
    return aoclsparse_itsol_s_init(handle);
}

bool can_exec_blkcsrmv()
{
    Au::X86Cpu Cpu = {0};
    return Cpu.hasFlag(Au::ECpuidFlag::avx512f) && Cpu.hasFlag(Au::ECpuidFlag::avx512vl)
           && aoclsparse_is_avx512_build();
}

// Returns 'true' if AVX512 tests can be executed in the given build
bool can_exec_avx512_tests()
{
    Au::X86Cpu Cpu = {0};
    return Cpu.hasFlag(Au::ECpuidFlag::avx512f) && aoclsparse_is_avx512_build();
}
