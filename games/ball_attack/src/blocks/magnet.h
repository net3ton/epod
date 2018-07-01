#ifndef __BLOCK_MAGNET_H__
#define __BLOCK_MAGNET_H__

#include "simple.h"


class BlockMagnetIn
  :public BlockSimple
{
public:
  BlockMagnetIn():m_power(0.f) {}

  virtual void init(int x, int y);
  virtual void update(long dt);
  virtual void render(ep::IGraphics *graphics);

protected:
  void initMagnet(float radius, float power);

  Aabb m_bboxEffect;
  float m_power;
};


class BlockMagnetOut
  :public BlockMagnetIn
{
public:
  virtual void init(int x, int y);
  virtual void render(ep::IGraphics *graphics);
};


#endif //__BLOCK_MAGNET_H__
