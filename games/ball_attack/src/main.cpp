#include <epod.h>
#include "game.h"

// res embedded generated code
#include <res_gen.cpp>
#include <loc.cpp>

rc::Resources res;

BallAttackGame *g_game = 0;

namespace ep
{

IScene *create_root_scene()
{
  ep::set_screen_size(640, 960);
  ep::set_app_name("BallAttack");

  ep::set_supported_orientations(SCREEN_ORIENT_PORTRAIT_TOP | SCREEN_ORIENT_PORTRAIT_DOWN);
  ep::set_screen_orientation(SCREEN_ORIENT_PORTRAIT_TOP);

  ep::set_supported_locs("en ru");
  ep::log("[d] current localization = '%s'", ep::get_loc());

  loc::init(ep::get_loc());
  res.setTilesLang(ep::get_loc());

  g_game = new BallAttackGame();
  return g_game;
}

void destroy_root_scene(IScene *root)
{
  delete root;
}

} // namespace ep


namespace ba
{

bool is_cheats()
{
  return true;
}

void set_scene(BaseScene *scene)
{
  if (g_game)
    g_game->setCurScene(scene);
}

void move_to_scene(BaseScene *scene, bool to_right)
{
  if (g_game)
    g_game->moveToScene(scene, to_right);
}

void show_sky(bool show)
{
  if (g_game)
    g_game->showSky(show);
}

float get_move_progress()
{
  return g_game ? g_game->getMoveTime() : 0.f;
}

} // namespace ba
