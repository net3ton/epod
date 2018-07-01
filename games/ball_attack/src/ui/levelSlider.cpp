#include "levelSlider.h"
#include "../profile.h"
#include "../res.h"


LevelsPage::LevelsPage()
  :m_parent(0)
  ,m_slider(0)
  ,m_starsCur(0)
{
  for (int i=0; i<LEVELS_COUNT; ++i)
    m_btns[i].init(this);
}

void LevelsPage::initPage(UIMenu *parent, LevelsSlider *slider, int first_id)
{
  UIMenu::init();

  m_parent = parent;
  m_slider = slider;
  m_starsCur = 0;

  int lastId = pf::get_level_last();

  for (int i=0; i<LEVELS_COUNT; ++i)
  {
    int id = first_id + i;

    int stars = pf::get_level_stars(id);
    int state = LevelSelector::STATE_DISABLED;

    m_starsCur += stars;

    if (id < 3 || pf::is_levels_bought())
    {
      if (id == lastId && !stars)
        state = LevelSelector::STATE_NEW;
      else if (id <= lastId)
        state = LevelSelector::STATE_OPENED;
    }

    m_btns[i].setState(id, state, stars);
  }
}

void LevelsPage::getBtnPos(int idx, int &x, int &y)
{
  x = 16 + (idx % 5) * 58;
  y = 62 + (idx / 5) * 61;
}

void LevelsPage::setPos(int left)
{
  for (int i=0; i<LEVELS_COUNT; ++i)
  {
    int x, y;
    getBtnPos(i, x, y);

    m_btns[i].setPos(x + left, y);
  }
}

void LevelsPage::addStars(int &max, int &cur)
{
  max += LEVELS_COUNT * 3;
  cur += m_starsCur;
}

void LevelsPage::onAct(UIControl *ctrl)
{
  LevelSelector *sel = (LevelSelector*) ctrl;

  m_slider->setLevelId(sel->getLevelId());
  m_parent->onAct(m_slider);
}


///////////////////////////////////////////////////////////////////////////////


LevelsPageFirst::LevelsPageFirst()
  :m_btnBuy(this)
  ,m_left(0)
{
}

void LevelsPageFirst::initPage(UIMenu *parent, LevelsSlider *slider, int first_id)
{
  LevelsPage::initPage(parent, slider, first_id);

  bool bought = pf::is_levels_bought();

  m_btns[3].setVisible(bought);
  m_btnBuy.setVisible(!bought);
}

void LevelsPageFirst::setPos(int left)
{
  m_left = left;
  LevelsPage::setPos(left);

  int x, y;
  getBtnPos(3, x, y);
  m_btnBuy.setPos(x + left, y);
}

void LevelsPageFirst::render(ep::IGraphics *graph)
{
  LevelsPage::render(graph);

  if (!pf::is_levels_bought())
  {
    int x, y;
    getBtnPos(3, x, y);
    graph->drawTile(x + m_left - 2, y, &::res.mTiles.levels_buy_v, 0xFFFFFFFF);
  }
}

void LevelsPageFirst::onAct(UIControl *ctrl)
{
  if (ctrl == &m_btnBuy)
  {
    m_slider->setLevelId(-1);
    m_parent->onAct(m_slider);
    return;
  }

  LevelsPage::onAct(ctrl);
}


///////////////////////////////////////////////////////////////////////////////


LevelsPageLast::LevelsPageLast()
  :txtMoreLevels(this, 160, 90, "More levels are coming...", ep::A_CENTER)
{
}

void LevelsPageLast::setPos(int left)
{
  txtMoreLevels.setPos(160 + left, 90);
}


///////////////////////////////////////////////////////////////////////////////


LevelsSlider::LevelsSlider(UIMenu *menu)
  :UIControl(menu)
  ,m_current(0), m_next(0)
  ,m_curLevelId(0)
  ,m_pressed(false), m_moving(false)
  ,m_startPos(0), m_curDelta(0), m_moveDelta(0)
  ,m_starsMax(0), m_starsCur(0)
  ,WIDTH(320)
{
  setPos(0, 40);
  setSize(WIDTH, 360);

  m_pages[0] = new LevelsPageFirst();
  m_pages[1] = new LevelsPage();
  m_pages[2] = new LevelsPageLast();
}

