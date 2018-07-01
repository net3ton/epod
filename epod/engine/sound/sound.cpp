#include "sound.h"
#include "channel.h"
#include "system.h"
#include "internal.h"

#include "../epod.h"

#include <assert.h>
#include <openal/al.h>
#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>


namespace ep
{

Sound::Sound(const char *filename, bool looped)
  :m_id(0), m_fileName(filename), m_looped(looped), m_volume(1.f)
{
}


Sound::~Sound()
{
  free();
}


bool Sound::load()
{
  if (m_fileName.empty())
  {
    logError("Can't load sound. Filename is empty!");
    return false;
  }

  return loadFromFile(m_fileName.c_str());
}


void Sound::free()
{
  if (!m_id)
    return;

  snd::free_channel(m_id);

  alDeleteBuffers(1, (ALuint*)&m_id);
  m_id = 0;

  log("Sound deleted: %s", m_fileName.c_str());
}


void Sound::play(bool restart)
{
  if (!m_id && !load())
    return;

  Channel *channel = snd::get_channel(m_id);
  if (!channel)
    return;

  if (!restart && channel->isPlaying())
    return;

  channel->setLooped(m_looped);
  channel->setVolume(m_volume * snd::get_volume_sound());
  channel->play();
}


void Sound::pause()
{
  Channel *channel = snd::get_exist_channel(m_id);
  if (channel)
    channel->pause();
}


void Sound::stop()
{
  Channel *channel = snd::get_exist_channel(m_id);
  if (channel)
    channel->stop();
}


void Sound::setVolume(float volume)
{
  m_volume = volume;

  Channel *channel = snd::get_exist_channel(m_id);
  if (channel)
    channel->setVolume(m_volume * snd::get_volume_sound());
}


bool Sound::loadFromFile(const char *filename)
{
  free();

  alGenBuffers(1, (ALuint*)&m_id);

  if (!loadFromOgg(filename))
  {
    free();
    return false;
  }

  return true;
}


bool Sound::loadFromOgg(const char *filename)
{
  OggVorbis_File oggStream;
  if (!snd::open_ogg(filename, oggStream))
    return false;

  // Get some information about the OGG file
  vorbis_info *oggInfo = ov_info(&oggStream, -1);
  if (NULL == oggInfo)
  {
    logError("Sound loading. Vorbis. can't get info");
    return false;
  }

  ALsizei freq = oggInfo->rate;
  ALenum format = (oggInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

  // Load all data
  {
    ogg_int64_t samples = ov_pcm_total(&oggStream, -1);
    assert(OV_EINVAL != samples);

    long size = 2 * oggInfo->channels * samples;
    char *data = new char[size];

    snd::read_ogg(oggStream, data, size);
    alBufferData(m_id, format, data, size, freq);

    delete[] data;
  }

  log("Sound loaded: '%s' (freq: %d, channels: %d, bitrate: %d)", filename,
    oggInfo->rate, oggInfo->channels, oggInfo->bitrate_nominal);

  snd::close_ogg(oggStream);

  return true;
}

} // namespace ep
