#ifndef __GAME_SURVIVAL_H__
#define __GAME_SURVIVAL_H__

#include "levelBase.h"


class LevelSurvival: public LevelBase
{
public:
  LevelSurvival();
  ~LevelSurvival() {}

  virtual void init();
  virtual void render(ep::IGraphics *graph);

  virtual void restart();
  virtual void completeLevel(bool is_ok);

private:
  virtual void levelUpdate(long dt);

  float m_pathMax;

  float m_path, m_speed;
  int m_pathInt, m_lineInt;

  void levelCompleteCheck();

  void prepareLine(int yoffset);
  int getRandomBlockType();
};

#endif //__GAME_SURVIVAL_H__
