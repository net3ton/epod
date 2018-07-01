#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <math/point2.h>


void swap(float &f1, float &f2);


class Aabb
{
public:
  float min[2], max[2];

  inline Aabb() { clear(); }
  inline Aabb(const Aabb &two, float r) { expand(two, r); }
  inline Aabb(const Point2f &point, float r)
  {
    min[0] = point.x - r;
    min[1] = point.y - r;
    max[0] = point.x + r;
    max[1] = point.y + r;
  }

  inline void expand(const Aabb &two, float r)
  {
    for (int i=0; i<2; ++i)
      min[i] = two.min[i] - r;
    for (int i=0; i<2; ++i)
      max[i] = two.max[i] + r;  
  }

  inline void add(const Aabb &two)
  {
    if (isEmpty())
    {
      expand(two, 0.f);
      return;
    }

    for (int i=0; i<2; ++i)
      if (two.min[i] < min[i])
        min[i] = two.min[i];
    for (int i=0; i<2; ++i)
      if (two.max[i] > max[i])
        max[i] = two.max[i];
  }

  inline float left() const { return min[0]; }
  inline float top() const { return min[1]; }
  inline float right() const { return max[0]; }
  inline float bottom() const { return max[1]; }

  inline bool isEmpty() { return min[0] == max[0] && min[1] == max[1]; }

  inline void clear() { min[0] = min[1] = max[0] = max[1] = 0.f; }
  inline void set(float x, float y, float w, float h)
  {
    min[0] = x;
    min[1] = y;
    max[0] = x + w;
    max[1] = y + h;
  }

  inline void fromCircles(const Point2f &p1, const Point2f &p2, float r)
  {
    min[0] = p1.x;
    min[1] = p1.y;

    max[0] = p2.x;
    max[1] = p2.y;

    for (int i=0; i<2; ++i)
      if (min[i] > max[i])
        swap(min[i], max[i]);

    min[0] -= r;
    min[1] -= r;

    max[0] += r;
    max[1] += r;
  }

  inline bool test(const Aabb &two) const
  {
    return two.max[0] >= min[0] && two.min[0] <= max[0] &&
      two.max[1] >= min[1] && two.min[1] <= max[1];
  }

  inline bool test(float x, float y) const
  {
    return x >= min[0] && x <= max[0] &&
      y >= min[1] && y <= max[1];
  }

  Point2f center() const
  {
    return Point2f(min[0] + (max[0] - min[0])/2, min[1] + (max[1] - min[1])/2);
  }

  void move(const Point2f &dir)
  {
    min[0] += dir.x;
    min[1] += dir.y;
    max[0] += dir.x;
    max[1] += dir.y;
  }
};


bool sweep_segment_aabb(const Point2f &p, const Point2f &dir, const Aabb &b, float &tmin);
bool sweep_segment_circle(const Point2f &p, const Point2f &dir, const Point2f &cen, float r, float &tmin);
bool sweep_circle_aabb(const Point2f &p, const Point2f &dir, float r, const Aabb &b, float &tmin, Point2f &n);


#endif // __COLLISION_H__
