#ifndef __BALL_H__
#define __BALL_H__

#include <math/point2.h>
#include <timer_int.h>

namespace ep { class IGraphics; }


class Ball
{
public:
  Ball();

  void update(long dt);
  void render(ep::IGraphics *graphics);

  const Point2f &getPos() const { return m_pos; }
  const Point2f &getDir() const { return m_dir; }
  const Point2f &getPosLast() const { return m_posLast; }
  float getRadius() const { return m_radius; }

  void setDir(const Point2f &dir) { m_dir = dir; }
  void setPos(const Point2f &pos) { m_pos = pos; }

  void setType(int type) { m_type = type; }
  void setSticky(int enable) { m_sticky = enable; }
  void setGravity(int enable) { m_gravity = enable; }

  void modDir(const Point2f &dir);
  void spin(float power);

  void setSpeed(float speed) { m_speed = speed; }
  void setRadius(float radius) { m_radius = radius; }

  void die();
  void stop();
  void normal();
  //void reset();

  void reflectDirX() { m_dir.x = -m_dir.x; }
  void reflectDirY() { m_dir.y = -m_dir.y; }
  void setPosX(float x) { m_pos.x = x; }
  void setPosY(float y) { m_pos.y = y; }

  void reflectDirLeft() { if (m_dir.x > 0) reflectDirX(); }
  void reflectDirRight() { if (m_dir.x < 0) reflectDirX(); }
  void reflectDirTop() { if (m_dir.y > 0) reflectDirY(); }
  void reflectDirBottom() { if (m_dir.y < 0) reflectDirY(); }

  enum
  {
    TYPE_NORMAL = 0,
    TYPE_FIRE,
    TYPE_PLASMA,
    TYPE_GHOST,
  };

  bool isFire() const { return (m_type == TYPE_FIRE); }
  bool isPlasma() const { return (m_type == TYPE_PLASMA); }
  bool isGhost() const { return (m_type == TYPE_GHOST); }
  bool isSticky() const { return m_sticky; }

private:
  int m_state;
  int m_type;

  bool m_sticky;
  bool m_gravity;

  Point2f m_pos, m_dir;
  Point2f m_posLast;
  float m_speed, m_speedMod;
  float m_radius;

  int m_color;
  TimerInt m_timer;
};

#endif //__BALL_H__
