#ifndef _EPOD_SPRITE_H_
#define _EPOD_SPRITE_H_

#include <math/point2.h>
#include "epod_interface.h"


namespace ep
{

class Sprite
{
public:
  Sprite(Tile *tile);

  void draw(IGraphics *graphics);

  void scale(float x, float y);
  void dir(const Point2f &dir);
  void pos(const Point2f &pos);

  const Point2f &getPos() const { return m_pos; }

  void setColor(int color) { m_color = color; }

protected:
  void updateVerts();

  Tile *m_tile;

  Point2f m_pos;
  Point2f m_scale;
  Point2f m_dir;

  int m_color;

  Vertex m_verts[4];
};

} // namespace ep

#endif //_EPOD_SPRITE_H_
 