#include "gameSurvival.h"
#include "levels.h"
#include "levelStats.h"
#include "collision.h"
#include "blocks.h"
#include "hints.h"
#include "res.h"
#include "main.h"
#include "menu/failed.h"

enum
{
  FIELD_TOP = 38,
  FIELD_LEFT = 0,
  PANEL_SIZE = 16,
  COUNT_H = 16,
  COUNT_W = 20,
};


LevelSurvival::LevelSurvival()
  :m_pathMax(280), m_path(0.f), m_speed(0.002f)
  ,m_pathInt(0), m_lineInt(-1)
{
}


void LevelSurvival::init()
{
  ep::snd::fade_to_music(&::res.mSounds.inlevel);
  ba::stats().resetSurvival();

  prepareLine(0);

  LevelBase::init();

  hints::on_start_survival();
}


void LevelSurvival::render(ep::IGraphics *graph)
{
  graph->drawTile(160, m_pathMax, &::res.mTiles.survival_line, 0xFFFFFFFF, ep::A_CENTER);

  LevelBase::render(graph);
}


void LevelSurvival::levelUpdate(long dt)
{
  if (m_state == STATE_INGAME)
  {
    float delta = dt * m_speed;
    m_path += delta;

    int curInt = (int)m_path;
    if (m_pathInt != curInt)
    {
      Point2f dir(0, curInt - m_pathInt);
      m_pathInt = curInt;

      for (int i=0; i<m_blocks.size(); ++i)
        m_blocks[i]->move(dir);
    }

    int curLine = (int)m_path / PANEL_SIZE;
    if (curLine != m_lineInt)
    {
      //ep::log("[d] line = %d", curLine);
      m_lineInt = curLine;
      prepareLine(-PANEL_SIZE);
    }
  }

  levelCompleteCheck();
}


void LevelSurvival::levelCompleteCheck()
{
  if (m_state == STATE_FAILED)
    return;

  bool endLevel = false;

  int n = 0;
  for (int i=0; i<m_blocks.size(); ++i)
  {
    if (m_blocks[i]->isDone())
    {
      ::destroy_block(m_blocks[i]);
      continue;
    }

    if (m_blocks[i]->getBbox().bottom() >= m_pathMax)
      endLevel = true;

    m_blocks[n] = m_blocks[i];
    n++;
  }

  m_blocks.erase(m_blocks.begin() + n, m_blocks.end());

  if (endLevel)
  {
    m_ball.stop();
    m_stateTimer.start(1000);
    m_state = STATE_FAILED;
  }
}


void LevelSurvival::restart()
{
  ba::stats().resetSurvival();

  for (int i=0; i<m_blocks.size(); ++i)
    ::destroy_block(m_blocks[i]);

  m_blocks.clear();

  m_path = 0.f;
  m_pathInt = 0;
  m_lineInt = -1;

  prepareLine(0);

  LevelBase::restart();
}


void LevelSurvival::completeLevel(bool is_ok)
{
  LevelBase::completeLevel(is_ok);

  destroyUpMenu();

  m_upMenu = MenuSurvivalEnd::getMenu();
  m_upMenu->init();

  m_hud.showPauseButton(false);
}


int LevelSurvival::getRandomBlockType()
{
  const int BLOCK_TYPES_COUNT = 9;
  int types[BLOCK_TYPES_COUNT] =
  {
    BLOCK_SIMPLE,
    BLOCK_SIMPLE,
    BLOCK_SIMPLE,
    BLOCK_SIMPLE,
    BLOCK_EXPLO,
    BLOCK_BONUS,
    BLOCK_DOUBLE,
    BLOCK_DOUBLE,
    BLOCK_TRIPLE,
  };

  const int BLOCK_BACKS_COUNT = 5;
  int backs[BLOCK_BACKS_COUNT] =
  {
    PANEL_A,
    PANEL_B,
    PANEL_C,
    PANEL_D,
    PANEL_E,
  };

  int blockChance = rand() % 2;
  if (!blockChance)
    return BLOCK_NONE;

  return types[rand() % BLOCK_TYPES_COUNT] | (backs[rand() % BLOCK_BACKS_COUNT] << 8);
}


void LevelSurvival::prepareLine(int yoffset)
{
  for (int i=0; i<COUNT_H; ++i)
  {
    IBlockBase *block = ::create_block(getRandomBlockType());
    if (!block)
      continue;

    int x = FIELD_LEFT + i * PANEL_SIZE;
    int y = FIELD_TOP + yoffset;

    block->init(x, y);
    block->showAnim();
    m_blocks.push_back(block);
  }
}
