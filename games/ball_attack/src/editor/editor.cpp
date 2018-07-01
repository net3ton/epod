#include "editor.h"
#include "editorLevels.h"
#include "blockEnemy.h"
#include "../blocks.h"
#include "../enemies.h"
#include "../main.h"
#include "../res.h"

#include <epod_fsystem.h>

enum
{
  VERSION = 1,

  FIELD_TOP = 38,
  FIELD_LEFT = 0,
  PANEL_SIZE = 16,
  COUNT_H = 16,
  COUNT_W = 20,
};


namespace ba
{

LevelEditor *cur_editor = NULL;

void start_editor()
{
  cur_editor = new LevelEditor();
  move_to_scene(cur_editor, false);
}

void destroy_editor()
{
  if (cur_editor)
  {
    delete cur_editor;
    cur_editor = NULL;
  }
}

} // namespace ba


///////////////////////////////////////////////////////////////////////////////


LevelEditor::LevelEditor()
  :m_levelsMenu(NULL)
  ,m_mode(NULL)
  ,m_curBlockId(BLOCK_NONE)
  ,m_curBackId(PANEL_A)
  ,m_drawGrid(true)
{
  m_grid.resize(COUNT_W * COUNT_H, NULL);
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::init()
{
  m_hud.init();
  m_hud.setEditor(this);
}

void LevelEditor::destroy()
{
  m_hud.destroy();
}


void LevelEditor::showLevelsMenu(bool show, bool to_save)
{
  if (show == (m_levelsMenu != NULL))
    return;

  if (show)
  {
    m_levelsMenu = MenuEditorLevels::getMenu();
    m_levelsMenu->setEditor(this, to_save);
    m_levelsMenu->init();
  }
  else
  {
    m_levelsMenu->destroy();
    m_levelsMenu->setEditor(NULL, true);
    m_levelsMenu = NULL;
  }
}


void LevelEditor::save(int id)
{
  char name[256];
  sprintf(name, "levels/level_%02d.lvl", id);

  ep::log(name);
  ep::File p(name, ep::FILE_MODE_WRITE);
  if (!p.opened())
  {
    ep::log("level saving failed!");
    return;
  }

  p.writeInt(VERSION);

  p.writeInt(FIELD_TOP);
  p.writeInt(FIELD_LEFT);
  p.writeInt(PANEL_SIZE);
  p.writeInt(COUNT_H);
  p.writeInt(COUNT_W);

  for (int i=0; i<m_grid.size(); ++i)
    p.writeInt(m_grid[i] ? m_grid[i]->getId() : BLOCK_NONE);
}


void LevelEditor::load(int id)
{
  char name[256];
  sprintf(name, "levels/level_%02d.lvl", id);
  ep::log("level loading '%s'...", name);

  ep::File p(name, ep::FILE_MODE_READ);
  if (!p.opened())
  {
    ep::log("level loading failed!");
    return;
  }

  p.readInt(); // ver

  p.readInt(); // FIELD_TOP
  p.readInt(); // FIELD_LEFT
  p.readInt(); // PANEL_SIZE

  int h = p.readInt(); // COUNT_H
  int w = p.readInt(); // COUNT_W

  if (w != COUNT_W || h != COUNT_H)
  {
    ep::log("wrong grid size!");
    return;
  }

  for (int i=0; i<m_grid.size(); ++i)
  {
    if (m_grid[i])
    {
      delete m_grid[i];
      m_grid[i] = NULL;
    }

    setBlock(i, p.readInt());
  }
}


void LevelEditor::setBlock(int ind, int id)
{
  int x = FIELD_LEFT + (ind % COUNT_W) * PANEL_SIZE;
  int y = FIELD_TOP + (ind / COUNT_W) * PANEL_SIZE;

  if (get_enemy_id(id) != ENEMY_NONE)
  {
    BlockEnemy *block = new BlockEnemy();
    block->init(x, y);
    block->setEnemyBlockId(id);
    block->loadEnemy();

    m_grid[ind] = block;
  }
  else
  {
    m_grid[ind] = ::put_block(id, x, y);
  }
}


void LevelEditor::update(long dt)
{
  if (m_levelsMenu)
    m_levelsMenu->update(dt);
}


void LevelEditor::render(ep::IGraphics *graphics)
{
  if (m_levelsMenu)
  {
    m_levelsMenu->render(graphics);
    return;
  }

  if (m_drawGrid)
  {
    for (int i=0; i<=COUNT_H; ++i)
    {
      int y = FIELD_TOP + i * PANEL_SIZE;
      graphics->drawLine(FIELD_LEFT, y, 320 - FIELD_LEFT, y, 0x88888888);
    }

    for (int i=0; i<=COUNT_W; ++i)
    {
      int x = FIELD_LEFT + i * PANEL_SIZE;
      graphics->drawLine(x, FIELD_TOP, x, FIELD_TOP + COUNT_H * PANEL_SIZE, 0x88888888);
    }
  }

  if (m_mode)
    m_mode->render(graphics);

  for (int i=0; i<m_grid.size(); ++i)
    if (m_grid[i])
      m_grid[i]->render(graphics);

  m_hud.render(graphics);
}


void LevelEditor::setGridPoint(int x, int y, IBlockBase *value)
{
  int i = (x - FIELD_LEFT) / PANEL_SIZE;
  if (i < 0 || i >= COUNT_W)
    i = -1;

  int j = (y - FIELD_TOP) / PANEL_SIZE;
  if (j < 0 || j >= COUNT_H)
    j = -1;

  if (i == -1 || j == -1)
    return;

  int ind = i + j * COUNT_W;

  if (m_grid[ind])
  {
    if (value && m_grid[ind]->getId() == value->getId())
      return;

    delete m_grid[ind];
    m_grid[ind] = NULL;
  }

  if (!value)
    return;

  setBlock(ind, value->getId());
}


bool LevelEditor::onPointerDown(int idx, int x, int y)
{
  if (m_levelsMenu)
    return m_levelsMenu->onPointerDown(idx, x, y);

  if (m_hud.onPointerDown(idx, x, y))
    return true;

  //ep::log("x = %d, y = %d", x, y);

  setGridPoint(x, y, m_mode);
  return true;
}

bool LevelEditor::onPointerUp(int idx, int x, int y)
{
  if (m_levelsMenu)
    return m_levelsMenu->onPointerUp(idx, x, y);

  m_hud.onPointerUp(idx, x, y);
  return true;
}

bool LevelEditor::onPointerMove(int idx, int x, int y)
{
  if (m_levelsMenu)
    return m_levelsMenu->onPointerMove(idx, x, y);

  if (m_hud.onPointerMove(idx, x, y))
    return true;

  setGridPoint(x, y, m_mode);
  return true;
}

bool LevelEditor::onKeyDown(int key)
{
  //ep::log("key = %X", key);

  if (key == 0x09) // tab
  {
    m_drawGrid = !m_drawGrid;
    return true;
  }

  int modeIdCur = m_mode ? m_mode->getId() : BLOCK_NONE;

  if (key == 0xC0) // ~
    m_curBlockId = BLOCK_NONE;
  else if (key == '1')
    m_curBlockId = BLOCK_SIMPLE;
  else if (key == '2')
    m_curBlockId = BLOCK_STATIC;
  else if (key == '3')
    m_curBlockId = BLOCK_EXPLO;
  else if (key == '4')
    m_curBlockId = BLOCK_BONUS;
  else if (key == '5')
    m_curBlockId = BLOCK_DOUBLE;
  else if (key == '6')
    m_curBlockId = BLOCK_TRIPLE;
  else if (key == '7')
    m_curBlockId = BLOCK_MAGNET_IN;
  else if (key == '8')
    m_curBlockId = BLOCK_MAGNET_OUT;
  else if (key == '9')
    m_curBlockId = BLOCK_DYNAMIC;
  else if (key == '0')
    m_curBlockId = BLOCK_ENEMY;

  else if (key == 0x51) // q
    m_curBackId = PANEL_A;
  else if (key == 0x57) // w
    m_curBackId = PANEL_B;
  else if (key == 0x45) // e
    m_curBackId = PANEL_C;
  else if (key == 0x52) // r
    m_curBackId = PANEL_D;
  else if (key == 0x54) // t
    m_curBackId = PANEL_E;
  else if (key == 0x59) // y
    m_curBackId = PANEL_ROUNDED_A;
  else if (key == 0x55) // u
    m_curBackId = PANEL_ROUNDED_B;
  else if (key == 0x49) // i
    m_curBackId = PANEL_ROUNDED_C;
  else if (key == 0x4F) // o
    m_curBackId = PANEL_ROUNDED_D;
  else if (key == 0x50) // p
    m_curBackId = PANEL_ROUNDED_E;

  else if (key == 0x41) // a
    m_curBackId = ENEMY_BOSS01;

  //else if (key == 0x53) // s
  //else if (key == 0x44) // d
  //else if (key == 0x46) // f
  //else if (key == 0x47) // g
  //else if (key == 0x48) // h
  //else if (key == 0x4A) // j
  //else if (key == 0x4B) // k
  //else if (key == 0x4C) // l

  int modeId = m_curBlockId | (m_curBackId << 8);

  if (modeId != modeIdCur)
  {
    if (m_mode)
    {
      delete m_mode;
      m_mode = NULL;
    }

    const int POSX = 0, POSY = 20;

    if (m_curBlockId == BLOCK_ENEMY)
    {
      BlockEnemy *block = new BlockEnemy();
      block->init(POSX, POSY);
      block->setEnemyBlockId(modeId);

      m_mode = block;
    }
    else
    {
      m_mode = ::put_block(modeId, POSX, POSY);
    }
  }  

  return true;
}

bool LevelEditor::onKeyUp(int key)
{
  return true;
}
