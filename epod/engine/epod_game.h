#ifndef _EPOD_GAME_H_
#define _EPOD_GAME_H_

#include "epod_scene.h"

#include <auto_ptr.h>
#include <list>


namespace ep
{

class Font;
struct FpsRenderer;

typedef std::list<IScene *> ScenesList;


class GameBase
  :public IScene
{
public:
  GameBase();
  virtual ~GameBase();

  virtual void init();
  virtual void destroy();

  virtual void setCurScene(IScene *scene);

  virtual void update(long dt);
  virtual void render(IGraphics *graphics);
  virtual void renderScene(IGraphics *graphics);

  // input
  virtual bool onPointerDown(int idx, int x, int y);
  virtual bool onPointerUp(int idx, int x, int y);
  virtual bool onPointerMove(int idx, int x, int y);

  virtual bool onKeyDown(int key);
  virtual bool onKeyUp(int key);

  // events
  virtual void onPause();
  virtual void onResume();

  ///
  void setFpsDisplay(Font *fps_font);

protected:
  virtual void setScene(IScene *scene) { m_scene = scene; }
  virtual IScene *getScene() { return m_scene; }

  void sceneToDestroy(IScene *scene);
  void destroyScenes();

private:
  TAutoPtr<IGraphics> m_graphics;
  TAutoPtr<FpsRenderer> m_fpsRenderer;

  IScene *m_scene;
  ScenesList m_scenesToDestroy;
};

} // namespace ep

#endif //_EPOD_GAME_H_
