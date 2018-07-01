#include "blockEnemy.h"
#include "../enemies.h"
#include "../res.h"

#include <epod.h>
#include <epod_graphics.h>


BlockEnemy::BlockEnemy()
  :m_enemyId(0)
  ,m_enemy(0)
{
  m_textId.setFont(&::res.mFonts.main);
  m_textId.setAlign(ep::A_CENTER);
  m_textId.setColor(0x80FFFFFF);
}


BlockEnemy::~BlockEnemy()
{
  if (m_enemy)
  {
    ::destroy_enemy(m_enemy);
    m_enemy = 0;
  }

  for (int i=0; i<m_blocks.size(); ++i)
    ::destroy_block(m_blocks[i]);
}


void BlockEnemy::init(int x, int y)
{
  BlockSimple::init(x, y);
  m_textId.setPos(x + 8, y + 8);
}


void BlockEnemy::render(ep::IGraphics *graph)
{
  for (int i=0; i<m_blocks.size(); ++i)
    m_blocks[i]->render(graph);

  BlockSimple::render(graph);

  m_textId.draw(graph);
}


void BlockEnemy::setEnemyBlockId(int id)
{
  setId(id);

  m_enemyId = get_enemy_id(id);
  m_textId.printf("%d", m_enemyId);
}


void BlockEnemy::loadEnemy()
{
  m_enemy = ::put_enemy(m_enemyId, m_bbox.left(), m_bbox.top(), m_blocks);
}
