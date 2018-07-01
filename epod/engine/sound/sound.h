#ifndef __EPOD_SOUND_SOUND_H__
#define __EPOD_SOUND_SOUND_H__

#include "interface.h"
#include <string>


namespace ep
{

class Sound
  :public ISound
{
public:
  Sound(const char *filename, bool looped);
  virtual ~Sound();

  bool loadFromFile(const char *filename);

  virtual bool load();
  virtual void free();

  virtual void play(bool restart = true);
  virtual void pause();
  virtual void stop();

  virtual void setVolume(float volume);
  virtual float getVolume() { return m_volume; }

protected:
  bool loadFromOgg(const char *filename);

  int m_id;
  std::string m_fileName;
  bool m_looped;
  float m_volume;
};

} // namespace ep

#endif //__EPOD_SOUND_SOUND_H__
