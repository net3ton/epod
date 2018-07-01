#ifndef __FX_SPARK_H__
#define __FX_SPARK_H__

#include <math/point2.h>
#include <vector>

namespace ep { class IGraphics; }

class Spark
{
public:
  Spark();

  void cast(const Point2f &pos, const Point2f &dir, float speed, int time);

  void update(long dt);
  void render(ep::IGraphics *graphics);

  bool dead() const { return !m_time; }

private:
  int m_time, m_timeFull;
  int m_color;
  float m_speed;
  Point2f m_pos, m_dir;
  float m_tiwstSpeed, m_twistAccel; 
};


class SparkEmitter
{
public:
  SparkEmitter();

  void castSpark(const Point2f &pos, const Point2f &dir);

  void castSparks(const Point2f &pos);
  void castRandSpark(const Point2f &pos);

  void update(long dt);
  void render(ep::IGraphics *graphics);

private:
  std::vector<Spark> sparks;
  int sparkMarker;
  Point2f emiPos;
};

#endif //__FX_SPARK_H__
