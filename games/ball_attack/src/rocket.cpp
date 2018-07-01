#include "rocket.h"
#include "ball.h"
#include "res.h"

#include <epod.h>
#include <epod_graphics.h>

enum
{
  HANDLE_HEIGHT = 40,
  HANDLE_TIME = 300,
};


Rocket::Rocket()
  :m_tile(0)
  ,m_height(20.f)
  ,m_bound1(0.f), m_bound2(320.f)
  ,m_handled(false)
  ,m_handleOffset(0.f), m_handleOffsetLen(0.f)
{
  setSize(SIZE_NORMAL);
}


void Rocket::setSize(int size)
{
  m_size = size;

  if (size == SIZE_SMALL)
  {
    m_tile = &::res.mTiles.rocket3;
    m_width2 = 27.f;
    m_radius = 35.f;
  }
  else if (size == SIZE_BIG)
  {
    m_tile = &::res.mTiles.rocket2;
    m_width2 = 57.f;
    m_radius = 120.f;
  }
  else
  {
    m_tile = &::res.mTiles.rocket;
    m_width2 = 40.f;
    m_radius = 60.f;
  }
}


void Rocket::update(long dt)
{
  if (m_handleOffset)
  {
    m_handleTimer.update(dt);
    float offset = m_handleOffsetLen * sin(m_handleTimer.getVal(HALFPI));
    float delta = m_handleOffset - offset;

    m_handleOffset = offset;
    m_pos.x -= delta;
  }
}

void Rocket::render(ep::IGraphics *graphics)
{
  float ypos = m_pos.y - m_height;

  int handleColor = m_handled ? 0x55FFFFFF : 0x88FFFFFF;
  graphics->drawTile(m_pos.x - m_handleOffset, ypos + HANDLE_HEIGHT + 5, &::res.mTiles.rocket_handle, handleColor, ep::A_CENTER);
  graphics->drawTile(m_pos.x - m_width2, ypos, m_tile, 0xFFFFFFFF);

  /*
  const float rad = m_radius;
  const float cosa = m_width2 / rad;
  const float alpha = acos(cosa);
  const float range = (HALFPI - alpha) * 2.f;

  float h1 = m_pos.y - rad * sin(alpha) + (rad - m_height);

  const int seg = 10;
  for (int i=0; i<seg; ++i)
  {
    float a = alpha + (i * range / seg);
    float x1 = m_pos.x + rad * cos(a);
    float y1 = m_pos.y - rad * sin(a) + (rad - m_height);
    float b = alpha + ((i+1) * range / seg);
    float x2 = m_pos.x + rad * cos(b);
    float y2 = m_pos.y - rad * sin(b) + (rad - m_height);

    graphics->drawLine(x1, y1, x2, y2, 0xFFFFFFFF);
  }

  graphics->drawLine(m_pos.x - m_width2, m_pos.y, m_pos.x - m_width2, h1, 0xFFFFFFFF);
  graphics->drawLine(m_pos.x + m_width2, m_pos.y, m_pos.x + m_width2, h1, 0xFFFFFFFF);

  graphics->drawLine(m_pos.x - m_width2, m_pos.y, m_pos.x + m_width2, m_pos.y, 0xFFFFFFFF);
  */
}

Point2f Rocket::getStickBallPos()
{
  return m_pos - Point2f(0, m_height + 7);
}

bool Rocket::collide(Ball &ball)
{
  //static int i = 0;
  if (checkCollide(ball))
  {
    //i++;
    //ep::log("collide (%d)!", i);
    ::res.mSounds.ball_knock.play();
    return true;
  }

  return false;
}

bool Rocket::checkCollide(Ball &ball)
{
  const Point2f &pos = ball.getPos();
  float r = ball.getRadius();

  if (pos.x + r < m_pos.x - m_width2 ||
      pos.y + r < m_pos.y - m_height ||
      pos.x - r > m_pos.x + m_width2 ||
      pos.y - r > m_pos.y)
    return false;

  const Point2f &last = ball.getPosLast();

  if (last.y + r < m_pos.y)
  {
    Point2f c = m_pos;
    c.y += (m_radius - m_height);

    Point2f n = pos - c;
    if (n.length() <= (m_radius + r))
    {
      n.normalize();
      Point2f dir = ball.getDir();
      Point2f newDir = reflect(dir, n);
      ball.setDir(newDir);
      ball.setPos(c + n * (m_radius + r + 1.f));
      return true;
    }

    return false;
  }
  else if (last.y - r > m_pos.y)
  {
    ball.reflectDirY();
    ball.setPosY(m_pos.y + r + 1.f);
    return true;
  }
  else if (last.x + r < m_pos.x - m_width2)
  {
    ball.reflectDirX();
    ball.setPosX(m_pos.x - m_width2 - r - 1.f);
    return true;
  }
  else if (last.x - r > m_pos.x + m_width2)
  {
    ball.reflectDirX();
    ball.setPosX(m_pos.x + m_width2 + r + 1.f);    
    return true;
  }

  return false;
}

bool Rocket::collideBonus(const Point2f &pos)
{
  if (pos.x < (m_pos.x - m_width2) || pos.x > (m_pos.x + m_width2))
    return false;

  if (pos.y < (m_pos.y - m_height) || pos.y > m_pos.y)
    return false;

  return true;
}

void Rocket::setPos(float x)
{
  x += m_handleOffset;

  if (x < m_bound1)
    x = m_bound1;
  else if (x > m_bound2)
    x = m_bound2;

  m_pos.x = x;
}

bool Rocket::tryHandle(int x, int y)
{
  m_handled = false;

  const float HANDLE_LEN = 80.f;

  if (x < (m_pos.x - HANDLE_LEN) || x > (m_pos.x + HANDLE_LEN))
    return false;

  if (y < (m_pos.y - m_height))
    return false;

  handle(x);
  return true;
}

void Rocket::handle(float x)
{
  m_handled = true;

  m_handleOffsetLen = m_pos.x - x;
  m_handleOffset = m_handleOffsetLen;
  m_handleTimer.start(HANDLE_TIME);
}

bool Rocket::releaseHandle()
{
  if (m_handled)
  {
    m_handled = false;
    return true;
  }

  return false;
}
