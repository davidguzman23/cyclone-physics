#include <float.h>

namespace cyclone2 {
    typedef float real;

    #define real_sqrt sqrtf
    #define real_pow powf
    #define REAL_MAX FLT_MAX

    /** Defines the precision of the absolute magnitude operator. */
    #define real_abs fabsf

    /** Defines the precision of the sine operator.*/
    #define real_sin sinf

    /** Defines the precision of the cosine operator. */
    #define real_cos cosf

    /** Defines the precision of the exponent operator */
    #define real_exp expf

    /** Defines the precision of the floating point modulo operator. */
    #define real_fmod fmodf
}