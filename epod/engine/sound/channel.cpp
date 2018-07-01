#include "channel.h"

#include <assert.h>
#include <openal/al.h>


namespace ep
{

Channel::Channel()
  :m_id(0), m_soundId(0)
{
}


Channel::Channel(int sound_id)
  :m_id(0), m_soundId(sound_id)
{
  init();
  alSourcei(m_id, AL_BUFFER, sound_id);
}


Channel::~Channel()
{
  destroy();
}


void Channel::init()
{
  if (m_id)
    return;

  alGenSources(1, (ALuint*)&m_id);
}


void Channel::destroy()
{
  if (!m_id)
    return;

  alDeleteSources(1, (ALuint*)&m_id);
  m_id = 0;
}


void Channel::play()
{
  alSourcePlay(m_id);
}


void Channel::pause()
{
  alSourcePause(m_id);
}


void Channel::stop()
{
  alSourceStop(m_id);
}


bool Channel::isPlaying()
{
  ALint state = 0;
  alGetSourcei(m_id, AL_SOURCE_STATE, &state);
  return (state == AL_PLAYING);
}


bool Channel::isPaused()
{
  ALint state = 0;
  alGetSourcei(m_id, AL_SOURCE_STATE, &state);
  return (state == AL_PAUSED);
}


float Channel::getVolume()
{
  float volume = 1.f;
  alGetSourcef(m_id, AL_GAIN, &volume);
  return volume;
}


void Channel::setVolume(float volume)
{
  assert(volume >= 0.f && volume <= 1.f);
  alSourcef(m_id, AL_GAIN, volume);
}


void Channel::setLooped(bool looped)
{
  alSourcei(m_id, AL_LOOPING, looped ? AL_TRUE : AL_FALSE);
}


bool Channel::queueBuffer(int id)
{
  alGetError();
  alSourceQueueBuffers(m_id, 1, (ALuint*)&id);
  return (alGetError() == AL_NO_ERROR);
}


int Channel::unqueueBuffer()
{
  ALuint doneId = 0;
  alGetError();
  alSourceUnqueueBuffers(m_id, 1, &doneId);
  return (alGetError() == AL_NO_ERROR) ? doneId : 0;
}


int Channel::queueProccessedCount()
{
  ALint count = 0;
  alGetSourcei(m_id, AL_BUFFERS_PROCESSED, &count);
  return count;
}

} // namespace ep
