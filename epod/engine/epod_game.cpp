#include "epod.h"
#include "epod_game.h"
#include "epod_graphics.h"
#include "epod_sound.h"
#include "epod_font.h"

#include <assert.h>


namespace ep
{

struct FpsRenderer
{
public:
  FpsRenderer()
    :m_enabled(false), m_fpsCounter(0), m_fpsTime(0)
    ,m_statsTiles(0), m_statsDrawCalls(0)
  {
  }

  void draw(IGraphics *graphics)
  {
    if (!m_enabled)
      return;

    m_fpsCounter++;

    graphics->accountStats(m_statsTiles, m_statsDrawCalls);

    int time = get_time();
    int fpsDeltaTime = time - m_fpsTime;
    if (fpsDeltaTime >= 1000)
    {
      int fps = 1000 * m_fpsCounter / fpsDeltaTime;
      int tiles = m_statsTiles / m_fpsCounter;
      int drawCalls = m_statsDrawCalls / m_fpsCounter;

      m_label.printf("%d t:%d d:%d", fps, tiles, drawCalls);

      m_fpsTime = time;
      m_fpsCounter = 0;

      m_statsTiles = 0;
      m_statsDrawCalls = 0;
    }

    m_label.draw(graphics);
  }

  void setFont(Font *font)
  {
    m_enabled = true;
    m_label.setFont(font);
    m_label.setPos(2, 0);
    m_label.setColor(C_WHITE);
  }

private:
  bool m_enabled;
  TextRenderer m_label;

  int m_fpsCounter;
  int m_fpsTime;

  int m_statsTiles;
  int m_statsDrawCalls;
};


///////////////////////////////////////////////////////////////////////////////


GameBase::GameBase()
  :m_graphics(new Graphics())
  ,m_fpsRenderer(new FpsRenderer())
  ,m_scene(0)
{
}

GameBase::~GameBase()
{
}


void GameBase::init()
{
  snd::init();
}

void GameBase::destroy()
{
  destroyScenes();

  if (m_scene)
    m_scene->destroy();
  m_scene = 0;

  snd::destroy();
}


void GameBase::setCurScene(IScene *scene)
{
  sceneToDestroy(getScene());

  setScene(scene);
  scene->init();
}


///////////////////////////////////////////////////////////////////////////////

void GameBase::sceneToDestroy(IScene *scene)
{
  if (scene)
    m_scenesToDestroy.push_back(scene);
}

void GameBase::destroyScenes()
{
  if (m_scenesToDestroy.empty())
    return;

  for (ScenesList::iterator i=m_scenesToDestroy.begin(), e=m_scenesToDestroy.end(); i!=e; ++i)
    (*i)->destroy();

  m_scenesToDestroy.clear();
}

void GameBase::update(long dt)
{
  destroyScenes();

  snd::update(dt);

  if (m_scene)
    m_scene->update(dt);
}

void GameBase::render(IGraphics *graphics)
{
  m_graphics->beginRender(ep::get_width(), ep::get_height(), ep::get_view_orient());
  m_graphics->clear();

  renderScene(m_graphics.get());

  m_fpsRenderer->draw(m_graphics.get());

  m_graphics->endRender();
}

void GameBase::renderScene(IGraphics *graphics)
{
  if (m_scene)
    m_scene->render(m_graphics.get());
}

void GameBase::setFpsDisplay(Font *fps_font)
{
  m_fpsRenderer->setFont(fps_font);
}

///////////////////////////////////////////////////////////////////////////////


bool GameBase::onPointerDown(int idx, int x, int y)
{
  int cx = 0, cy = 0;
  m_graphics->translatePoint(x, y, cx, cy);

  if (m_scene)
    return m_scene->onPointerDown(idx, cx, cy);

  return false;
}

bool GameBase::onPointerUp(int idx, int x, int y)
{
  int cx = 0, cy = 0;
  m_graphics->translatePoint(x, y, cx, cy);

  if (m_scene)
    return m_scene->onPointerUp(idx, cx, cy);

  return false;
}

bool GameBase::onPointerMove(int idx, int x, int y)
{
  int cx = 0, cy = 0;
  m_graphics->translatePoint(x, y, cx, cy);

  if (m_scene)
    return m_scene->onPointerMove(idx, cx, cy);

  return false;
}

bool GameBase::onKeyDown(int key)
{
  if (m_scene)
    return m_scene->onKeyDown(key);

  return false;
}

bool GameBase::onKeyUp(int key)
{
  if (m_scene)
    return m_scene->onKeyUp(key);

  return false;
}


///////////////////////////////////////////////////////////////////////////////


void GameBase::onPause()
{
  if (m_scene)
    m_scene->onPause();
}

void GameBase::onResume()
{
  if (m_scene)
    m_scene->onResume();
}

} // namespace ep
