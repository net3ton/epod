#ifndef __ENEMY_BOSS_ONE_H__
#define __ENEMY_BOSS_ONE_H__

#include "../enemyBase.h"

class EnemyBossOne
  :public EnemyBase
{
public:
  EnemyBossOne();

protected:
  virtual void update(long dt);
  virtual const char *getName() { return "boss_01"; }

  float m_speedX;
};


#endif //__ENEMY_BOSS_ONE_H__
