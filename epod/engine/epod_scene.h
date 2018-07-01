#ifndef _EPOD_SCENE_H_
#define _EPOD_SCENE_H_

///
/// EPod scene interface
///

namespace ep
{

class IGraphics;


class IScene
{
public:
  virtual ~IScene() {}

  virtual void init() = 0;
  virtual void destroy() = 0;

  virtual void update(long dt) = 0;
  virtual void render(IGraphics *graphics) = 0;

  // input
  virtual bool onPointerDown(int idx, int x, int y) = 0;
  virtual bool onPointerUp(int idx, int x, int y) = 0;
  virtual bool onPointerMove(int idx, int x, int y) = 0;

  virtual bool onKeyDown(int key) = 0;
  virtual bool onKeyUp(int key) = 0;

  // events
  virtual void onPause() = 0;
  virtual void onResume() = 0;
};

} // namespace ep

#endif //_EPOD_SCENE_H_
