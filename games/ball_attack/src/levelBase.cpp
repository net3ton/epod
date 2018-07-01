#include "levelBase.h"
#include "levelStats.h"
#include "levels.h"
#include "blocks.h"
#include "enemies.h"
#include "enemyBase.h"
#include "bonuses.h"
#include "hints.h"
#include "res.h"
#include "main.h"
#include "menu/pause.h"


LevelBase::LevelBase()
  :m_id(-1)
  ,m_state(STATE_INGAME)
  ,m_pause(false)
  ,m_fadeShow(false)
  ,m_bonusPart(0)
  ,m_upMenu(NULL)
  ,m_shield(false)
  ,m_shieldLevel(400)
{
  m_bound1.set(0, 40);
  m_bound2.set(320, 480);

  m_ball.setRadius(5.f);
  m_ball.setSpeed(250.f);

  m_rocket.setLevel(400);
  m_rocket.setPos(320.f / 2);

  m_pauseEndText.setFont(&::res.mFonts.big_nums);
  m_pauseEndText.setAlign(ep::A_CENTER);
  m_pauseEndText.setColor(0xAAFFFFFF);
  m_pauseEndTimeCur = 0;
}

LevelBase::~LevelBase()
{
  for (int i=0; i<m_blocks.size(); ++i)
    ::destroy_block(m_blocks[i]);

  for (int i=0; i<m_enemies.size(); ++i)
    ::destroy_enemy(m_enemies[i]);
}


void LevelBase::init()
{
  m_hud.setLevel(this);
  m_hud.init();
  m_hud.anim(true);

  ballToRocket();

  hints::on_start_level();
}

void LevelBase::destroy()
{
  destroyUpMenu();

  m_bonuses.reset();

  m_hud.destroy();
  m_hud.setLevel(NULL);

  ba::destroy_level(this);
}


void LevelBase::onMoveinEnd()
{
  m_fadeShow = true;
  m_fadeTimer.start(800);

  ba::show_sky(false);
}

void LevelBase::onMoveoutStart()
{
  m_hud.anim(false);

  ba::show_sky(true);

  setPause(true);
  m_fadeShow = false;
}


void LevelBase::destroyUpMenu()
{
  m_hud.showPauseButton(true);

  if (m_upMenu)
  {
    m_upMenu->destroy();
    m_upMenu = NULL;
  }
}


void LevelBase::setPause(bool pause)
{
  if (pause)
  {
    m_pause = true;
    m_pauseEndTimer.end();
  }
  else
  {
    if (m_state == STATE_INGAME)
      m_pauseEndTimer.start(2950);
    else
      m_pause = false;
  }
}


void LevelBase::showPauseMenu(bool show)
{
  setPause(show);

  destroyUpMenu();

  if (show)
  {
    m_upMenu = MenuPause::getMenu();
    m_upMenu->init();
  }

  m_hud.showPauseButton(!show);
}


void LevelBase::restart()
{
  m_hud.reset();
  m_bonuses.reset();

  m_pause = false;
  m_pauseEndTimer.end();

  m_shield = false;

  destroyUpMenu();
  ballToRocket();
}


void LevelBase::update(long dt)
{
  m_hud.update(dt);

  if (m_upMenu)
  {
    m_upMenu->update(dt);
    return;
  }

  ba::stats().onUpdate(dt);

  m_rocket.update(dt);

  /// pause
  /////////////////////////////////////////////////////////////////////////////
  if (m_pauseEndTimer.isWorking())
  {
    int curTime = m_pauseEndTimer.getTime() / 1000;
    if (curTime != m_pauseEndTimeCur)
    {
      m_pauseEndText.printf("%d", curTime + 1);
      m_pauseEndTimeCur = curTime;
    }

    if (m_pauseEndTimer.update(dt))
      m_pause = false;
  }

  if (m_pause)
    return;
  /////////////////////////////////////////////////////////////////////////////

  hints::update(dt);

  m_fadeTimer.update(dt);

  if (m_stateTimer.update(dt))
  {
    if (m_state == STATE_COMPLETE)
      completeLevel(true);
    else if (m_state == STATE_FAILED)
      completeLevel(false);
    else if (m_state == STATE_NEW_BALL || m_state == STATE_STICKY_BALL)
      ballToRocket();

    return;
  }

  //m_sparks.update(dt);
  m_fractions.update(dt);
  m_ballPath.update(dt);
  m_exposions.update(dt);
  m_bonusLastTimer.update(dt);

  m_bonuses.update(dt);
  m_bonusPanel.update(dt);

  if (m_state != STATE_BALL_HOLDING)
    m_ball.update(dt);

  for (int i=0; i<m_blocks.size(); ++i)
    if (!m_blocks[i]->isDead())
      m_blocks[i]->update(dt);

  for (int i=0; i<m_enemies.size(); ++i)
    m_enemies[i]->update(dt);

  updateExplo(dt);

  /////////////////////////////////////////////////////////////////////////////

  if (m_state == STATE_BALL_HOLDING)
  {
    m_ball.setPos(m_rocket.getStickBallPos());
    return;
  }
  else if (m_state == STATE_NEW_BALL)
  {
    Point2f dir = m_rocket.getStickBallPos() - m_newBallPos;
    float delta = dt / (float)m_stateTimer.getTime();
    m_newBallPos += dir * delta;
    return;
  }
  else if ( m_state == STATE_STICKY_BALL)
  {
    Point2f pos = m_ball.getPos();
    Point2f dir = m_rocket.getStickBallPos() - pos;
    float delta = dt / (float)m_stateTimer.getTime();
    pos += dir * delta;
    m_ball.setPos(pos);
    return;
  }

  if (m_rocket.collide(m_ball))
  {
    ba::stats().onRocket();

    if (m_ball.isSticky())
    {
      m_stateTimer.start(200);
      m_state = STATE_STICKY_BALL;
    }
  }
  else
  {
    boundsCollisionCheck();
    blocksCollisionCheck(dt);
  }

  levelUpdate(dt);
}


