#include "epod.h"
#include <string>

namespace ep
{

IScene *root_scene = 0;

IScene *get_root_scene()
{
  return root_scene;
}

bool init_epod()
{
  if (root_scene)
  {
    log("EPod device've been inited already!");
    return true;
  }

  root_scene = create_root_scene();
  return (root_scene != 0);
}

void free_epod()
{
  if (!root_scene)
    return;

  root_scene->destroy();

  destroy_root_scene(root_scene);
  root_scene = 0;
}

///////////////////////////////////////////////////////////////////////////////

static std::string ep_app_name;

void set_app_name(const char *name)
{
  ep_app_name = name;
}

const char *get_app_name()
{
  return ep_app_name.c_str();
}

///////////////////////////////////////////////////////////////////////////////

static int ep_width = 0;
static int ep_height = 0;

void set_screen_size(int width, int height)
{
  ep_width = width;
  ep_height = height;
}

int get_scr_width()
{
  return ep_width;
}

int get_scr_height()
{
  return ep_height;
}

int get_width()
{
  int orient = get_screen_orientation();
  switch(orient)
  {
    case SCREEN_ORIENT_PORTRAIT_TOP:
    case SCREEN_ORIENT_PORTRAIT_DOWN:
      return ep_width;

    case SCREEN_ORIENT_LANDSCAPE_LEFT:
    case SCREEN_ORIENT_LANDSCAPE_RIGHT:
      return ep_height;
  }

  logError("not supported screen orientation!");
  return 0;
}

int get_height()
{
  int orient = get_screen_orientation();
  switch(orient)
  {
    case SCREEN_ORIENT_PORTRAIT_TOP:
    case SCREEN_ORIENT_PORTRAIT_DOWN:
      return ep_height;

    case SCREEN_ORIENT_LANDSCAPE_LEFT:
    case SCREEN_ORIENT_LANDSCAPE_RIGHT:
      return ep_width;
  }

  logError("not supported screen orientation!");
  return 0;
}


///////////////////////////////////////////////////////////////////////////////

static int ep_supported_orientations = 0;
static int ep_cur_orientation = 0;

void set_supported_orientations(int orients)
{
  ep_supported_orientations = orients;
}

void set_screen_orientation(int orient)
{
  if (orient & ep_supported_orientations)
    ep_cur_orientation = orient;
}

int get_screen_orientation()
{
  return ep_cur_orientation;
}


///////////////////////////////////////////////////////////////////////////////

void set_supported_locs(const char *locs)
{
  char locsBuff[256];
  strcpy(locsBuff, locs);

  const char *curLoc = get_loc();

  char *loc = strtok(locsBuff, " ");
  if (loc == NULL)
    return;

  char defaultLoc[32];
  strcpy(defaultLoc, loc);

  while (loc != NULL)
  {
    if (!strcmp(loc, curLoc))
      return;

    loc = strtok(NULL, " ");
  }

  set_loc(defaultLoc);
}

} // namespace ep
