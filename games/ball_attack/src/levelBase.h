#ifndef __LEVEL_BASE_H__
#define __LEVEL_BASE_H__

#include <epod.h>
#include <math/point2.h>
#include <math/rect2.h>
#include <timer_int.h>
#include <vector>
#include <list>

#include "types.h"
#include "sceneBase.h"
#include "ball.h"
#include "rocket.h"
#include "bonusManager.h"
#include "bonusPanel.h"
#include "menu/hud.h"
#include "fx/spark.h"
#include "fx/fraction.h"
#include "fx/path.h"
#include "fx/explosion.h"


class LevelBase
  :public BaseScene
{
public:
  LevelBase();
  virtual ~LevelBase();

  virtual void init();
  virtual void destroy();

  virtual void update(long dt);
  virtual void render(ep::IGraphics *graphics);

  /// input
  virtual bool onPointerDown(int idx, int x, int y);
  virtual bool onPointerUp(int idx, int x, int y);
  virtual bool onPointerMove(int idx, int x, int y);

  virtual bool onKeyDown(int key);
  virtual bool onKeyUp(int key) { return false; }

  virtual void onPause() { showPauseMenu(true); }
  virtual void onResume() {}

  /// BaseScene
  virtual void onMoveinEnd();
  virtual void onMoveoutStart();

  void showPauseMenu(bool show);

  virtual void restart();
  virtual void completeLevel(bool is_ok) { setPause(true); }

  const Blocks &getBlocks() const { return m_blocks; }
  bool hasBlock(IBlockBase *block) const;

  Rocket &getRocket() { return m_rocket; }
  Ball &getBall() { return m_ball; }

  /// bonuses
  void dropBonus(int id, int x, int y);
  bool endBonus(int id) { return m_bonuses.endBonus(id); }

  void addBonusToPanel(IBonus *bonus) { m_bonusPanel.addBonus(bonus); }
  void deleteBonusFromPanel(IBonus *bonus) { m_bonusPanel.deleteBonus(bonus); }

  void activateShield(bool enable) { m_shield = enable; }

  /// fx
  //void fxCastSpark(const Point2f &pos, const Point2f &dir) { m_sparks.castSpark(pos, dir); }
  void fxCastFraction(const ep::Tile *tile, const Point2f &pos, const Point2f &dir) { m_fractions.cast(tile, pos, dir); }
  void fxCastPath(const Point2f &pos, int type) { m_ballPath.cast(pos, type); }

  void exploBlocks(const Point2f &from, float dist);

protected:
  enum
  {
    STATE_INGAME,
    STATE_BALL_HOLDING,
    STATE_STICKY_BALL,
    STATE_NEW_BALL,
    STATE_COMPLETE,
    STATE_FAILED,
  };

  int m_id;
  int m_state;
  TimerInt m_stateTimer;

  TimerInt m_fadeTimer;
  bool m_fadeShow;

  bool m_pause;
  int m_pauseEndTimeCur;
  TimerInt m_pauseEndTimer;
  ep::TextRenderer m_pauseEndText;
  void setPause(bool pause);

  MenuHud m_hud;
  UIMenu *m_upMenu;
  void destroyUpMenu();

  Ball m_ball;
  Rocket m_rocket;
  Blocks m_blocks;
  Enemies m_enemies;
  BonusManager m_bonuses;
  BonusPanel m_bonusPanel;

  /// fx
  //SparkEmitter m_sparks;
  FractionManager m_fractions;
  PathManager m_ballPath;
  ExploManager m_exposions;

  ///
  void ballToRocket();
  void pullBall(int x, int y);
  Point2f m_newBallPos;

  /// bonuses
  int m_bonusPart;
  TimerInt m_bonusLastTimer;

  virtual void levelUpdate(long dt) {}
  virtual void onBlockBreak(float x, float y);

  /////////////////////////////////////////////////////////////////////////////
  /// shield bonus
  bool m_shield;
  float m_shieldLevel;

  /// collision
  void boundsCollisionCheck();
  Point2f m_bound1, m_bound2;
  
  struct SweepRes
  {
    IBlockBase *block;
    float t;
    Point2f n;
  };

  typedef std::list<SweepRes> SweepResList;
  typedef SweepResList::iterator SweepResIter;

  void blocksCollisionCheck(long dt);
  std::vector<IBlockBase*> m_frameColliders;
  SweepResList m_frameSweepRes;
  /////////////////////////////////////////////////////////////////////////////

  struct ExploBlock
  {
    IBlockBase *block;
    int time;
    Point2f from;
  };

  typedef std::list<ExploBlock> ExploList;
  typedef ExploList::iterator ExploIter;

  ExploList m_exploBlocks;

  void updateExplo(long dt);
};

#endif //__LEVEL_BASE_H__
