/*========================== begin_copyright_notice ============================

Copyright (c) 2000-2021 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

============================= end_copyright_notice ===========================*/

#include "../include/BiF_Definitions.cl"
#include "../../Headers/spirv.h"
#include "../IMF/FP32/sinh_s_la.cl"

#if defined(cl_khr_fp64)
    #include "../IMF/FP64/sinh_d_la.cl"
#endif // defined(cl_khr_fp64)

float __builtin_spirv_OpenCL_sinh_f32( float x )
{
    float result;

    if(__FastRelaxedMath && (!__APIRS))
    {
        // For most inputs, we'll use the expansion
        //  sinh(x) = 0.5f * ( e^x - e^-x ):
        float pexp = __builtin_spirv_OpenCL_exp_f32(  x );
        float nexp = __builtin_spirv_OpenCL_exp_f32( -x );
        result = 0.5f * ( pexp - nexp );

        // For x close to zero, we'll simply use x.
        // We use 2^-10 as our cutoff value for
        // "close to zero".
        float px = __builtin_spirv_OpenCL_fabs_f32( x );
        result = ( px > as_float(0x3A800000) ) ? result : x;
    }
    else
    {
        result = __ocl_svml_sinhf(x);
    }

    return result;
}

GENERATE_VECTOR_FUNCTIONS_1ARG_LOOP( __builtin_spirv_OpenCL_sinh, float, float, f32 )

#if defined(cl_khr_fp64)

INLINE double __builtin_spirv_OpenCL_sinh_f64( double x )
{
    return __ocl_svml_sinh(x);
}

GENERATE_VECTOR_FUNCTIONS_1ARG_LOOP( __builtin_spirv_OpenCL_sinh, double, double, f64 )

#endif // defined(cl_khr_fp64)

#if defined(cl_khr_fp16)

INLINE half __builtin_spirv_OpenCL_sinh_f16( half x )
{
    return __builtin_spirv_OpenCL_sinh_f32((float)x);
}

GENERATE_VECTOR_FUNCTIONS_1ARG_LOOP( __builtin_spirv_OpenCL_sinh, half, half, f16 )

#endif // defined(cl_khr_fp16)
