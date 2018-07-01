#ifndef __MENU_LOADING_H__
#define __MENU_LOADING_H__

#include "../uiMenu.h"
#include "../ui/buttonMenu.h"
#include "../ui/textMenu.h"


class MenuLoading:
  public UIMenu
{
MENU_SINGLETON_DEF(MenuLoading)

public:
  MenuLoading();

  virtual void update(long dt);

};


#endif //__MENU_LOADING_H__
