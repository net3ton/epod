#ifndef __LEVEL_STATS_H__
#define __LEVEL_STATS_H__

class LevelStats;


namespace ba
{

LevelStats &stats();

bool is_survival();

} // namespace ba


class LevelStats
{
public:
  LevelStats();

  void reset(int id);
  void resetSurvival();

  bool useLife();
  void addLife();

  void addScores(int val);

  void onRocket();
  void onBonus();
  void onUpdate(int time);

  int getLevelId() const { return m_levelId; }

  int getScores() const { return m_scores; }
  int getLives() const { return m_lives; }
  int getBonuses() const { return m_bonuses; }
  int getTime() const { return m_time; }

private:
  int m_levelId;

  int m_lives;
  int m_scores;
  int m_bonuses;
  int m_time;

  float m_scoresMul;
};

#endif //__LEVEL_STATS_H__