bool LevelBase::hasBlock(IBlockBase *block) const
{
  for (int i=0; i<m_blocks.size(); ++i)
    if (m_blocks[i] == block)
      return true;

  return false;
}


void LevelBase::ballToRocket()
{
  m_state = STATE_BALL_HOLDING;
  m_stateTimer.end();

  m_ball.normal();
  m_ball.setPos(m_rocket.getStickBallPos());
  ba::stats().onRocket();

  m_newBallPos = m_hud.getLivesPos();
}

void LevelBase::pullBall(int x, int y)
{
  if (y > 300)
    y = 300;

  m_state = STATE_INGAME;
  Point2f dir = Point2f(x, y) - m_ball.getPos();
  dir.normalize();
  m_ball.setDir(dir);
}


void LevelBase::render(ep::IGraphics *graphics)
{
  if (m_fadeShow)
  {
    int fadeAlpha = 0x60 - m_fadeTimer.getVal(0x60);
    graphics->drawFillRect(0, 0, ep::get_width(), ep::get_height(), fadeAlpha << 24);
  }

  /////////////////////////////////////////////////////////////////////////////

  for (int i=0; i<m_blocks.size(); ++i)
    m_blocks[i]->render(graphics);

  for (int i=0; i<m_enemies.size(); ++i)
    m_enemies[i]->render(graphics);

  if (m_shield)
    graphics->drawTile(160, m_shieldLevel, &::res.mTiles.survival_line, 0xFFFFFFFF, ep::A_CENTER);

  hints::render(graphics);

  m_ballPath.render(graphics);

  m_bonuses.render(graphics);

  m_ball.render(graphics);
  m_rocket.render(graphics);

  //m_sparks.render(graphics);
  m_fractions.render(graphics);
  m_exposions.render(graphics);

  m_hud.render(graphics);
  m_bonusPanel.render(graphics);

  if (m_state == STATE_NEW_BALL)
    graphics->drawTile(m_newBallPos.x, m_newBallPos.y, &::res.mTiles.ball, 0xFFFFFFFF, ep::A_CENTER);

  if (m_pause && !m_upMenu)
  {
    float part = (m_pauseEndTimer.getTime() % 1000) / 1000.f;
    float scale = 0.5f + 0.8f * (1.f - cos(part));

    graphics->wtmScale(160, 310, scale, scale);
    m_pauseEndText.draw(graphics);
    graphics->wtmIdent();
  }

  if (m_upMenu)
    m_upMenu->render(graphics);
}


void LevelBase::onBlockBreak(float x, float y)
{
  m_bonusPart += 2;

  if ((rand() % 100) >= m_bonusPart || m_bonusLastTimer.isWorking())
    return;

  m_bonusPart = 0;
  m_bonusLastTimer.start(1000);
 
  const int BONUSES_COUNT = 9;
  int bonuses[BONUSES_COUNT][2] =
  {
    BONUS_SPEEDUP,     5,
    BONUS_SPEEDDOWN,   5,
    BONUS_ROCKETBIG,   5,
    BONUS_ROCKETSMALL, 5,
    BONUS_STICKY,      4,
    BONUS_FIREBALL,    2,
    BONUS_SHIELD,      4,
    BONUS_GHOSTBALL,   3,
    BONUS_GRAVITY,     3
  };

  int sum = 0;
  for (int i=0; i<BONUSES_COUNT; ++i)
    sum += bonuses[i][1];

  int part = rand() % sum;

  int cur = 0;
  for (int id=0; id<BONUSES_COUNT; ++id)
  {
    cur += bonuses[id][1];
    if (cur > part)
      dropBonus(bonuses[id][0], x, y);
  }
}


void LevelBase::dropBonus(int id, int x, int y)
{
  m_bonuses.drop(id, x, y);

  hints::on_bonus();
}


bool LevelBase::onPointerDown(int idx, int x, int y)
{
  if (m_upMenu)
    return m_upMenu->onPointerDown(idx, x, y);

  if (m_hud.onPointerDown(idx, x, y))
    return true;

  if (idx == 0)
  {
    if (m_rocket.tryHandle(x, y))
      hints::on_rocket_handle();
  }
  else
  {
    if (m_rocket.isHandled() && m_state == STATE_BALL_HOLDING)
    {
      pullBall(x, y);
      hints::on_start_ball();
    }
  }
  return true;
}

bool LevelBase::onPointerUp(int idx, int x, int y)
{
  if (m_upMenu)
    return m_upMenu->onPointerUp(idx, x, y);

  if (idx == 0)
  {
    if (m_rocket.releaseHandle())
      hints::on_rocket_release();
  }

  m_hud.onPointerUp(idx, x, y);
  return true;
}

bool LevelBase::onPointerMove(int idx, int x, int y)
{
  if (m_upMenu)
    return m_upMenu->onPointerMove(idx, x, y);

  if (m_hud.onPointerMove(idx, x, y))
    return true;

  if (idx == 0 && m_rocket.isHandled())
    m_rocket.setPos(x);
  return true;
}


bool LevelBase::onKeyDown(int key)
{
  // multitouch emulation
  if (key == 32)
  {
    onPointerDown(1, 0, 0);
    return true;
  }

  return false;
}
