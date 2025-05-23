/* ************************************************************************
 * Copyright (c) 2020-2024 Advanced Micro Devices, Inc.
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
#include "aoclsparse_context.h"
#include "aoclsparse_ellmv.hpp"

/*
 *===========================================================================
 *   C wrapper
 * ==========================================================================
 */

/*
 *  ELLMV
 * ===============================================
 */
extern "C" aoclsparse_status aoclsparse_sellmv(aoclsparse_operation       trans,
                                               const float               *alpha,
                                               aoclsparse_int             m,
                                               aoclsparse_int             n,
                                               aoclsparse_int             nnz,
                                               const float               *ell_val,
                                               const aoclsparse_int      *ell_col_ind,
                                               aoclsparse_int             ell_width,
                                               const aoclsparse_mat_descr descr,
                                               const float               *x,
                                               const float               *beta,
                                               float                     *y)
{
    return aoclsparse_ellmv_t<float>(
        trans, alpha, m, n, nnz, ell_val, ell_col_ind, ell_width, descr, x, beta, y);
}
extern "C" aoclsparse_status aoclsparse_dellmv(aoclsparse_operation       trans,
                                               const double              *alpha,
                                               aoclsparse_int             m,
                                               aoclsparse_int             n,
                                               aoclsparse_int             nnz,
                                               const double              *ell_val,
                                               const aoclsparse_int      *ell_col_ind,
                                               aoclsparse_int             ell_width,
                                               const aoclsparse_mat_descr descr,
                                               const double              *x,
                                               const double              *beta,
                                               double                    *y)
{
    return aoclsparse_ellmv_t<double>(
        trans, alpha, m, n, nnz, ell_val, ell_col_ind, ell_width, descr, x, beta, y);
}

/*
 *  ELLTMV
 * ===============================================
 */
extern "C" aoclsparse_status aoclsparse_selltmv(aoclsparse_operation       trans,
                                                const float               *alpha,
                                                aoclsparse_int             m,
                                                aoclsparse_int             n,
                                                aoclsparse_int             nnz,
                                                const float               *ell_val,
                                                const aoclsparse_int      *ell_col_ind,
                                                aoclsparse_int             ell_width,
                                                const aoclsparse_mat_descr descr,
                                                const float               *x,
                                                const float               *beta,
                                                float                     *y)
{
    return aoclsparse_elltmv_t<float>(
        trans, alpha, m, n, nnz, ell_val, ell_col_ind, ell_width, descr, x, beta, y);
}

extern "C" aoclsparse_status aoclsparse_delltmv(aoclsparse_operation       trans,
                                                const double              *alpha,
                                                aoclsparse_int             m,
                                                aoclsparse_int             n,
                                                aoclsparse_int             nnz,
                                                const double              *ell_val,
                                                const aoclsparse_int      *ell_col_ind,
                                                aoclsparse_int             ell_width,
                                                const aoclsparse_mat_descr descr,
                                                const double              *x,
                                                const double              *beta,
                                                double                    *y)
{
    return aoclsparse_elltmv_t<double>(
        trans, alpha, m, n, nnz, ell_val, ell_col_ind, ell_width, descr, x, beta, y);
}

/*
 *  ELLTHYMV
 * ===============================================
 */
extern "C" aoclsparse_status aoclsparse_sellthybmv(aoclsparse_operation       trans,
                                                   const float               *alpha,
                                                   aoclsparse_int             m,
                                                   aoclsparse_int             n,
                                                   aoclsparse_int             nnz,
                                                   const float               *ell_val,
                                                   const aoclsparse_int      *ell_col_ind,
                                                   aoclsparse_int             ell_width,
                                                   aoclsparse_int             ell_m,
                                                   const float               *csr_val,
                                                   const aoclsparse_int      *csr_row_ind,
                                                   const aoclsparse_int      *csr_col_ind,
                                                   aoclsparse_int            *row_idx_map,
                                                   aoclsparse_int            *csr_row_idx_map,
                                                   const aoclsparse_mat_descr descr,
                                                   const float               *x,
                                                   const float               *beta,
                                                   float                     *y)
{
    return aoclsparse_ellthybmv_t<float>(trans,
                                         alpha,
                                         m,
                                         n,
                                         nnz,
                                         ell_val,
                                         ell_col_ind,
                                         ell_width,
                                         ell_m,
                                         csr_val,
                                         csr_row_ind,
                                         csr_col_ind,
                                         row_idx_map,
                                         csr_row_idx_map,
                                         descr,
                                         x,
                                         beta,
                                         y);
}

extern "C" aoclsparse_status aoclsparse_dellthybmv(aoclsparse_operation       trans,
                                                   const double              *alpha,
                                                   aoclsparse_int             m,
                                                   aoclsparse_int             n,
                                                   aoclsparse_int             nnz,
                                                   const double              *ell_val,
                                                   const aoclsparse_int      *ell_col_ind,
                                                   aoclsparse_int             ell_width,
                                                   aoclsparse_int             ell_m,
                                                   const double              *csr_val,
                                                   const aoclsparse_int      *csr_row_ind,
                                                   const aoclsparse_int      *csr_col_ind,
                                                   aoclsparse_int            *row_idx_map,
                                                   aoclsparse_int            *csr_row_idx_map,
                                                   const aoclsparse_mat_descr descr,
                                                   const double              *x,
                                                   const double              *beta,
                                                   double                    *y)
{
    return aoclsparse_ellthybmv_t<double>(trans,
                                          alpha,
                                          m,
                                          n,
                                          nnz,
                                          ell_val,
                                          ell_col_ind,
                                          ell_width,
                                          ell_m,
                                          csr_val,
                                          csr_row_ind,
                                          csr_col_ind,
                                          row_idx_map,
                                          csr_row_idx_map,
                                          descr,
                                          x,
                                          beta,
                                          y);
}
