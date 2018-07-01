#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__

#include <epod_scene.h>


class BaseScene:
  public ep::IScene
{
public:
  virtual void onMoveinEnd() = 0;
  virtual void onMoveoutStart() = 0;
};

#endif //__SCENE_BASE_H__
