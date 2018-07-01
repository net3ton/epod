#ifndef _COMMON_MATH_POINT2_H_
#define _COMMON_MATH_POINT2_H_

#include "common.h"


template <class T>
class Point2T
{
public:
  T x, y;

  inline Point2T() { zero(); }
  inline Point2T(T ax, T ay) { x = ax; y = ay; }

  template <class U>
  explicit Point2T(const Point2T<U> &other) 
	  :x(static_cast<T>(other.x))
	  ,y(static_cast<T>(other.y))
  {
  }

  inline void zero() { x = 0; y = 0; }
  inline void set(T ax, T ay) { x = ax; y = ay; }

  inline bool isZero() const { return (x == 0 && y == 0); }

  inline const T &operator[](int i) const { return (&x)[i]; }
  inline T &operator[](int i) { return (&x)[i]; }

  inline Point2T operator -() const { return Point2T(-x, -y); }
  inline Point2T operator +() const { return *this; }

  inline Point2T operator +(const Point2T &a) const { return Point2T(x + a.x, y + a.y); }
  inline Point2T operator -(const Point2T &a) const { return Point2T(x - a.x, y - a.y); }
  inline Point2T operator *(T a) const { return Point2T(x*a, y*a); }
  inline Point2T operator /(T a) const { return Point2T(x/a, y/a); }

  inline T operator *(const Point2T &a) const { return x*a.x + y*a.y; }

  inline Point2T &operator +=(const Point2T &a) { x+=a.x; y+=a.y; return *this; }
  inline Point2T &operator -=(const Point2T &a) { x-=a.x; y-=a.y; return *this; }
  inline Point2T &operator *=(T a) { x*=a; y*=a; return *this; }
  inline Point2T &operator /=(T a) { x/=a; y/=a; return *this; }

  inline bool operator ==(const Point2T &a) const { return (x == a.x && y == a.y); }
  inline bool operator !=(const Point2T &a) const { return (x != a.x || y != a.y); }

  inline T lengthSq() const { return x*x + y*y; }
  inline float length() const { return sqrtf(lengthSq()); }

  inline void perp() { float t=x; x=-y; y=t; }

  inline void normalize()
  {
    float len = length();
    if (len != 0.f)
    {
      x /= len;
      y /= len;
    }
    else
    {
      //assert(false && "expected non-zero vector length in normalize()");
      zero();
    }
  }

  /// friends
  friend inline Point2T operator *(T a, const Point2T &p) { return Point2T(p.x*a, p.y*a); }
  //friend inline Point2T operator *(const Point2T &p, T a) { return Point2T(p.x*a, p.y*a); }

  friend T lengthSq(const Point2T &a) { return a.x*a.x + a.y*a.y; }
  friend float length(const Point2T &a) { return sqrt(a.lengthSq()); }

  friend Point2T perp(Point2T a) { a.perp(); return a; }
  friend Point2T normalize(Point2T a) { a.normalize(); return a; }

//private:
//  template <class U> void operator *=(U) { assert(false); }
};

//note: return type is void. This forbids implicit conversions like float->int in (Point2i * float) multiplication.
//template <class T, class U> void operator *(U, const Point2T<T> &);
//template <class T, class U> void operator *(const Point2T<T> &, U);

//inline Point2T<float> operator *(int v, const Point2T<float> &p) {return Point2T<float>(p.x*v, p.y*v);};
//inline Point2T<float> operator *(const Point2T<float> &p, int v) {return Point2T<float>(p.x*v, p.y*v);};

///Returns the reflection vector [i - 2 * dot(i, n) * n / dot(n, n)], 
///given the entering ray direction i, and the surface normal n
template <class T> 
inline Point2T<T> reflect(const Point2T<T> &i, const Point2T<T> &n)
{
  float dot_nn = n * n;
  if (dot_nn == 0)
    return i;
  else
    return i - (2 * i * n / dot_nn) * n;
}

/*
template <class T> 
inline Point2T<T> project(const Point2T<T> &a, const Point2T<T> &n)
{
  float nn = n.x*n.x + n.y*n.y;
  if (nn == 0)
    return Point2T<T>(0.f, 0.f);
  else
    return ((a.x*n.x + a.y*n.y)*n/nn);
}
*/

typedef Point2T<int> Point2i;
typedef Point2T<float> Point2f;

#endif // _COMMON_MATH_POINT2_H_
