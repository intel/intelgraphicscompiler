/*========================== begin_copyright_notice ============================

Copyright (C) 2017-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

#include "../include/BiF_Definitions.cl"
#include "../../Headers/spirv.h"
#include "../IMF/FP32/asinpi_s_la.cl"

#if defined(cl_khr_fp64)
    #include "../IMF/FP64/asinpi_d_la.cl"
#endif // defined(cl_khr_fp64)

INLINE float __builtin_spirv_OpenCL_asinpi_f32( float x )
{
    return __ocl_svml_asinpif(x);
}

GENERATE_VECTOR_FUNCTIONS_1ARG( __builtin_spirv_OpenCL_asinpi, float, float, f32 )

#if defined(cl_khr_fp64)

INLINE double __builtin_spirv_OpenCL_asinpi_f64( double x )
{
    return __ocl_svml_asinpi(x);
}

GENERATE_VECTOR_FUNCTIONS_1ARG( __builtin_spirv_OpenCL_asinpi, double, double, f64 )

#endif // defined(cl_khr_fp64)

#if defined(cl_khr_fp16)

INLINE half __builtin_spirv_OpenCL_asinpi_f16( half x )
{
    return M_1_PI_H * __builtin_spirv_OpenCL_asin_f16(x);
}

GENERATE_VECTOR_FUNCTIONS_1ARG( __builtin_spirv_OpenCL_asinpi, half, half, f16 )

#endif // defined(cl_khr_fp16)
