#include "music.h"
#include "system.h"
#include "internal.h"

#include "../epod.h"
#include "../epod_threads.h"

#include <assert.h>
#include <openal/al.h>
#include <tremor/ivorbiscodec.h>


namespace ep
{

Music::Music(const char *filename)
  :m_fileName(filename), m_volume(1.f), m_thread(NULL)
  ,m_playing(false)
  ,m_time(0), m_timeTotal(0), m_startPos(0.f)
  ,m_oggFormat(0), m_oggFreq(0)
{
  memset(m_ids, 0, sizeof(m_ids));
  memset(&m_ogg, 0, sizeof(m_ogg));
}


Music::~Music()
{
  free();
}


bool Music::load()
{
  if (m_fileName.empty())
  {
    logError("Can't load music. Filename is empty!");
    return false;
  }

  free();

  if (!snd::open_ogg(m_fileName.c_str(), m_ogg))
    return false;

  m_time = 0;
  m_timeTotal = ov_time_total(&m_ogg, -1);
  assert(m_timeTotal && "music total time is 0.0");

  vorbis_info *info = ov_info(&m_ogg, -1);
  m_oggFormat = (info->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
  m_oggFreq = info->rate;

  log("Music loaded: '%s' (freq: %d, channels: %d, bitrate: %d)", m_fileName.c_str(),
    info->rate, info->channels, info->bitrate_nominal);

  return true;
}


void Music::free()
{
  if (!m_ogg.datasource)
    return;

  stop();

  snd::close_ogg(m_ogg);

  m_time = m_timeTotal = 0;
  m_oggFormat = 0;
  m_oggFreq = 0;
  m_startPos = 0.f;

  log("Music deleted: %s", m_fileName.c_str());
}


void Music::play(bool restart)
{
  if (m_thread)
  {
    if (restart)
      setPos(0.f);

    if (!m_channel.isPlaying())
      m_channel.play();
    return;
  }

  if (!m_ogg.datasource && !load())
    return;

  if (restart)
    setPos(0.f);
  else
    setPos(m_startPos);

  m_channel.init();

  m_thread = ep::create_thread(&threadProc, this);
  assert(m_thread && "music thread not started!");

  snd::account_music(this, true);
}


void Music::pause()
{
  float curPos = getPos();
  stop();
  m_startPos = curPos;
}


void Music::stop()
{
  snd::account_music(this, false);

  m_startPos = 0.f;

  if (m_thread)
  {
    m_playing = false;
    ep::end_thread(m_thread);
    m_thread = NULL;
  }

  m_channel.destroy();
}


void Music::setVolume(float volume)
{
  m_volume = volume;
  if (m_playing)
    m_channel.setVolume(m_volume * snd::get_volume_music());
}


void Music::setPos(float sec)
{
  m_time = (int)(sec * 1000);
  ov_time_seek(&m_ogg, m_time);
}


float Music::getPos()
{
  return ov_time_tell(&m_ogg) * 0.001f;
}


void *Music::threadProc(void *val)
{
  Music *music = reinterpret_cast<Music*>(val);
  music->backgroudProc();
  return NULL;
}


void Music::backgroudProc()
{
  m_playing = true;

  alGenBuffers(QUEUE_LEN, (ALuint*)&m_ids);

  const int BUF_SIZE = 1024 * 64;
  char data[BUF_SIZE];

  for (int i=0; i<QUEUE_LEN; ++i)
    queueData(m_ids[i], data, BUF_SIZE);

  m_channel.setVolume(m_volume * snd::get_volume_music());
  m_channel.play();

  while (m_playing)
  {
    if (m_channel.queueProccessedCount() > 0)
    {
      int id = m_channel.unqueueBuffer();
      queueData(id, data, BUF_SIZE);

      if (!m_channel.isPlaying())
        m_channel.play();
    }
  }

  m_channel.stop();

  alDeleteBuffers(QUEUE_LEN, (ALuint*)&m_ids);
  memset(m_ids, 0, sizeof(m_ids));
}


void Music::queueData(int id, char *data, int size)
{
  if (!id)
    return;

  if (m_time >= m_timeTotal)
    setPos(0.f);

  int sres = snd::read_ogg(m_ogg, data, size);
  if (!sres)
    return;

  m_time = ov_time_tell(&m_ogg);
  //ep::log("reading ogg, time = %d, size = %d", (int)m_time, sres);

  alGetError();
  alBufferData(id, m_oggFormat, data, sres, m_oggFreq);

  if (alGetError() != AL_NO_ERROR)
  {
    ep::logError("music streaming error!");
    return;
  }

  m_channel.queueBuffer(id);
}

} // namespace ep
