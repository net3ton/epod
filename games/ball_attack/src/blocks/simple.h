#ifndef __BLOCK_SIMPLE_H__
#define __BLOCK_SIMPLE_H__

#include "../blocks.h"
#include "../collision.h"

#include <math/point2.h>
#include <math/rect2.h>
#include <timer_int.h>


class BlockSimple
  :public IBlockBase
{
public:
  BlockSimple();

  virtual void init(int x, int y);
  virtual void reset();
  virtual void update(long dt);
  virtual void render(ep::IGraphics *graphics);

  virtual void showAnim();

  virtual void collide(const Point2f &from, bool power);
  virtual bool testCollide(const Aabb &b);
  virtual bool sweepCollide(const Point2f &p, const Point2f &dir, float r, float &tmin, Point2f &n);

  virtual bool isDead() const { return m_dead; }
  virtual bool isDone() const { return isDead(); }
  virtual bool isBreakable() const { return true; }

  virtual Point2f getPos() { return m_bbox.center(); }
  virtual const Aabb &getBbox() { return m_bbox; }

  virtual void move(const Point2f &dir) { m_bbox.move(dir); }

  virtual void playKnock();

protected:
  Aabb m_bbox;
  bool m_dead;

  int m_color;

  /// anim in
  TimerInt m_timerDelay, m_timerAnim;
};


class BlockStatic
  :public BlockSimple
{
public:
  virtual void render(ep::IGraphics *graphics);
  virtual void collide(const Point2f &from, bool power);

  virtual bool isDone() const { return true; }
  virtual bool isBreakable() const { return false; }
};


class BlockBonus
  :public BlockSimple
{
public:
  virtual void render(ep::IGraphics *graphics);
  virtual void collide(const Point2f &from, bool power);
};

#endif //__BLOCK_SIMPLE_H__
