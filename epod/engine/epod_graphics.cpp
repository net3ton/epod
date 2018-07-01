#include "epod.h"
#include "epod_graphics.h"
#include "epod_texture.h"

#include <gles/gl.h>
#include <assert.h>


static void get_color_values(int color, float &a, float &r, float &g, float &b)
{
  a = ((color >> 24) & 0x000000FF) / 255.f;
  r = ((color >> 16) & 0x000000FF) / 255.f;
  g = ((color >> 8) & 0x000000FF) / 255.f;
  b = (color & 0x000000FF) / 255.f;
}


namespace ep
{

Graphics::Graphics()
  :m_color(0), m_texture(0)
  ,m_width(0), m_height(0), m_orient(VIEW_ORIENT_NORMAL)
  ,m_statsTiles(0), m_statsCurTiles(0), m_statsDrawCalls(0), m_statsCurDrawCalls(0)
{
}

void Graphics::beginRender(int width, int height, int orient)
{
  m_orient = orient;
  translateSize(width, height, m_width, m_height);

  /////////////////////////////////////////////////////////////////////////////

  /// Setup viewport
  Rect2i view(0, 0, width, height);
  setViewport(view);
  setClip(view);

  /// Setup ortho projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (orient == VIEW_ORIENT_DOWN)
    glRotatef(180.f, 0.f, 0.f, 1.f);
  else if (orient == VIEW_ORIENT_RIGHT)
    glRotatef(-90.f, 0.f, 0.f, 1.f);
  else if (orient == VIEW_ORIENT_LEFT)
    glRotatef(90.f, 0.f, 0.f, 1.f);

  glOrthof(0.f, (float)width, (float)height, 0.f, -1.f, 1.f);

  /// Setup states
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_DITHER);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glEnable(GL_SCISSOR_TEST);

  glShadeModel(GL_SMOOTH);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  ///

  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  setColor(C_WHITE);
  setBlendMode(BLEND_DEFAULT);
  setTexture(0);
}

void Graphics::endRender()
{
  glFlush();

  m_statsTiles = m_statsCurTiles;
  m_statsDrawCalls = m_statsCurDrawCalls;

  m_statsCurTiles = 0;
  m_statsCurDrawCalls = 0;
}

void Graphics::clear(int color)
{
  float a, r, g, b;
  ::get_color_values(color, a, r, g, b);

  glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}


void Graphics::drawFillRect(float x, float y, float w, float h, int color)
{
  const GLfloat square[] = {
      x,     y,
      x,     y + h,
      x + w, y,
      x + w, y + h
    };

  setColor(color);
  setTexture(0);

  glVertexPointer(2, GL_FLOAT, 0, square);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}


void Graphics::drawGradientRect(float x, float y, float w, float h, int c1, int c2, int c3, int c4)
{
  const GLfloat square[] = {
      x,     y,
      x,     y + h,
      x + w, y,
      x + w, y + h
    };

  const GLint colors[] = {
      c1, c2, c3, c4
    };

  setTexture(0);

  glVertexPointer(2, GL_FLOAT, 0, square);
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}


void Graphics::drawLine(float x1, float y1, float x2, float y2, int color)
{
  const GLfloat line[] = {
      x1, y1,
      x2, y2
    };

  setColor(color);
  setTexture(0);

  glVertexPointer(2, GL_FLOAT, 0, line);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDrawArrays(GL_LINES, 0, 2);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}


void Graphics::drawTriangles(int color, const ITexture *texture,
    TArrayPtr<Vertex> verts, ETrDrawMode mode)
{
  if (!verts.size())
    return;

  setColor(color);
  setTexture(texture->getId());

  glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &verts.begin()->x);
  glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &verts.begin()->u);

  GLenum glmode = 0;
  if (mode == TDM_COMMON)
    glmode = GL_TRIANGLES;
  else if (mode == TDM_STRIP)
    glmode = GL_TRIANGLE_STRIP;

  assert(glmode && "unknown triangle draw mode!");

  glDrawArrays(glmode, 0, verts.size());

  m_statsCurDrawCalls++;
}


void Graphics::drawTile(float x, float y, const Tile *tile, int color, int align)
{
  if (!tile)
    return;

  assert(tile->texture && "Texture is not set for Tile!");

  if (!tile->texture->getId())
    tile->texture->load();

  assert(tile->texture->getId() && "Texture is not loaded for Tile!");

  if (align & A_RIGHT)
    x -= tile->width;
  else if (align & A_HCENTER)
    x -= tile->width / 2;

  if (align & A_BOTTOM)
    y -= tile->height;
  else if (align & A_VCENTER)
    y -= tile->height / 2;

  x += tile->xo;
  y += tile->yo;

  Vertex square[] =
  {
    {x,            y,            tile->u0, 1.f - tile->v0},
    {x,            y + tile->hr, tile->u0, 1.f - tile->v1},
    {x + tile->wr, y,            tile->u1, 1.f - tile->v0},
    {x + tile->wr, y + tile->hr, tile->u1, 1.f - tile->v1}
  };

  drawTriangles(color, tile->texture, ArrayPtr(4, square), TDM_STRIP);

  m_statsCurTiles++;
}


