#include "spark.h"
#include "../res.h"

#include <epod.h>
#include <epod_graphics.h>


SparkEmitter::SparkEmitter()
  :sparkMarker(0)
{
  sparks.resize(200);
}

void SparkEmitter::castSpark(const Point2f &pos, const Point2f &dir)
{
  sparks[sparkMarker].cast(pos, dir, 50, 2000);

  sparkMarker++;
  if (sparkMarker >= sparks.size())
    sparkMarker = 0;
}

void SparkEmitter::castSparks(const Point2f &pos)
{
  for (int i=0; i<25; ++i)
    castRandSpark(pos);
}

void SparkEmitter::castRandSpark(const Point2f &pos)
{
  Point2f dir(rand() % 200 - 100, rand() % 200 - 100);
  dir.normalize();

  castSpark(pos, dir);
}

void SparkEmitter::update(long dt)
{
  for (int i=0; i<sparks.size(); ++i)
    sparks[i].update(dt);
}

void SparkEmitter::render(ep::IGraphics *graphics)
{
  for (int i=0; i<sparks.size(); ++i)
      sparks[i].render(graphics);
}


///////////////////////////////////////////////////////////////////////////////


Spark::Spark()
  :m_time(0), m_timeFull(0)
  ,m_color(0)
  ,m_speed(0.f)
{
}

void Spark::cast(const Point2f &pos, const Point2f &dir, float speed, int time)
{
  m_pos = pos;
  m_dir = dir;
  m_speed = speed;
  m_time = m_timeFull = time;
  m_color = 0xFFFFFFFF;

  m_tiwstSpeed = (rand() % 500 - 250) / 3000.f;
  m_twistAccel = (rand() % 500 - 250) / 3000.f;
}

void Spark::update(long dt)
{
  if (!m_time)
    return;

  m_time -= dt;
  if (m_time < 0)
    m_time = 0;

  m_pos += m_dir * m_speed * dt / 1000.f;

  //
  //m_dir = m_dir + m_dir.perp() * 0.1f;
  //m_dir.normalize();

  m_dir = m_dir + perp(m_dir) * m_tiwstSpeed;
  m_dir.normalize();
  m_tiwstSpeed += m_twistAccel * dt / 1000.f;
  //

  if (m_time > m_timeFull / 2)
  {
    int green = 0x1FE * (m_timeFull - m_time) / (float)m_timeFull;
    m_color = 0xFFFF0000 | (green << 8);
  }
  else
  {
    int blue = (0x1FE * (m_timeFull - m_time) / (float)m_timeFull) - 0x100;
    m_color = 0xFFFFFF00 | blue;
  }
}

void Spark::render(ep::IGraphics *graphics)
{
  if (!m_time)
    return;

  float len = 5.f * m_time / m_timeFull;
  graphics->drawLine(m_pos.x, m_pos.y, m_pos.x + m_dir.x * len, m_pos.y + m_dir.y * len, m_color);
  //graphics->drawTile(m_pos.x, m_pos.y, ::res.mTiles.part_small1, 0xAA999999);
}
