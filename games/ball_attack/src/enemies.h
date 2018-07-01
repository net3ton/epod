#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "types.h"

/// Enemies types
enum
{
  ENEMY_NONE = 0,

  ENEMY_BOSS01 = 1,
};

int get_enemy_id(int block_type);

EnemyBase *put_enemy(int type, int x, int y, Blocks &blocks);
void destroy_enemy(EnemyBase *enemy);

#endif //__ENEMIES_H__
 