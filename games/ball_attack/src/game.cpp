#include "game.h"
#include "res.h"
#include "profile.h"
#include "menu/loading.h"

enum
{
  MOVE_SCENE_TIME = 350,

  MOUNT3_COLOR = 0x50FFFFFF,
  MOUNT2_COLOR = 0x96FFFFFF,
  MOUNT1_COLOR = 0xD0FFFFFF,

  STARS_COUNT = 40,
};


BallAttackGame::BallAttackGame()
  :m_moveScene(NULL)
  ,m_moveScenePos(0.f), m_moveDist(0.f), m_moveSceneRight(true)
  ,m_mount1Pos(0), m_mount2Pos(0), m_mount3Pos(0)
  ,m_drawSky(true)
{
}


void BallAttackGame::init()
{
  ep::GameBase::init();

  // show fps
  setFpsDisplay(&::res.mFonts.system);

  pf::load();

  for (int i=0; i<STARS_COUNT; ++i)
  {
    Point2f pos(rand() % 320, rand() % 320);
    m_stars.cast(pos);
  }

  setCurScene(MenuLoading::getMenu());
}


void BallAttackGame::onPause()
{
  ep::GameBase::onPause();
  ep::log("pause");
}


void BallAttackGame::onResume()
{
  ep::GameBase::onResume();
  ep::log("resume");
}


void BallAttackGame::update(long dt)
{
  ep::GameBase::update(dt);

  m_stars.update(dt);

  if (m_moveScene)
  {
    m_moveScene->update(dt);

    m_moveTimer.update(dt);
    m_moveScenePos = ep::get_width() * cos(m_moveTimer.getVal(HALFPI));

    if (m_moveTimer.isEnd())
    {
      sceneToDestroy(getScene());

      setScene(m_moveScene);
      m_moveScene->onMoveinEnd();
      m_moveScene = NULL;

      m_moveScenePos = 0.f;
      m_moveDist += m_moveSceneRight ? ep::get_width() : -ep::get_width();
    }

    const float dx = m_moveSceneRight ? m_moveScenePos : -m_moveScenePos;
    const int WIDTH = ep::get_width();

    m_mount3Pos = (int)((m_moveDist + dx) * 0.3f) % WIDTH;
    m_mount2Pos = (int)((m_moveDist + dx) * 0.6f) % WIDTH;
    m_mount1Pos = (int)(m_moveDist + dx) % WIDTH;
  }
}


void BallAttackGame::setCurScene(BaseScene *scene)
{
  if (m_moveScene)
  {
    m_moveScene->destroy();
    m_moveScene = NULL;
  }

  BaseScene *curScene = getScene();
  if (curScene)
  {
    curScene->onMoveoutStart();
    sceneToDestroy(curScene);
  }

  setScene(scene);
  scene->init();
  scene->onMoveinEnd();
}


void BallAttackGame::moveToScene(BaseScene *scene, bool to_right)
{
  BaseScene *curScene = getScene();
  if (curScene)
    curScene->onMoveoutStart();

  m_moveScene = scene;
  m_moveSceneRight = to_right;
  m_moveTimer.start(MOVE_SCENE_TIME);

  m_moveScene->init();
}


void BallAttackGame::renderScene(ep::IGraphics *g)
{
  const float WIDTH = ep::get_width();
  const float HEIGHT = ep::get_height();

  g->drawGradientRect(0, 0, 320, 480, 0xFF4A0820, 0xFF8C1041, 0xFF4A0820, 0xFF8C1041);

  const float M3_H = 120.f;
  const float M2_H = 72.f;
  const float M1_H = 47.f;

  //g->drawFillRect(0, HEIGHT - M3_H, WIDTH, M3_H, 0x503D993D);
  g->drawGradientRect(0, HEIGHT - M3_H, WIDTH, M3_H, 0x50339933, 0x00339933, 0x50339933, 0x00339933);
  g->drawTile(m_mount3Pos, HEIGHT - M3_H, &::res.mTiles.mount3, MOUNT3_COLOR, ep::A_LEFT_BOTTOM);
  g->drawTile(m_mount3Pos + WIDTH, HEIGHT - M3_H, &::res.mTiles.mount3, MOUNT3_COLOR, ep::A_LEFT_BOTTOM);

  //g->drawFillRect(0, HEIGHT - M2_H, WIDTH, M2_H, 0x903D993D);
  g->drawGradientRect(0, HEIGHT - M2_H, WIDTH, M2_H, 0x96339933, 0x00339933, 0x96339933, 0x00339933);
  g->drawTile(m_mount2Pos, HEIGHT - M2_H, &::res.mTiles.mount2, MOUNT2_COLOR, ep::A_LEFT_BOTTOM);
  g->drawTile(m_mount2Pos + WIDTH, HEIGHT - M2_H, &::res.mTiles.mount2, MOUNT2_COLOR, ep::A_LEFT_BOTTOM);

  g->drawFillRect(0, HEIGHT - M1_H, WIDTH, M1_H, 0xD0339933);
  //g->drawGradientRect(0, HEIGHT - M1_H, WIDTH, M1_H, 0xFF3D993D, 0x903D993D, 0xFF3D993D, 0x903D993D);
  g->drawTile(m_mount1Pos, HEIGHT - M1_H, &::res.mTiles.mount1, MOUNT1_COLOR, ep::A_LEFT_BOTTOM);
  g->drawTile(m_mount1Pos + WIDTH, HEIGHT - M1_H, &::res.mTiles.mount1, MOUNT1_COLOR, ep::A_LEFT_BOTTOM);

  m_stars.render(g);

  if (m_drawSky)
  {
    const float S3_H = 45.f;
    const float S2_H = 28.f;

    g->drawFillRect(0, 0, WIDTH, S3_H, 0x50555566);
    g->drawTile(m_mount3Pos, S3_H, &::res.mTiles.sky2, MOUNT3_COLOR);
    g->drawTile(m_mount3Pos + WIDTH, S3_H, &::res.mTiles.sky2, MOUNT3_COLOR);

    g->drawFillRect(0, 0, WIDTH, S2_H, 0x96555566);
    g->drawTile(m_mount2Pos, S2_H, &::res.mTiles.sky, MOUNT2_COLOR);
    g->drawTile(m_mount2Pos + WIDTH, S2_H, &::res.mTiles.sky, MOUNT2_COLOR);
  }

  if (m_moveScene)
    g->wtmTrans(m_moveSceneRight ? m_moveScenePos : -m_moveScenePos, 0);

  ep::GameBase::renderScene(g);

  if (m_moveScene)
  {
    g->wtmTrans(m_moveSceneRight ? (m_moveScenePos - WIDTH) : (WIDTH - m_moveScenePos), 0);
    m_moveScene->render(g);
  }
}
