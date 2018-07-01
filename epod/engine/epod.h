#ifndef _EPOD_MAIN_H_
#define _EPOD_MAIN_H_

#include "epod_scene.h"
#include "epod_interface.h"
#include "epod_platform.h"


namespace ep
{

// For internal use only
bool init_epod();
void free_epod();
IScene *get_root_scene();

/// app info

void set_app_name(const char *name);
const char *get_app_name();

/// screen size

void set_screen_size(int width, int height);

int get_scr_width();
int get_scr_height();

int get_width();
int get_height();

/// screen orientation

enum
{
  SCREEN_ORIENT_INVALID = 0,
  SCREEN_ORIENT_PORTRAIT_TOP = VIEW_ORIENT_NORMAL,
  SCREEN_ORIENT_PORTRAIT_DOWN = VIEW_ORIENT_DOWN,
  SCREEN_ORIENT_LANDSCAPE_LEFT = VIEW_ORIENT_LEFT,
  SCREEN_ORIENT_LANDSCAPE_RIGHT = VIEW_ORIENT_RIGHT,
};

void set_supported_orientations(int orients);
void set_screen_orientation(int orient);
int get_screen_orientation();

// Need to implement
IScene *create_root_scene();
void destroy_root_scene(IScene *root);

}

#endif //_EPOD_MAIN_H_
