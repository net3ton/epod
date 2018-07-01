#include "loading.h"
#include "main.h"
#include "../main.h"
#include "../res.h"


MENU_SINGLETON_IMPLE(MenuLoading)

MenuLoading::MenuLoading()
{
}


void MenuLoading::update(long dt)
{
  //res.mTextures

  ba::set_scene(MenuMain::getMenu());
}
