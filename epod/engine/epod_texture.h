#ifndef _EPOD_TEXTURE_H_
#define _EPOD_TEXTURE_H_

#include "epod_interface.h"
#include <string>


namespace ep
{

class Texture
  :public ITexture
{
public:
  Texture();
  Texture(const char *filename);
  Texture(int width, int height, bool with_alpha);
  virtual ~Texture();

  bool loadFromFile(const char *filename);

  virtual bool load();
  virtual void free();

  virtual int getId() const { return m_id; }
  virtual int getWidth() const { return m_width; }
  virtual int getHeight() const { return m_height; }

  virtual float getInvWidth() const { return m_invWidth; }
  virtual float getInvHeight() const { return m_invHeight; }

protected:
  bool loadFromPvr(const char *filename);

  int m_id;
  int m_width, m_height;
  float m_invWidth, m_invHeight;

  std::string m_fileName;
  int m_counter;
};

} // namespace ep

#endif //_EPOD_TEXTURE_H_
