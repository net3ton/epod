#ifndef _EPOD_FONT_H_
#define _EPOD_FONT_H_

#include "epod_interface.h"
#include <math/point2.h>
#include <string>
#include <vector>
#include <map>


namespace ep
{

// BMFont
struct Glyph
{
  float u0, v0;
  float u1, v1;

  int width;
  int height;
  int xoffset;
  int yoffset;
  int xadvance;
};


class Font
{
public:
  Font(const char *filename, ITexture *texture);
  virtual ~Font();

  bool load();
  void free();

  bool isLoaded() { return (!m_glyphs.empty() && m_texture->getId()); }

  float getWidth(const char *utf8str) const;
  int getHeight() const;

  const Glyph *getGlyph(unsigned code) const;
  ITexture *getTexture() const { return m_texture; }

protected:

  bool loadFromFile(const char *filename);

  int m_height;
  ITexture *m_texture;

  typedef std::map<unsigned, Glyph*> TGlyphMap;

  std::vector<Glyph> m_glyphs;
  TGlyphMap m_map;

  std::string m_fileName;
};


class TextRenderer
{
public:
  TextRenderer();
  TextRenderer(Font *font);
  TextRenderer(Font *font, const char *utf8str);

  void clear();

  void setStr(const char *utf8str);
  void setPos(float x, float y);
  void setFont(Font *font);
  void setAlign(int align);
  void setColor(int color) { m_color = color; }

  // set it before setting text if needed
  void setMaxWidth(int max_width) { m_maxWidth = max_width; }

  int getAlign() const { return m_align; }

  int getWidth() const;
  int getHeight() const;

  void printfa(const char *fmt, va_list args);
  void printf(const char *fmt, ...);

  void draw(IGraphics *graphics);

private:
  Point2f m_pos;
  int m_width;
  int m_maxWidth;
  int m_linesCount;
  int m_color;
  int m_align;
  Font *m_font;

  std::vector<Vertex> m_verts;
};


int split_line(const char *utf8str, const Font &font, int width, int &skip_first);


} // namespace ep

#endif //_EPOD_FONT_H_
