#include "collision.h"


void swap(float &f1, float &f2)
{
  float t = f1;
  f1 = f2;
  f2 = t;
}


bool sweep_segment_aabb(const Point2f &p, const Point2f &dir, const Aabb &b, float &tmin)
{
  const float *dm = &dir.x;
  const float *pm = &p.x;

  tmin = 0.f;
  float tmax = 1.f;

  for (int i=0; i<2; ++i)
  {
    if (dm[i] == 0.f)
    {
      if (pm[i] < b.min[i] || pm[i] > b.max[i])
        return false;
    }
    else
    {
      float odd = 1.f / dm[i];
      float t1 = (b.min[i] - pm[i]) * odd;
      float t2 = (b.max[i] - pm[i]) * odd;

      if (t1 > t2)
        swap(t1, t2);

      if (t1 > tmin)
        tmin = t1;
      if (t2 < tmax)
        tmax = t2;

      if (tmin > tmax)
        return false;
    }
  }

  return true;
}


bool sweep_segment_circle(const Point2f &p, const Point2f &dir, const Point2f &cen, float r, float &tmin)
{
  const Point2f m = p - cen;

  float c = m * m - r * r;

  /// (c < 0.f) - we are inside circle
  /// (c == 0.f) - we are on circle
  if (c <= 0.f)
  {
    tmin = 0.f;
    return true;
  }

  float b = dir * m;

  /// we are outside circle (c > 0.f) and pointing away from circle (b > 0.f)
  if (b > 0.f && c > 0.f)
    return false;

  float a = dir * dir;
  float discr = b * b - a * c;

  if (discr < 0.f)
    return false;

  tmin = (-b - sqrt(discr)) / a;

  return true;
}


bool sweep_circle_aabb(const Point2f &p, const Point2f &dir, float r, const Aabb &b, float &tmin, Point2f &n)
{
  const Aabb box(b, r);

  if (!sweep_segment_aabb(p, dir, box, tmin) || tmin > 1.f)
    return false;

  const Point2f c = p + dir * tmin;

  // voronoy test
  int u = 0, v = 0;
  if (c.x < b.min[0])
    u |= 1;
  if (c.x > b.max[0])
    v |= 1;
  if (c.y < b.min[1])
    u |= 2;
  if (c.y > b.max[1])
    v |= 2;

  int m = u | v;

  // corner intersection
  if (m == 3)
  {
    Point2f corner(u & 1 ? b.min[0] : b.max[0], u & 2 ? b.min[1] : b.max[1]);

    if (!sweep_segment_circle(p, dir, corner, r, tmin) || tmin > 1.f)
      return false;

    n = normalize(p + dir * tmin - corner);
    return true;
  }

  if (u & 1)
    n.set(-1.f, 0.f);
  else if (v & 1)
    n.set(1.f, 0.f);
  else if (u & 2)
    n.set(0.f, -1.f);
  else if (v & 2)
    n.set(0.f, 1.f);

  // edge intersection
  return true;
}
