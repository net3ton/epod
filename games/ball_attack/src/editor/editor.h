#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__

#include <epod.h>
#include <math/point2.h>
#include <math/rect2.h>
#include <vector>

#include "../sceneBase.h"
#include "editorHud.h"

class MenuEditorLevels;
class IBlockBase;

namespace ba
{

void start_editor();
void destroy_editor();

}


class LevelEditor
  :public BaseScene
{
public:
  LevelEditor();
  ~LevelEditor();

  virtual void init();
  virtual void destroy();

  virtual void update(long dt);
  virtual void render(ep::IGraphics *graphics);

  // input
  virtual bool onPointerDown(int idx, int x, int y);
  virtual bool onPointerUp(int idx, int x, int y);
  virtual bool onPointerMove(int idx, int x, int y);

  virtual bool onKeyDown(int key);
  virtual bool onKeyUp(int key);

  virtual void onPause() {}
  virtual void onResume() {}

  virtual void onMoveinEnd() {}
  virtual void onMoveoutStart() {}

  void showLevelsMenu(bool show, bool to_save);

  void save(int id);
  void load(int id);

private:
  MenuEditorHud m_hud;
  MenuEditorLevels *m_levelsMenu;

  void setGridPoint(int x, int y, IBlockBase *value);
  void setBlock(int ind, int id);

  std::vector<IBlockBase*> m_grid;
  IBlockBase* m_mode;

  int m_curBlockId, m_curBackId;

  bool m_drawGrid;
};

#endif //__LEVEL_EDITOR_H__
