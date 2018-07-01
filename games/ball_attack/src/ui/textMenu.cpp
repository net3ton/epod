#include "textMenu.h"
#include "../res.h"

TextMenu::TextMenu(UIMenu *menu, int x, int y, const char *text, int align)
  :UIText(menu)
{
  setPos(x, y);
  setSize(0, 0);
  setFont(&::res.mFonts.main);
  setAlign(align);
  setText(text);
}
