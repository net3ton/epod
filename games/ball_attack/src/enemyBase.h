#ifndef __ENEMY_BASE_H__
#define __ENEMY_BASE_H__

#include "types.h"
#include "collision.h"

namespace ep { class IGraphics; }


class EnemyBase
{
public:
  EnemyBase();

  virtual void init(int x, int y, Blocks &blocks);
  virtual void reset();

  virtual void update(long dt) {}
  virtual void render(ep::IGraphics *graphics) {}

  void showAnim();

protected:
  virtual const char *getName() { return 0; }

  void updateBbox();
  void load(const char *id);
  void move(const Point2f &dir);

  Aabb m_bbox;
  Blocks m_blocks;
};


#endif //__ENEMY_BASE_H__
