#ifndef __ROCKET_H__
#define __ROCKET_H__

#include <math/point2.h>
#include <timer_int.h>

class Ball;
namespace ep { class IGraphics; }
namespace ep { class Tile; }


class Rocket
{
public:
  Rocket();

  void update(long dt);
  void render(ep::IGraphics *graphics);

  bool collide(Ball &ball);
  bool collideBonus(const Point2f &pos);

  void setPos(float x);
  void setLevel(int y) { m_pos.y = y; }
  void setBounds(int x1, int x2) { m_bound1 = x1, m_bound2 = x2; }
  void setSize(int size);

  Point2f getStickBallPos();

  bool tryHandle(int x, int y);
  bool releaseHandle();
  bool isHandled() const { return m_handled; }

  enum
  {
    SIZE_NORMAL,
    SIZE_SMALL,
    SIZE_BIG,
  };

private:
  int m_size;
  ep::Tile *m_tile;

  bool checkCollide(Ball &ball);

  Point2f m_pos;
  float m_height, m_width2;
  float m_bound1, m_bound2;
  float m_radius;

  void handle(float x);
  bool m_handled;
  float m_handleOffset, m_handleOffsetLen;
  TimerInt m_handleTimer;
};

#endif //__ROCKET_H__
