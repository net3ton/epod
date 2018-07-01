#ifndef __EPOD_SOUND_MUSIC_H__
#define __EPOD_SOUND_MUSIC_H__

#include "channel.h"
#include "interface.h"
#include <string>
#include <tremor/ivorbisfile.h>


namespace ep
{

class Music
  :public ISound
{
public:
  Music(const char *filename);
  virtual ~Music();

  virtual bool load();
  virtual void free();

  virtual void play(bool restart = false);
  virtual void pause();
  virtual void stop();

  virtual void setVolume(float volume);
  virtual float getVolume() { return m_volume; }

  void setPos(float sec);
  float getPos();

protected:
  static void *threadProc(void *val);
  void backgroudProc();

  void queueData(int id, char *data, int size);

  enum { QUEUE_LEN = 3 };
  int m_ids[QUEUE_LEN];

  std::string m_fileName;
  float m_volume;

  Channel m_channel;
  void *m_thread;

  bool m_playing;
  float m_startPos;

  OggVorbis_File m_ogg;
  ogg_int64_t m_time, m_timeTotal;
  int m_oggFormat, m_oggFreq;
};

} // namespace ep

#endif //__EPOD_SOUND_MUSIC_H__
