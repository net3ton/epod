#ifndef __HINTS_H__
#define __HINTS_H__

namespace ep { class IGraphics; }

enum
{
  HINT_HANDLE_ROCKET = 0,
  HINT_START_BALL,
  HINT_BONUSES,
  HINT_SURVIVAL_LEVEL,
  HINT_COMMON_LEVEL,
};

namespace hints
{
  void on_start_level();
  void on_rocket_handle();
  void on_rocket_release();
  void on_start_ball();

  void on_start_survival();

  void on_bonus();

  void update(long dt);
  void render(ep::IGraphics *graph);

}

#endif //__HINTS_H__
