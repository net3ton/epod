#include "gameLevel.h"
#include "levels.h"
#include "levelStats.h"
#include "blocks.h"
#include "enemies.h"
#include "res.h"
#include "main.h"
#include "enemies/bossOne.h"
#include "menu/pause.h"
#include "menu/complete.h"
#include "menu/failed.h"

#include <epod_fsystem.h>


Level::Level()
{
}


void Level::init()
{
  ep::snd::fade_to_music(&::res.mSounds.inlevel);
  ba::stats().reset(m_id);

  LevelBase::init();
}


void Level::load(int id)
{
  ep::log("starting level with id = %d", id);

  m_id = id;

  char name[256];
  sprintf(name, "levels/level_%02d.lvl", id);

  ep::File p(name, ep::FILE_MODE_READ);
  if (!p.opened())
  {
    ep::log("level loading failed!");
    return;
  }

  p.readInt(); // version

  int top = p.readInt();
  int left = p.readInt();
  int size = p.readInt();
  int h = p.readInt();
  int w = p.readInt();

  int count = h * w;
  for (int i=0; i<count; ++i)
  {
    int blockType = p.readInt();
    if (blockType == BLOCK_NONE)
      continue;

    int x = left + (i % w) * size;
    int y = top + (i / w) * size;

    int enemyId = get_enemy_id(blockType);
    
    if (enemyId != ENEMY_NONE)
    {
      EnemyBase *enemy = ::put_enemy(enemyId, x, y, m_blocks);
      if (enemy)
      {
        enemy->showAnim();
        m_enemies.push_back(enemy);
      }
      continue;
    }

    IBlockBase *block = ::put_block(blockType, x, y);
    if (block)
    {
      block->showAnim();
      m_blocks.push_back(block);
    }
  }

  //m_enemies.push_back(::put_enemy(ENEMY_BOSS01, 160, 100, m_blocks));
}


void Level::restart()
{
  ba::stats().reset(m_id);

  for (int i=0; i<m_enemies.size(); ++i)
    m_enemies[i]->reset();

  for (int i=0; i<m_blocks.size(); ++i)
    m_blocks[i]->reset();

  LevelBase::restart();
}


void Level::completeLevel(bool is_ok)
{
  LevelBase::completeLevel(is_ok);

  showLevelEndMenu(is_ok);
}


void Level::showLevelEndMenu(bool complete)
{
  destroyUpMenu();

  m_hud.anim(false);

  if (complete)
    m_upMenu = MenuComplete::getMenu();
  else
    m_upMenu = MenuFailed::getMenu();

  m_upMenu->init();

  m_hud.showPauseButton(false);
}


void Level::levelUpdate(long dt)
{
  levelCompleteCheck();
}


void Level::levelCompleteCheck()
{
  if (m_state == STATE_COMPLETE)
    return;

  for (int i=0; i<m_blocks.size(); ++i)
  {
    IBlockBase *block = m_blocks[i];
    if (!block->isDone())
      return;
  }

  m_ball.stop();
  m_stateTimer.start(2000);
  m_state = STATE_COMPLETE;
}
