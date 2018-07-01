#include "epod.h"
#include "epod_texture.h"
#include "epod_fsystem.h"

#include <image/pvr.h>
#include <string.h>
#include <gles/gl.h>
#include <assert.h>


static int get_bigger_pow2(int val)
{
  if (val < 0)
    return 1;
  int num = 1;
  while (num < val)
    num <<= 1;
  return num;
}


namespace ep
{

Texture::Texture()
  :m_id(0), m_width(0), m_height(0), m_counter(0)
{
}

Texture::Texture(const char *filename)
  :m_id(0), m_width(0), m_height(0), m_counter(0), m_fileName(filename)
{
}

Texture::Texture(int width, int height, bool with_alpha)
  :m_id(0), m_width(0), m_height(0), m_counter(0)
{
  m_width = ::get_bigger_pow2(width);
  m_height = ::get_bigger_pow2(height);
  m_invWidth = 1.f / m_width;
  m_invHeight = 1.f / m_height;

  glGenTextures(1, (GLuint*)&m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (with_alpha)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

  m_fileName = "generic texture";
  m_counter = 1;
}

Texture::~Texture()
{
  free();
}


bool Texture::loadFromFile(const char *filename)
{
  free();

  // texture's been loaded already
  if (m_counter > 0)
  {
    if (m_fileName == filename)
    {
      m_counter++;
      return true;
    }
    else
    {
      logError("Can't load another texture (%s). Current texture in use (%s) with counter = %d",
        filename, m_fileName.c_str(), m_counter);
      return false;
    }
  }

  m_fileName = filename;

  glGenTextures(1, (GLuint*)&m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  const char *ext = strrchr(filename, '.');
  if (!strcmp(ext, ".pvr"))
  {
    if (!loadFromPvr(filename))
      return false;
  }
  else
  {
    logError("Unknown texture format (%s). Failed to load texture: %s", ext, filename);
    return false;
  }

  m_counter++;
  log("Texture loaded (%dx%d): %s", m_width, m_height, filename);

  return true;
}


bool Texture::load()
{
  if (m_fileName.empty())
  {
    logError("Can't load texture. Filename is empty!");
    return false;
  }

  return loadFromFile(m_fileName.c_str());
}


void Texture::free()
{
  if (!m_id)
    return;

  m_counter--;
  assert(m_counter >= 0 && "Texture counter should always be >= 0");
  if (m_counter > 0)
    return;

  glDeleteTextures(1, (GLuint*)&m_id);
  m_id = 0;
  m_width = m_height = 0;

  log("Texture deleted: %s", m_fileName.c_str());
}


bool Texture::loadFromPvr(const char *filename)
{
  File texfile;
  EPFILE *ftex = texfile.open(filename, FILE_MODE_READ);
  if (!ftex)
  {
    logError("Failed to load file (%s)", filename);
    return false;
  }

  PVR_Texture_Header *tex = (PVR_Texture_Header *)texfile.readEntire();

  if (PVRTTextureLoadFromPointer(tex) != PVR_SUCCESS)
  {
    logError("Failed to load texture (%s)", filename);
    return false;
  }

  m_width = tex->dwWidth;
  m_height = tex->dwHeight;

  m_invWidth = 1.f / m_width;
  m_invHeight = 1.f / m_height;
  return true;
}


} // namespace ep
