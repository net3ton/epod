#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include "levelBase.h"


class Level: public LevelBase
{
public:
  Level();
  ~Level() {}

  void load(int id);

  virtual void init();

  virtual void restart();
  virtual void completeLevel(bool is_ok);

private:
  virtual void levelUpdate(long dt);

  void levelCompleteCheck();
  void showLevelEndMenu(bool complete);
};

#endif //__GAME_LEVEL_H__
