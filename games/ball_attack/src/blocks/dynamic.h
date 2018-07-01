#ifndef __BLOCK_DYNAMIC_H__
#define __BLOCK_DYNAMIC_H__

#include "simple.h"


class BlockDynamic
  :public BlockSimple
{
public:
  BlockDynamic();

  virtual void init(int x, int y);
  virtual void reset();
  virtual void update(long dt);

protected:
  Point2f m_initPos;
};


#endif //__BLOCK_DYNAMIC_H__
