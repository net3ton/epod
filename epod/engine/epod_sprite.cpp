#include "epod_sprite.h"


namespace ep
{

Sprite::Sprite(Tile *tile)
  :m_tile(tile)
  ,m_pos(0.f, 0.f), m_scale(1.f, 1.f), m_dir(0.f, -1.f)
{
  m_verts[0].u = tile->u0;
  m_verts[0].v = 1.f - tile->v0;

  m_verts[1].u = tile->u0;
  m_verts[1].v = 1.f - tile->v1;

  m_verts[2].u = tile->u1;
  m_verts[2].v = 1.f - tile->v0;

  m_verts[3].u = tile->u1;
  m_verts[3].v = 1.f - tile->v1;

  updateVerts();
}


void Sprite::draw(IGraphics *graphics)
{
  graphics->drawTriangles(m_color, m_tile->texture, ArrayPtr(4, m_verts), TDM_STRIP);
}


void Sprite::scale(float x, float y)
{
  m_scale.set(x, y);
  updateVerts();
}


void Sprite::dir(const Point2f &dir)
{
  m_dir = dir;
  updateVerts();
}


void Sprite::updateVerts()
{
  float cosVal = m_dir.x;
  float sinVal = m_dir.y;

  float w2 = (m_tile->width / 2.f) * m_scale.x;
  float h2 = (m_tile->height / 2.f) * m_scale.y;

  m_verts[0].x = m_pos.x + w2 * cosVal + h2 * sinVal;
  m_verts[0].y = m_pos.y + w2 * sinVal - h2 * cosVal;

  m_verts[1].x = m_pos.x - w2 * cosVal + h2 * sinVal;
  m_verts[1].y = m_pos.y - w2 * sinVal - h2 * cosVal;

  m_verts[2].x = m_pos.x + w2 * cosVal - h2 * sinVal;
  m_verts[2].y = m_pos.y + w2 * sinVal + h2 * cosVal;

  m_verts[3].x = m_pos.x - w2 * cosVal - h2 * sinVal;
  m_verts[3].y = m_pos.y - w2 * sinVal + h2 * cosVal;
}


void Sprite::pos(const Point2f &pos)
{
  Point2f delta = pos - m_pos;
  for (int i=0; i<4; ++i)
  {
    m_verts[i].x += delta.x;
    m_verts[i].y += delta.y;
  }
  m_pos = pos;
}


} // namespace ep
