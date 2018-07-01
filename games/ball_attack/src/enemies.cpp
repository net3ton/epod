#include "enemies.h"
#include "blocks.h"
#include "enemies/bossOne.h"

#include <epod.h>


static EnemyBase *make_an_enemy(int type)
{
  if (type == ENEMY_NONE)
    return NULL;
  if (type == ENEMY_BOSS01)
    return new EnemyBossOne();

  ep::logError("Unknown enemy type (%d) creation!", type);
  return NULL;
}


void destroy_enemy(EnemyBase *enemy)
{
  delete enemy;
}


int get_enemy_id(int block_type)
{
  if ((block_type & 0xFF) != BLOCK_ENEMY)
    return ENEMY_NONE;

  return (block_type >> 8) & 0xFF;
}


EnemyBase *put_enemy(int type, int x, int y, Blocks &blocks)
{
  EnemyBase *enemy = make_an_enemy(type);
  if (enemy)
    enemy->init(x, y, blocks);

  return enemy;
}
