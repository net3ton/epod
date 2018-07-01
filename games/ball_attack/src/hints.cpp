#include "hints.h"
#include "levels.h"
#include "levelBase.h"
#include "levelStats.h"
#include "res.h"

#include <epod.h>
#include <list>

namespace hints
{

struct HintInfo
{
  int id;
  const ep::Tile *hint;
  Point2f pos;
  int time;
  int color;
};

typedef std::list<HintInfo> HintsList;
typedef HintsList::iterator HintsListIter;

HintsList hints;

void clear_hints()
{
  hints.clear();
}

void add_hint(int id, float x, float y, int time =-1)
{
  for (HintsListIter it=hints.begin(), et=hints.end(); it!=et; ++it)
    if ((*it).id == id)
      return;

  const ep::Tile *tile = 0;

  if (id == HINT_HANDLE_ROCKET)
    tile = &::res.mTiles.hint_handle_rocket;
  else if (id == HINT_START_BALL)
    tile = &::res.mTiles.hint_start_ball;
  else if (id == HINT_BONUSES)
    tile = &::res.mTiles.hint_bonuses;
  else if (id == HINT_SURVIVAL_LEVEL)
    tile = &::res.mTiles.hint_survival_level;

  if (!tile)
    return;

  HintInfo newHint;

  newHint.id = id;
  newHint.hint = tile;
  newHint.pos.set(x, y);
  newHint.time = time;
  newHint.color = 0xFFFFFFFF;

  hints.push_back(newHint);
}

void set_hint_time(int id, int time)
{
  for (HintsListIter i=hints.begin(), e=hints.end(); i!=e; ++i)
  {
    HintInfo &hint = (*i);

    if (hint.id == id)
    {
      hint.time = time;
      break;
    }
  }
}


///////////////////////////////////////////////////////////////////////////////

bool m_ball_start = false;
bool m_survival_start = false;
bool m_bonuses = false;

void on_start()
{
  int xpos = ba::level()->getRocket().getStickBallPos().x - 15;
  add_hint(HINT_HANDLE_ROCKET, xpos, 360, -1);
}

void on_start_level()
{
  clear_hints();

  if (m_ball_start)
    return;

  on_start();
}

void on_rocket_handle()
{
  if (m_ball_start)
    return;

  set_hint_time(HINT_HANDLE_ROCKET, 300);
  add_hint(HINT_START_BALL, 150, 130, -1);
}

void on_start_ball()
{
  if (m_ball_start)
    return;

  m_ball_start = true;
  set_hint_time(HINT_START_BALL, 300);

  if (ba::is_survival())
    on_start_survival();
}

void on_rocket_release()
{
  if (m_ball_start)
    return;

  set_hint_time(HINT_START_BALL, 300);
  on_start();
}


void on_start_survival()
{
  if (!m_ball_start || m_survival_start)
    return;

  m_survival_start = true;
  add_hint(HINT_SURVIVAL_LEVEL, 120, 265, 4000);
}


void on_bonus()
{
  if (m_bonuses)
    return;

  m_bonuses = true;
  add_hint(HINT_BONUSES, 160, 160, 5000);
}


void update(long dt)
{
  for (HintsListIter i=hints.begin(), e=hints.end(); i!=e; )
  {
    HintInfo &hint = (*i);
    if (hint.time != -1)
    {
      hint.time -= dt;
      if (hint.time <= 0)
      {
        hints.erase(i++);
        continue;
      }

      if (hint.time < 500)
      {
        int alpha = 255 * hint.time / 500;
        hint.color = (alpha << 24) | 0x00FFFFFF;
      }
    }

    ++i;
  }
}


void render(ep::IGraphics *graph)
{
  for (HintsListIter i=hints.begin(), e=hints.end(); i!=e; ++i)
  {
    HintInfo &hint = (*i);
    graph->drawTile(hint.pos.x, hint.pos.y, hint.hint, hint.color, ep::A_CENTER);
  }
}

} //namespace hints
