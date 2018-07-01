#ifndef __BLOCK_ENEMY_H__
#define __BLOCK_ENEMY_H__

#include "../types.h"
#include "../blocks/simple.h"

#include <epod_font.h>
#include <math/point2.h>
#include <math/rect2.h>

/// Block for editor only!

class BlockEnemy
  :public BlockSimple
{
public:
  BlockEnemy();
  ~BlockEnemy();

  virtual void init(int x, int y);
  virtual void render(ep::IGraphics *graphics);

  /////

  void setEnemyBlockId(int id);
  void loadEnemy();

protected:
  int m_enemyId;
  EnemyBase *m_enemy;

  ep::TextRenderer m_textId;
  Blocks m_blocks;
};

#endif //__BLOCK_ENEMY_H__
