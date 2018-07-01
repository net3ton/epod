#include "enemyBase.h"
#include "blocks.h"
#include "levels.h"
#include "levelBase.h"

#include <epod.h>
#include <epod_graphics.h>
#include <epod_fsystem.h>


EnemyBase::EnemyBase()
{
}


void EnemyBase::init(int x, int y, Blocks &blocks)
{
  const char *name = getName();
  if (!name)
    return;

  load(name);

  Point2f curPos = m_bbox.center();
  Point2f delta(x - curPos.x + 8, y - curPos.y);
  move(delta);

  for (int i=0; i<m_blocks.size(); ++i)
    blocks.push_back(m_blocks[i]);
}


void EnemyBase::reset()
{
}


void EnemyBase::showAnim()
{
  for (int i=0; i<m_blocks.size(); ++i)
    m_blocks[i]->showAnim();
}


void EnemyBase::updateBbox()
{
  m_bbox.clear();
  for (int i=0; i<m_blocks.size(); ++i)
    m_bbox.add(m_blocks[i]->getBbox());
}


void EnemyBase::load(const char *id)
{
  ep::log("loading enemy '%s'", id);

  if (!m_blocks.empty())
  {
    ep::log("enemy '%s' is already loaded!", id);
    return;
  }

  char name[256];
  sprintf(name, "enemies/%s.enm", id);

  ep::File p(name, ep::FILE_MODE_READ);
  if (!p.opened())
  {
    ep::log("enemy loading failed!");
    return;
  }

  p.readInt(); // version

  int top = p.readInt();
  int left = p.readInt();
  int size = p.readInt();
  int h = p.readInt();
  int w = p.readInt();

  int count = h * w;
  for (int i=0; i<count; ++i)
  {
    int blockType = p.readInt();
    if (blockType == BLOCK_NONE)
      continue;

    int x = left + (i % w) * size;
    int y = top + (i / w) * size;

    IBlockBase *block = ::put_block(blockType, x, y);
    if (block)
      m_blocks.push_back(block);
  }

  updateBbox();
}


void EnemyBase::move(const Point2f &dir)
{
  m_bbox.move(dir);
  for (int i=0; i<m_blocks.size(); ++i)
    m_blocks[i]->move(dir);
}
