#ifndef __BLOCK_EXPLOSION_H__
#define __BLOCK_EXPLOSION_H__

#include "simple.h"
#include <timer_int.h>
#include <vector>


class BlockExplo
  :public BlockSimple
{
public:
  BlockExplo();

  virtual void render(ep::IGraphics *graphics);
  virtual void collide(const Point2f &from, bool power);

private:
  int m_bombColor;
  TimerInt m_iconTimer;
};


#endif //__BLOCK_EXPLOSION_H__
