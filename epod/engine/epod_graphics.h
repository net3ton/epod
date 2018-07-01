#ifndef _EPOD_GRAPHICS_H_
#define _EPOD_GRAPHICS_H_

#include "epod_interface.h"


namespace ep
{

class Graphics
  :public IGraphics
{
public:
  Graphics();
  virtual ~Graphics() {}

  virtual void beginRender(int width, int height, int orient);
  virtual void endRender();

  virtual void clear(int color);

  virtual void drawFillRect(float x, float y, float w, float h, int color);
  virtual void drawGradientRect(float x, float y, float w, float h, int c1, int c2, int c3, int c4);
  virtual void drawLine(float x1, float y1, float x2, float y2, int color);
  virtual void drawTile(float x, float y, const Tile *tile, int color, int align);

  virtual void drawTexture(float x, float y, ITexture *texture,
    float tx, float ty, float tw, float th, int color);

  virtual void drawTriangles(int color, const ITexture *texture,
    TArrayPtr<Vertex> verts, ETrDrawMode mode);

  virtual void setBlendMode(EBlendMode mode);
  virtual Rect2i setClip(const Rect2i &clip);

  virtual void translatePoint(int x, int y, int &out_x, int &out_y);

  virtual void wtmIdent();
  virtual void wtmTrans(float px, float py);
  virtual void wtmScale(float px, float py, float sx, float sy);

  virtual void accountStats(int &tiles, int &draw_calls);

private:
  void setColor(int color);
  void setTexture(int id);
  void setViewport(const Rect2i &view);

  Rect2i translateRect(const Rect2i &r);
  void translateSize(int width, int height, int &out_width, int &out_height);

  int m_color;
  int m_texture;
  EBlendMode m_blendMode;

  int m_width, m_height;
  int m_orient;
  Rect2i m_view;
  Rect2i m_clip;

  /// statistics
  int m_statsTiles, m_statsCurTiles;
  int m_statsDrawCalls, m_statsCurDrawCalls;
};

} // namespace ep

#endif //_EPOD_GRAPHICS_H_
