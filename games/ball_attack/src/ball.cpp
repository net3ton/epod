#include "ball.h"
#include "res.h"
#include "fx/effects.h"

#include <epod_graphics.h>

enum
{
  STATE_NORMAL,
  STATE_DIE,
  STATE_STOP,
};

const float GRAVITY_POWER = 0.001f;


Ball::Ball()
  :m_state(STATE_NORMAL)
  ,m_type(TYPE_NORMAL)
  ,m_sticky(false)
  ,m_gravity(false)
  ,m_speed(0.f)
  ,m_speedMod(1.f)
  ,m_radius(1.f)
  ,m_color(0xFFFFFFFF)
{
}

void Ball::update(long dt)
{
  m_posLast = m_pos;
  m_pos += m_dir * m_speed * m_speedMod * dt / 1000.f;

  fx::cast_path(m_pos, m_type);

  if (m_gravity)
  {
    Point2f gravityDir(0, GRAVITY_POWER * dt);
    modDir(gravityDir);
  }

  if (m_state == STATE_NORMAL)
    return;

  if (m_timer.update(dt))
  {
    if (m_state == STATE_DIE)
    {
      m_speedMod = 0.f;
      m_color = 0x00FFFFFF;
    }
    else // STATE_STOP
    {
      m_speedMod = 0.f;
      die();
    }
  }

  if (m_timer.isWorking())
  {
    if (m_state == STATE_DIE)
      m_color = 0x00FFFFFF | (m_timer.getVal(255) << 24);
    else // STATE_STOP
      m_speedMod = m_timer.getVal(100) / 100.f;
  }
}

void Ball::render(ep::IGraphics *graphics)
{
  ep::Tile *tile = NULL;

  if (m_type == TYPE_NORMAL)
    tile = &::res.mTiles.ball;
  else if (m_type == TYPE_FIRE)
    tile = &::res.mTiles.ball_fire;
  else if (m_type == TYPE_PLASMA)
    tile = &::res.mTiles.ball_plasma;
  else if (m_type == TYPE_GHOST)
    tile = &::res.mTiles.ball_ghost;

  if (!tile)
    return;

  if (m_sticky)
    graphics->drawTile(m_pos.x, m_pos.y, &::res.mTiles.ball_sticky, m_color, ep::A_CENTER);

  graphics->drawTile(m_pos.x, m_pos.y, tile, m_color, ep::A_CENTER);

  /*
  for (int i=0; i<32; ++i)
  {
    float x1 = m_pos.x + m_radius * cos(i * TWOPI / 32);
    float y1 = m_pos.y + m_radius * sin(i * TWOPI / 32);
    float x2 = m_pos.x + m_radius * cos((i+1) * TWOPI / 32);
    float y2 = m_pos.y + m_radius * sin((i+1) * TWOPI / 32);

    graphics->drawLine(x1, y1, x2, y2, m_color);
  }
  */
}


void Ball::die()
{
  m_timer.start(300);
  m_state = STATE_DIE;
}

void Ball::stop()
{
  m_timer.start(700);
  m_state = STATE_STOP;
}

void Ball::normal()
{
  m_timer.end();
  m_state = STATE_NORMAL;
  m_color = 0xFFFFFFFF;
  m_speedMod = 1.f;
}

//void Ball::reset()
//{
//  normal();
//
//  m_type = TYPE_NORMAL;
//  m_sticky = false;
//  m_gravity = false;
//}


void Ball::modDir(const Point2f &dir)
{
  Point2f newDir = m_dir + dir;
  newDir.normalize();
  setDir(newDir);
}

void Ball::spin(float power)
{
  Point2f newDir = m_dir + Point2f(m_dir.y, -m_dir.x) * power;
  newDir.normalize();
  setDir(newDir);
}
