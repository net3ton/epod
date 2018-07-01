#include "uiMenu.h"
#include "uiControl.h"


UIMenu::UIMenu()
  :m_focused(NULL)
  ,m_focusCaptured(false)
  ,m_lockControls(false)
{
}


void UIMenu::init()
{
  m_focused = NULL;
  m_lockControls = false;

  for (size_t i=0; i<m_controls.size(); ++i)
    m_controls[i]->init();
}


void UIMenu::destroy()
{
  for (size_t i=0; i<m_controls.size(); ++i)
    m_controls[i]->destroy();
}


void UIMenu::update(long dt)
{
  for (size_t i=0; i<m_controls.size(); ++i)
  {
    UIControl &ctrl = *m_controls[i];

    if (ctrl.isVisible() && ctrl.isEnabled())
      ctrl.update(dt);
  }
}


void UIMenu::render(ep::IGraphics *graph)
{
  for (size_t i=0; i<m_controls.size(); ++i)
  {
    UIControl &ctrl = *m_controls[i];

    if (ctrl.isVisible())
      ctrl.render(graph);
  }
}


void UIMenu::addControl(UIControl *ctrl)
{
  m_controls.push_back(ctrl);
}


void UIMenu::onMoveinEnd()
{
}

void UIMenu::onMoveoutStart()
{
  lockControls(true);
}


bool UIMenu::onPointerDown(int idx, int x, int y)
{
  if (idx || m_lockControls)
    return false;

  assert(!m_focused);

  for (size_t i=0; i<m_controls.size(); ++i)
  {
    UIControl *ctrl = m_controls[i];
    if (!ctrl->isVisible() || !ctrl->isEnabled())
      continue;

    if (ctrl->hitTest(x, y))
    {
      m_focused = ctrl;
      m_focused->onFocus(true);
      m_focused->onPointerDown(x, y);
      return true;
    }
  }

  return false;
}


bool UIMenu::onPointerUp(int idx, int x, int y)
{
  if (idx || m_lockControls)
    return false;

  if (m_focused)
  {
    if (m_focused->hitTest(x, y))
      m_focused->onPointerUp(x, y);
    else
      m_focused->onFocus(false);

    m_focused = NULL;
    return true;
  }

  return false;
}


bool UIMenu::onPointerMove(int idx, int x, int y)
{
  if (idx || m_lockControls)
    return false;

  if (!m_focused)
    return false;

  if (m_focusCaptured || m_focused->hitTest(x, y))
  {
    m_focused->onPointerMove(x, y);
    return true;
  }
  else
  {
    m_focused->onFocus(false);
    m_focused = NULL;
  }

  return false;
}