void Graphics::drawTexture(float x, float y, ITexture *texture,
  float tx, float ty, float tw, float th, int color)
{
  if (!texture->getId())
    texture->load();

  assert(texture->getId() && "Texture is not loaded!");

  float u0 = tx * texture->getInvWidth();
  float v0 = ty * texture->getInvHeight();
  float u1 = (tx + tw) * texture->getInvWidth();
  float v1 = (ty + th) * texture->getInvHeight();

  Vertex square[] =
  {
    {x,      y,      u0, 1.f - v0},
    {x,      y + th, u0, 1.f - v1},
    {x + tw, y,      u1, 1.f - v0},
    {x + tw, y + th, u1, 1.f - v1}
  };

  drawTriangles(color, texture, ArrayPtr(4, square), TDM_STRIP);
}


void Graphics::setColor(int color)
{
  if (color == m_color)
    return;

  float a, r, g, b;
  ::get_color_values(color, a, r, g, b);

  glColor4f(r, g, b, a);
  m_color = color;
}

void Graphics::setTexture(int id)
{
  if (id == m_texture)
    return;

  if (id)
  {
    if (!m_texture)
      glEnable(GL_TEXTURE_2D);

    m_texture = id;
    glBindTexture(GL_TEXTURE_2D, id);
  }
  else
  {
    if (m_texture)
    {
      m_texture = 0;
      glDisable(GL_TEXTURE_2D);
    }
  }
}

void Graphics::setBlendMode(EBlendMode mode)
{
  if (mode == m_blendMode)
    return;

  switch (mode)
  {
    case BLEND_ALPHA:
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;

    case BLEND_ADD:
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      break;

    default:
      return;
  }

  m_blendMode = mode;
}

void Graphics::translatePoint(int x, int y, int &out_x, int &out_y)
{
  switch(m_orient)
  {
    case VIEW_ORIENT_RIGHT:
      out_x = y;
      out_y = m_width - x;
      break;

    case VIEW_ORIENT_LEFT:
      out_x = m_height - y;
      out_y = x;
      break;

    case VIEW_ORIENT_DOWN:
      out_x = m_width - x;
      out_y = m_height - y;
      break;

    default:
      out_x = x;
      out_y = y;
      break;
  }
}

Rect2i Graphics::translateRect(const Rect2i &r)
{
  if (m_orient == VIEW_ORIENT_NORMAL)
    return Rect2i(r.x, m_height - r.bottom(), r.width, r.height);
  if (m_orient == VIEW_ORIENT_DOWN)
    return Rect2i(m_width - r.right(), r.y, r.width, r.height);
  if (m_orient == VIEW_ORIENT_RIGHT)
    return Rect2i(m_width - r.bottom(), m_height - r.right(), r.height, r.width);
  if (m_orient == VIEW_ORIENT_LEFT)
    return Rect2i(r.y, r.x, r.height, r.width);

  return r;
}

void Graphics::translateSize(int width, int height, int &out_width, int &out_height)
{
  if (m_orient == VIEW_ORIENT_LEFT || m_orient == VIEW_ORIENT_RIGHT)
  {
    out_width = height;
    out_height = width;
    return;
  }

  out_width = width;
  out_height = height;
}

void Graphics::setViewport(const Rect2i &view)
{
  Rect2i rect = translateRect(view);
  glViewport(rect.x, rect.y, rect.width, rect.height);
  m_view = rect;
}

Rect2i Graphics::setClip(const Rect2i &clip)
{
  Rect2i rect = translateRect(clip);
  glScissor(rect.x, rect.y, rect.width, rect.height);

  Rect2i prev(m_clip);
  m_clip = rect;
  return prev;
}


void Graphics::wtmIdent()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void Graphics::wtmTrans(float px, float py)
{
  GLfloat m[4][4];

  memset(m, 0, sizeof(m));
  m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;

  m[3][0] = px;
  m[3][1] = py;

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf((const GLfloat*)m);
}


void Graphics::wtmScale(float px, float py, float sx, float sy)
{
  GLfloat m[4][4];

  memset(m, 0, sizeof(m));
  m[0][0] = sx;
  m[1][1] = sy;
  m[2][2] = m[3][3] = 1.f;
  m[3][0] = px;
  m[3][1] = py;

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf((const GLfloat*)m);
}


void Graphics::accountStats(int &tiles, int &draw_calls)
{
  tiles += m_statsTiles;
  draw_calls += m_statsDrawCalls;
}


} // namespace ep
