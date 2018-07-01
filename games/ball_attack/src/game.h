#ifndef __GAME_H__
#define __GAME_H__

#include <epod.h>
#include <epod_game.h>
#include <timer_int.h>

#include "fx/star.h"
#include "sceneBase.h"


class BallAttackGame
  :public ep::GameBase
{
public:
  BallAttackGame();

  virtual void init();

  // events
  virtual void onPause();
  virtual void onResume();

  virtual void update(long dt);

  virtual void setCurScene(BaseScene *scene);
  void moveToScene(BaseScene *scene, bool to_right);

  void showSky(bool show) { m_drawSky = show; }
  float getMoveTime() { return m_moveTimer.getProgress(); }

private:
  /// to protect from setting up raw ep::IScene
  virtual void setScene(BaseScene *scene) { ep::GameBase::setScene(scene); }
  virtual BaseScene *getScene() { return (BaseScene *)ep::GameBase::getScene(); }

  virtual void renderScene(ep::IGraphics *g);

  BaseScene *m_moveScene;
  bool m_moveSceneRight;
  TimerInt m_moveTimer;

  float m_moveScenePos;
  float m_moveDist;

  int m_mount1Pos, m_mount2Pos, m_mount3Pos;

  StarManager m_stars;
  bool m_drawSky;
};

#endif //__GAME_H__
