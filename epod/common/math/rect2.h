#ifndef _COMMON_MATH_RECT2_H_
#define _COMMON_MATH_RECT2_H_

#include "point2.h"


template <class T>
class Rect2T
{
public:
  typedef Point2T<T> PointType;

  T x, y, width, height;

  inline Rect2T():x(0),y(0) { empty(); }
  inline Rect2T(T ax, T ay, T aw, T ah) { set(ax, ay, aw, ah); }

  inline void empty() { width = 0; height = 0; }
  inline bool isEmpty() const { return (width <= 0 || height <= 0); }
  inline void set(T ax, T ay, T aw, T ah) { x = ax; y = ay; width = aw; height = ah; }

  inline T left() const { return x; }
  inline T top() const { return y; }
  inline T right() const { return x + width; }
  inline T bottom() const { return y + height; }

  inline bool operator ==(const Rect2T &r) const { return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height); }
  inline bool operator !=(const Rect2T &r) const { return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height); }

  inline PointType center() const { return PointType(x + width / 2, y + height / 2); }

  inline bool isInside(T ax, T ay) const { return (ax >= x) && (ax <= x + width) && (ay >= y) && (ay <= y + height); }
};


typedef Rect2T<int> Rect2i;
typedef Rect2T<float> Rect2f;

#endif // _COMMON_MATH_RECT2_H_
