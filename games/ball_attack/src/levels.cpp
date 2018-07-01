#include "levels.h"
#include "gameLevel.h"
#include "gameSurvival.h"
#include "levelStats.h"
#include "profile.h"
#include "main.h"

enum
{
  LEVEL_LAST_ID = 10,
  STARS_MAX = 3,
};


namespace ba
{

LevelBase *cur_level = NULL;

LevelBase *level()
{
  return cur_level;
}


void start_level(int id)
{
  Level *level = new Level();
  cur_level = level;

  level->load(id);
  move_to_scene(cur_level, false);
}


void destroy_level(LevelBase *level)
{
  delete level;

  if (cur_level == level)
    cur_level = NULL;

  ep::log("Level has been destroyed!");
}


void restart_level()
{
  if (cur_level)
    cur_level->restart();
}


void start_next_level()
{
  int id = ba::stats().getLevelId();

  if (id != -1)
    start_level(id + 1);
}


void complete_level(int id)
{
  assert(id == ba::stats().getLevelId() && "wrong level id to complete!");

  int stars = ba::stats().getLives();
  if (stars > STARS_MAX)
    stars = STARS_MAX;

  pf::set_level_stars(id, stars);

  int nextId = id + 1;
  if (nextId > LEVEL_LAST_ID)
    nextId = LEVEL_LAST_ID;

  pf::set_level_last(nextId);

  ep::log("[d] level complete id = %d", ba::stats().getLevelId());
}


///////////////////////////////////////////////////////////////////////////////


void start_survival()
{
  cur_level = new LevelSurvival();
  move_to_scene(cur_level, false);  
}


} // namespace ba