LevelsSlider::~LevelsSlider()
{
  for (int i=0; i<COUNT; ++i)
    delete m_pages[i];
}

void LevelsSlider::init()
{
  m_starsMax = 0;
  m_starsCur = 0;

  for (int i=0; i<COUNT; ++i)
  {
    m_pages[i]->initPage(m_menu, this, i * LevelsPage::LEVELS_COUNT);
    m_pages[i]->addStars(m_starsMax, m_starsCur);
  }

  updatePos(0);
}

void LevelsSlider::destroy()
{
  for (int i=0; i<COUNT; ++i)
    m_pages[i]->destroy();
}

void LevelsSlider::updatePos(int delta)
{
  for (int i=0; i<COUNT; ++i)
  {
    int dir = i - m_current;
    m_pages[i]->setPos(dir * WIDTH - delta);
  }
}

bool LevelsSlider::isVisiblePage(int idx)
{
  return (idx == m_current || (m_curDelta > 0 && idx == (m_current + 1)) || (m_curDelta < 0 && idx == (m_current - 1)));
}

void LevelsSlider::update(long dt)
{
  m_timer.update(dt);

  if (m_timer.isWorking())
  {
    int delta = m_moveDelta * cos(m_timer.getVal(HALFPI));
    updatePos(m_curDelta + delta);

    if (m_timer.isReverceEnd())
    {
      if (m_next)
      {
        m_current += m_next;
        m_next = 0;
        updatePos(0);
      }

      m_curDelta = 0;
      m_moveDelta = 0;
    }
  }

  for (int i=0; i<COUNT; ++i)
  {
    if (isVisiblePage(i))
      m_pages[i]->update(dt);
  }
}

void LevelsSlider::render(ep::IGraphics *graph)
{
  for (int i=0; i<COUNT; ++i)
  {
    if (isVisiblePage(i))
      m_pages[i]->render(graph);
  }

  const int TOP = 380;

  graph->drawTile(160, TOP, &::res.mTiles.level_page_panel, 0xFFFFFFFF, ep::A_CENTER);

  for (int i=0; i<COUNT; ++i)
  {
    ep::Tile *ico = (i == m_current) ? &::res.mTiles.btn_level_star_active : &::res.mTiles.btn_level_star_free;
    graph->drawTile(145 + i * 15, TOP, ico, 0xFFFFFFFF, ep::A_CENTER);
  }
}

void LevelsSlider::onPointerDown(int x, int y)
{
  m_pages[m_current]->onPointerDown(0, x, y);

  m_pressed = true;
  m_moving = false;
  m_startPos = x;
  m_curDelta = 0;
  m_moveDelta = 0;
  m_next = 0;
  
}

void LevelsSlider::onPointerUp(int x, int y)
{
  if (!m_moving)
  {
    m_curDelta = 0;
    m_pages[m_current]->onPointerUp(0, x, y);
  }
  else
  {
    if (iabs(m_curDelta) < 75)
    {
      m_moveDelta = -m_curDelta;
    }
    else
    {
      if (m_curDelta > 0)
      {
        m_next = 1;
        m_moveDelta = WIDTH - m_curDelta;
      }
      else
      {
        m_next = -1;
        m_moveDelta = -WIDTH - m_curDelta;
      }
    }

    m_timer.start(350);
  }

  m_pressed = false;
  m_moving = false;
  m_startPos = 0;
}

void LevelsSlider::onPointerMove(int x, int y)
{
  m_curDelta = m_startPos - x;

  if ((m_curDelta < 0 && !m_current) || (m_curDelta > 0 && m_current == (COUNT - 1)))
    m_curDelta = 0;

  if (m_moving)
  {
    updatePos(m_curDelta);
  }
  else if (iabs(m_curDelta) > 5)
  {
    m_pages[m_current]->onPointerUp(0, 0, 0);
    m_moving = true;
  }
  else
  {
    m_pages[m_current]->onPointerMove(0, x, y);
  }
}

void LevelsSlider::onFocus(bool focus)
{
  if (!focus)
    onPointerUp(0, 0);
}
