#ifndef __BLOCK_DOUBLE_H__
#define __BLOCK_DOUBLE_H__

#include "simple.h"


class BlockDouble
  :public BlockSimple
{
public:
  BlockDouble():m_hits(0) {}

  virtual void reset();
  virtual void render(ep::IGraphics *graphics);
  virtual void collide(const Point2f &from, bool power);

protected:
  int m_hits;
};


class BlockTriple
  :public BlockDouble
{
public:
  BlockTriple() {}

  virtual void render(ep::IGraphics *graphics);
  virtual void collide(const Point2f &from, bool power);
};


#endif //__BLOCK_DOUBLE_H__
