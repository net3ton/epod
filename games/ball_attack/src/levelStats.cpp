#include "levelStats.h"

enum
{
  LEVEL_LIVES = 2,
  SURVIVAL_LIVES = 0,

  LEVEL_ID_SURVIVAL = -1,
};


namespace ba
{

LevelStats level_stats;
LevelStats &stats() { return level_stats; }

bool is_survival() { return level_stats.getLevelId() == LEVEL_ID_SURVIVAL; }

} // namespace ba


LevelStats::LevelStats()
  :m_levelId(-1)
  ,m_lives(1)
  ,m_scores(0)
  ,m_bonuses(0)
  ,m_time(0)
  ,m_scoresMul(1.f)
{
}

void LevelStats::reset(int id)
{
  m_levelId = id;
  m_scores = 0;
  m_lives = LEVEL_LIVES;
  m_bonuses = 0;
  m_time = 0;
}

void LevelStats::resetSurvival()
{
  m_levelId = LEVEL_ID_SURVIVAL;
  m_scores = 0;
  m_lives = SURVIVAL_LIVES;
  m_bonuses = 0;
  m_time = 0;
}

void LevelStats::addScores(int val)
{
  m_scores += (val * m_scoresMul);
  m_scoresMul += 0.5f;
}

void LevelStats::onRocket()
{
  m_scoresMul = 1.f;
}

void LevelStats::onBonus()
{
  m_bonuses++;
}

void LevelStats::onUpdate(int time)
{
  m_time += time;
}

bool LevelStats::useLife()
{
  if (m_lives > 0)
  {
    m_lives--;
    return true;
  }

  return false;
}

void LevelStats::addLife()
{
  m_lives++;
}
