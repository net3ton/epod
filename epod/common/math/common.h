#ifndef _COMMON_MATH_COMMON_H_
#define _COMMON_MATH_COMMON_H_

#include <assert.h>
#include <math.h>


/// PI, 2*PI, PI/2
const float PI = 3.1415926535f;
const float TWOPI = 6.283185307f;
const float HALFPI = 1.570796326794895f;

#define DEG_TO_RAD (PI / 180.f)
#define RAD_TO_DEG (180.f / PI)

/// converts degrees to radians
inline float deg_to_rad(float deg) { return deg * DEG_TO_RAD; }
/// converts radians to degrees
inline float rad_to_deg(float rad) { return rad * RAD_TO_DEG; }

inline int iabs(int val) { return (val < 0) ? -val : val; }

#endif // _COMMON_MATH_COMMON_H_
