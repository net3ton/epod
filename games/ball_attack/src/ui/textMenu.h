#ifndef __UI_TEXT_MENU_H__
#define __UI_TEXT_MENU_H__

#include "text.h"
#include <epod.h>


class TextMenu
  :public UIText
{
public:
  TextMenu(UIMenu *menu, int x, int y, const char *text, int align);
};


#endif //__UI_TEXT_MENU_H__
