#ifndef _EPOD_INTERFACE_H_
#define _EPOD_INTERFACE_H_

#include <array_ptr.h>
#include <math/rect2.h>

///
/// Main EPod interfaces
///

namespace ep
{

class ITexture;
class IGraphics;


enum EBlendMode
{
  BLEND_ALPHA = 1,
  BLEND_ADD = 2,

  BLEND_DEFAULT = BLEND_ALPHA,
};

enum ETrDrawMode
{
  TDM_COMMON = 1,
  TDM_STRIP = 2,
};

enum EAlign
{
  A_LEFT = 0x01,
  A_RIGHT = 0x02,
  A_TOP = 0x04,
  A_BOTTOM = 0x08,
  A_HCENTER = 0x10,
  A_VCENTER = 0x20,

  A_LEFT_TOP = A_LEFT | A_TOP,
  A_LEFT_BOTTOM = A_LEFT | A_BOTTOM,
  A_LCENTER = A_LEFT | A_VCENTER,
  A_RCENTER = A_RIGHT | A_VCENTER,
  A_CENTER = A_HCENTER | A_VCENTER,
};

enum EColors
{
  C_WHITE = 0xFFFFFFFF,
  C_BLACK = 0xFF000000,
  C_RED = 0xFF880000,
  C_GREEN = 0xFF008800,
  C_BLUE = 0xFF000088,
  C_YELLOW = 0xFF888800,
};


struct Vertex
{
  float x, y;
  float u, v;
};


struct Tile
{
  Tile()
    :texture(0), width(0), height(0), u0(0.f), v0(0.f), u1(0.f), v1(0.f)
    ,xo(0), yo(0), wr(0), hr(0)
  {
  }

  Tile(ITexture *tex, short w, short h, float tu0, float tv0, float tu1, float tv1,
    short xoff, short yoff, short wrend, short hrend)
    :texture(tex), width(w), height(h), u0(tu0), v0(tv0), u1(tu1), v1(tv1)
    ,xo(xoff), yo(yoff), wr(wrend), hr(hrend)
  {
  }

  void set(ITexture *tex, short w, short h, float tu0, float tv0, float tu1, float tv1,
    short xoff, short yoff, short wrend, short hrend)
  {
    texture = tex;
    width = w; height = h;
    u0 = tu0; v0 = tv0;
    u1 = tu1; v1 = tv1;
    xo = xoff; yo = yoff;
    wr = wrend; hr = hrend;
  }

  short width, height; // full tile size
  short xo, yo;        // render tile offset in full tile size
  short wr, hr;        // render size (size in atlas)

  ITexture *texture;
  float u0, v0;
  float u1, v1;
};


class ITexture
{
public:
  virtual ~ITexture() {}

  virtual bool load() = 0;
  virtual void free() = 0;

  virtual int getId() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;

  virtual float getInvWidth() const = 0;
  virtual float getInvHeight() const = 0;
};


/// view orientation
/// rotation view in window
enum
{
  VIEW_ORIENT_NORMAL = 1,
  VIEW_ORIENT_DOWN = 2,
  VIEW_ORIENT_LEFT = 4,
  VIEW_ORIENT_RIGHT = 8,
};


class IGraphics
{
public:
  virtual ~IGraphics() {}

  virtual void beginRender(int width, int height, int orient) = 0;
  virtual void endRender() = 0;

  virtual void clear(int color = C_BLACK) = 0;

  virtual void drawFillRect(float x, float y, float w, float h, int color) = 0;
  virtual void drawGradientRect(float x, float y, float w, float h, int c1, int c2, int c3, int c4) = 0;
  virtual void drawLine(float x1, float y1, float x2, float y2, int color) = 0;
  virtual void drawTile(float x, float y, const Tile *tile, int color = C_WHITE, int align = A_LEFT_TOP) = 0;

  virtual void drawTexture(float x, float y, ITexture *texture,
    float tx, float ty, float tw, float th, int color = C_WHITE) = 0;

  virtual void drawTriangles(int color, const ITexture *texture,
    TArrayPtr<Vertex> verts, ETrDrawMode mode) = 0;

  virtual void setBlendMode(EBlendMode mode) = 0;
  virtual Rect2i setClip(const Rect2i &clip) = 0;

  virtual void translatePoint(int x, int y, int &out_x, int &out_y) = 0;

  virtual void wtmIdent() = 0;
  virtual void wtmTrans(float px, float py) = 0;
  virtual void wtmScale(float px, float py, float sx, float sy) = 0;

  virtual void accountStats(int &tiles, int &draw_calls) = 0;
};


} // namespace ep

#endif //_EPOD_INTERFACE_H_
