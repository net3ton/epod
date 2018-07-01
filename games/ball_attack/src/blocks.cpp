#include "blocks.h"
#include "blocks/simple.h"
#include "blocks/explosion.h"
#include "blocks/double.h"
#include "blocks/magnet.h"
#include "blocks/dynamic.h"
#include "res.h"

#include <epod.h>


static IBlockBase *make_a_block(int type)
{
  if (type == BLOCK_NONE)
    return NULL;
  if (type == BLOCK_SIMPLE)
    return new BlockSimple();
  if (type == BLOCK_STATIC)
    return new BlockStatic();
  if (type == BLOCK_EXPLO)
    return new BlockExplo();
  if (type == BLOCK_BONUS)
    return new BlockBonus();
  if (type == BLOCK_DOUBLE)
    return new BlockDouble();
  if (type == BLOCK_TRIPLE)
    return new BlockTriple();
  if (type == BLOCK_MAGNET_IN)
    return new BlockMagnetIn();
  if (type == BLOCK_MAGNET_OUT)
    return new BlockMagnetOut();
  if (type == BLOCK_DYNAMIC)
    return new BlockDynamic();

  ep::logError("Unknown block type (%d) creation!", type);
  return NULL;
}


void destroy_block(IBlockBase *block)
{
  delete block;
}


IBlockBase *create_block(int type)
{
  int id = type & 0xFF;
  int back = (type >> 8) & 0xFF;

  IBlockBase *block = make_a_block(id);
  if (block)
  {
    block->setId(type);
    block->setBack(back);
  }

  return block;
}


IBlockBase *put_block(int type, int x, int y)
{
  IBlockBase *block = create_block(type);
  if (block)
    block->init(x, y);

  return block;
}


///////////////////////////////////////////////////////////////////////////////


static const ep::Tile *get_base_tile(int type)
{
  if (type == PANEL_A)
    return &::res.mTiles.panel_a;
  if (type == PANEL_B)
    return &::res.mTiles.panel_b;
  if (type == PANEL_C)
    return &::res.mTiles.panel_c;
  if (type == PANEL_D)
    return &::res.mTiles.panel_d;
  if (type == PANEL_E)
    return &::res.mTiles.panel_e;

  if (type == PANEL_ROUNDED_A)
    return &::res.mTiles.panel_round_a;
  if (type == PANEL_ROUNDED_B)
    return &::res.mTiles.panel_round_b;
  if (type == PANEL_ROUNDED_C)
    return &::res.mTiles.panel_round_c;
  if (type == PANEL_ROUNDED_D)
    return &::res.mTiles.panel_round_d;
  if (type == PANEL_ROUNDED_E)
    return &::res.mTiles.panel_round_e;

  return NULL;
}


IBlockBase::IBlockBase()
  :m_id(BLOCK_NONE)
  ,m_baseTile(&::res.mTiles.panel_a)
{
}


void IBlockBase::setBack(int type)
{
  m_baseTile = get_base_tile(type);
}
