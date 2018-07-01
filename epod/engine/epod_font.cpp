#include "epod.h"
#include "epod_font.h"
#include "epod_fsystem.h"

#include <utf8_utils.h>

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>


namespace ep
{

Font::Font(const char *filename, ITexture *texture)
  :m_fileName(filename), m_height(0), m_texture(texture)
{
}

Font::~Font()
{
  free();
}


bool Font::load()
{
  if (m_fileName.empty())
  {
    logError("[Font] Can't load. Filename is empty!");
    return false;
  }

  if (!m_texture->load())
    return false;

  return loadFromFile(m_fileName.c_str());
}


void Font::free()
{
  m_texture->free();
}


#pragma warning(push)
#pragma warning(disable: 4996) // 'sscanf' was declared deprecated (in vs) - but it's crossplatform!

bool Font::loadFromFile(const char *filename)
{
  m_map.clear();
  m_glyphs.clear();

  File fontFile(filename, FILE_MODE_READ);
  if (!fontFile.opened())
  {
    logError("Failed to load file (%s)", filename);
    return false;
  }

  // skip the first info line
  fontFile.readLine();

  int pagesCount = 0;
  const char *line = NULL;

  // read some basic attributes
  line = fontFile.readLine();
  if (!line || sscanf(line, "common lineHeight=%d base=%*d scaleW=%*d scaleH=%*d pages=%d\n",
        &m_height, &pagesCount) != 2)
    return false;

  assert(pagesCount == 1 && "Only one texture per font is supported!");

  // skip the line with texture name - not needed - texture mast be passed in ctr()
  fontFile.readLine();

  int glyphsCount = 0;

  // glyphs count
  line = fontFile.readLine();
  if (!line || sscanf(line, "chars count=%d\n", &glyphsCount) != 1) 
    return false;

  // read glyphs
  m_glyphs.resize(glyphsCount);
  for (int i=0; i<glyphsCount; ++i)
  {
    Glyph &glyph = m_glyphs[i];
    unsigned charCode = 0;
    unsigned charx = 0;
    unsigned chary = 0;

    line = fontFile.readLine();
    if (!line || sscanf(line, "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d\n",
        &charCode,
        &charx,
        &chary,
        &glyph.width,
        &glyph.height,
        &glyph.xoffset,
        &glyph.yoffset,
        &glyph.xadvance) != 8)
      return false;

    glyph.u0 = charx * m_texture->getInvWidth();
    glyph.v0 = 1.f - chary * m_texture->getInvHeight();
    glyph.u1 = (charx + glyph.width) * m_texture->getInvWidth();
    glyph.v1 = 1.f - (chary + glyph.height) * m_texture->getInvHeight();

    m_map.insert(TGlyphMap::value_type(charCode, &glyph));
  }

  log("Font loaded (height: %d, glyphs: %d): %s", m_height, glyphsCount, filename);

  return true;
}

#pragma warning(pop)


const Glyph *Font::getGlyph(unsigned code) const
{
  TGlyphMap::const_iterator ig = m_map.find(code);
  if (ig == m_map.end())
    return NULL;

  return (*ig).second;
}


float Font::getWidth(const char *utf8str) const
{
  if (!utf8str || !*utf8str)
    return 0.f;

  float width = 0.f;

  const char *utf8str_end = utf8str + strlen(utf8str);
  while (utf8str < utf8str_end)
  {
    unsigned code = UTF8ToCode(&utf8str, utf8str_end);
    const Glyph *glyph = getGlyph(code);
    if (glyph)
      width += glyph->xadvance;
  }

  return width;
}


int Font::getHeight() const
{
  return m_height;
}


///////////////////////////////////////////////////////////////////////////////

TextRenderer::TextRenderer()
  :m_width(0), m_maxWidth(0), m_linesCount(0)
  ,m_font(0), m_color(C_WHITE), m_align(A_LEFT_TOP)
{
}

TextRenderer::TextRenderer(Font *font)
  :m_width(0), m_maxWidth(0), m_linesCount(0)
  ,m_font(font), m_color(C_WHITE), m_align(A_LEFT_TOP)
{
}

TextRenderer::TextRenderer(Font *font, const char *utf8str)
  :m_width(0), m_maxWidth(0), m_linesCount(0)
  ,m_font(font), m_color(C_WHITE), m_align(A_LEFT_TOP)
{
  setStr(utf8str);
}

void TextRenderer::printfa(const char *fmt, va_list args)
{
  if (!fmt || !*fmt)
    return;

  const int SIZE = 512;
  char buf[SIZE];
  buf[0] = 0;

  vsnprintf(buf, SIZE, fmt, args);
  buf[SIZE-1] = 0;

  setStr(buf);
}

void TextRenderer::printf(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  printfa(fmt, ap);

  va_end(ap);
}

void TextRenderer::setStr(const char *utf8str)
{
  if (!m_font)
  {
    ep::logError("[TextRenderer] setting string without a specified font!");
    return;
  }

  if (!m_font->isLoaded())
    m_font->load();

  int maxLen = strlen(utf8str);
  m_verts.resize(maxLen * 6);

  float x = m_pos.x;
  float y = m_pos.y;

  m_linesCount = 0;
  int charsOnLine = 0;
  int skipFirst = 0;

  const char *utf8str_end = utf8str + maxLen;
  int ind = 0;
  while (utf8str < utf8str_end)
  {
    if (m_maxWidth && !charsOnLine)
    {
      charsOnLine = split_line(utf8str, *m_font, m_maxWidth, skipFirst);
      //ep::log("[d] * [TextRenderer] ********* [%d] %d", skipFirst, charsOnLine);
    }

    unsigned code = UTF8ToCode(&utf8str, utf8str_end);

    if (skipFirst > 0)
    {
      skipFirst--;
      continue;
    }

    const Glyph *glyph = m_font->getGlyph(code);
    if (glyph)
    {
      float posx = x + glyph->xoffset;
      float posy = y + glyph->yoffset + m_linesCount * m_font->getHeight();

      Vertex &vert0 = m_verts[ind + 0];
      vert0.x = posx;
      vert0.y = posy;
      vert0.u = glyph->u0;
      vert0.v = glyph->v0;

      Vertex &vert1 = m_verts[ind + 1];
      vert1.x = posx;
      vert1.y = posy + glyph->height;
      vert1.u = glyph->u0;
      vert1.v = glyph->v1;

      Vertex &vert2 = m_verts[ind + 2];
      vert2.x = posx + glyph->width;
      vert2.y = posy;
      vert2.u = glyph->u1;
      vert2.v = glyph->v0;

      memcpy(&m_verts[ind + 3], &vert1, sizeof(Vertex));
      memcpy(&m_verts[ind + 4], &vert2, sizeof(Vertex));

      Vertex &vert5 = m_verts[ind + 5];
      vert5.x = posx + glyph->width;
      vert5.y = posy + glyph->height;
      vert5.u = glyph->u1;
      vert5.v = glyph->v1;

      ind += 6;
      x += glyph->xadvance;
    }

    if (charsOnLine > 0)
    {
      charsOnLine--;

      if (!charsOnLine)
      {
        m_linesCount++;
        x = m_pos.x;
      }
    }
  }

  m_linesCount = m_maxWidth ? m_linesCount : 1;
  m_width = m_maxWidth ? m_maxWidth : (x - m_pos.x);
  m_verts.resize(ind);

  /////////////////////////////////////////////////////////////////////////////

  // horz align shift
  int hdelta = 0;
  if (m_align & A_RIGHT)
    hdelta = getWidth();
  else if (m_align & A_HCENTER)
    hdelta = getWidth() / 2;

  // vert align shift
  int vdelta = 0;
  if (m_align & A_BOTTOM)
    vdelta = getHeight();
  else if (m_align & A_VCENTER)
    vdelta = getHeight() / 2;

  if (hdelta || vdelta)
  {
    for (size_t i=0; i<m_verts.size(); ++i)
    {
      m_verts[i].x -= hdelta;
      m_verts[i].y -= vdelta;
    }
  }
}


int TextRenderer::getWidth() const
{
  return m_width;
}

int TextRenderer::getHeight() const
{
  return m_font ? m_linesCount * m_font->getHeight() : 0;
}


void TextRenderer::setPos(float x, float y)
{
  Point2f delta(x - m_pos.x, y - m_pos.y);
  if (delta.isZero())
    return;

  for (size_t i=0 ; i<m_verts.size() ; ++i)
  {
    m_verts[i].x += delta.x;
    m_verts[i].y += delta.y;
  }

  m_pos.set(x, y);
}


void TextRenderer::setAlign(int align)
{
  if (m_align == align)
    return;

  if (m_verts.empty() || !m_font)
  {
    m_align = align;
    return;
  }

  int h = getHeight();
  int w = getWidth();

  int xdelta = 0;
  if (m_align & A_RIGHT)
    xdelta = w;
  else if (m_align & A_HCENTER)
    xdelta = w / 2;

  if (align & A_RIGHT)
    xdelta -= w;
  else if (align & A_HCENTER)
    xdelta -= w / 2;

  int ydelta = 0;
  if (m_align & A_BOTTOM)
    ydelta = h;
  else if (m_align & A_VCENTER)
    ydelta = h / 2;

  if (align & A_BOTTOM)
    ydelta -= h;
  else if (align & A_VCENTER)
    ydelta -= h / 2;

  m_align = align;

  for (size_t i=0; i<m_verts.size(); ++i)
  {
    m_verts[i].x += xdelta;
    m_verts[i].y += ydelta;
  }
}


void TextRenderer::setFont(Font *font)
{
  if (!m_verts.empty())
    ep::logError("[TextRenderer] setting new font for an existing string!");

  m_font = font;
  m_width = 0;
  m_verts.clear();
}


void TextRenderer::clear()
{
  m_verts.clear();
}


void TextRenderer::draw(IGraphics *graphics)
{
  if (m_verts.empty())
    return;

  graphics->drawTriangles(m_color, m_font->getTexture(), ArrayPtr(m_verts), TDM_COMMON);

  // debug
  /*
  int h = getHeight();
  int w = getWidth();

  float x = m_pos.x;
  float y = m_pos.y;

  // vert align shift
  if (m_align & A_BOTTOM)
    y -= getHeight();
  else if (m_align & A_VCENTER)
    y -= getHeight() / 2;

  // horz align shift
  if (m_align & A_RIGHT)
    x -= getWidth();
  else if (m_align & A_HCENTER)
    x -= getWidth() / 2;

  graphics->drawLine(x - 5, y, x + w + 5, y, -1);
  graphics->drawLine(x, y - 5, x, y + h + 5, -1);
  graphics->drawLine(x - 5, y + h, x + w + 5, y + h, -1);
  graphics->drawLine(x + w, y - 5, x + w, y + h + 5, -1);
  */
}


///////////////////////////////////////////////////////////////////////////////


int split_line(const char *utf8str, const Font &font, int width, int &skip_first)
{
  if (!utf8str || !*utf8str || width <= 0)
    return 0;

  const unsigned NEWLINE = '\n';
  const unsigned TRANSFER = '\t';
  const unsigned SPACE = ' ';

  int lineWidth = 0;

  int maxLen = strlen(utf8str);
  const char *utf8str_end = utf8str + maxLen;

  int ind = 0;
  int charsCount = 0;

  skip_first = 0;

  while (utf8str < utf8str_end)
  {
    unsigned code = UTF8ToCode(&utf8str, utf8str_end);

    /// skip spaces on line start
    if (!ind)
    {
      if (code == SPACE || code == TRANSFER)
      {
        skip_first++;
        continue;
      }
    }

    ///////////////////////////////////////////////////////////////////////////

    bool toCut = false;

    if (code == SPACE)
      charsCount = ind + 1;

    if (code == TRANSFER)
      charsCount = ind + 1;

    if (code == NEWLINE)
    {
      toCut = true;
      charsCount = ind + 1;
    }
    else
    {
      const Glyph *glyph = font.getGlyph(code);
      if (glyph)
      {
        lineWidth += glyph->xadvance;
        if (lineWidth >= width && ind)
        {
          toCut = true;
          //charsCount = ind;
        }
      }
    }

    if (toCut)
    {
      if (!charsCount)
        charsCount = ind;

      return charsCount;
    }

    ind++;
  }

  return ind;
}


} // namespace ep
