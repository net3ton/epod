#include "channel.h"
#include "music.h"

#include "../epod.h"

#include <assert.h>
#include <openal/al.h>
#include <openal/alc.h>
#include <list>


namespace ep
{

namespace snd
{

ALCcontext *m_context = 0;
ALCdevice *m_device = 0;

typedef std::list<Channel*> ChannelList;
typedef ChannelList::iterator ChannelIter;

typedef std::list<Music*> MusicList;
typedef MusicList::iterator MusicIter;

ChannelList m_channels;
MusicList m_musics;

float m_volume_sound = 1.f;
float m_volume_music = 1.f;


///////////////////////////////////////////////////////////////////////////////


Music *cur_music = NULL;

Music *next_music = NULL;
int next_music_delay = 0;
const int NEXT_MUSIC_TIME = 1000;


void set_music(Music *music)
{
  if (next_music)
  {
    next_music->stop();
    next_music = NULL;
  }

  if (cur_music == music)
    return;

  if (cur_music)
    cur_music->stop();

  cur_music = music;
  cur_music->play();
}


void fade_to_music(Music *music)
{
  if (music == cur_music)
    return;

  next_music = music;
  next_music->play();
  next_music->setVolume(0.f);
  next_music_delay = NEXT_MUSIC_TIME;
}


void update(long dt)
{
  if (!next_music)
    return;

  next_music_delay -= dt;
  if (next_music_delay < 0)
    next_music_delay = 0;

  float progress = next_music_delay * 2.f / NEXT_MUSIC_TIME;

  if (progress >= 1.f)
  {
    if (cur_music)
      cur_music->setVolume(progress - 1.f);
  }
  else
  {
    next_music->setVolume(1.f - progress);
  }

  if (!next_music_delay)
  {
    if (cur_music)
      cur_music->stop();
    cur_music = next_music;
    next_music = NULL;
  }
}


///////////////////////////////////////////////////////////////////////////////


bool init()
{
  m_device = alcOpenDevice("Generic Software");
  if (!m_device)
  {
    logError("alcOpenDevice: %d", alGetError());
    return false;
  }

  m_context = alcCreateContext(m_device, NULL);
  if (!m_context)
  {
    logError("alcCreateContext: %d", alGetError());
    return false;
  }

  if (!alcMakeContextCurrent(m_context))
  {
    logError("alcMakeContextCurrent: %d", alGetError());
    return false;
  }

  log("Sound driver is inited. (%s)", alcGetString(m_device, ALC_DEVICE_SPECIFIER));
  alGetError();
  return true;
}


void destroy()
{
  for (ChannelIter ch=m_channels.begin(), end=m_channels.end(); ch!=end; ++ch)
    delete (*ch);

  m_channels.clear();

  if (m_context)
  {
    alcMakeContextCurrent(NULL);
    alcDestroyContext(m_context);
    m_context = 0;
  }

  if (m_device)
  {
    alcCloseDevice(m_device);
    m_device = 0;
  }

  log("Sound driver is destroyed.");
}


void suspend(bool to_suspend)
{
  if (!m_context)
    return;

  if (to_suspend)
  {
    log("Sound driver is suspended...");
    alcMakeContextCurrent(NULL);
    alcSuspendContext(m_context);
  }
  else
  {
    log("Sound driver is resumed...");
    alcMakeContextCurrent(m_context);
    alcProcessContext(m_context);
  }
}


///////////////////////////////////////////////////////////////////////////////


void set_volume_sound(float volume)
{
  assert(volume >= 0.f && volume <= 1.f);
  m_volume_sound = volume;

  //if (!m_context)
  //  return;

  //for (ChannelIter ch=m_channels.begin(), end=m_channels.end(); ch!=end; ++ch)
  //  (*ch)->setVolume(volume);
}

float get_volume_sound() { return m_volume_sound; }

void set_volume_music(float volume)
{
  assert(volume >= 0.f && volume <= 1.f);
  m_volume_music = volume;

  if (!m_context)
    return;

  // updating volumes
  for (MusicIter m=m_musics.begin(), end=m_musics.end(); m!=end; ++m)
    (*m)->setVolume((*m)->getVolume());
}

float get_volume_music() { return m_volume_music; }


///////////////////////////////////////////////////////////////////////////////
/// For internal use only
///////////////////////////////////////////////////////////////////////////////


Channel *get_exist_channel(int sound_id)
{
  if (!m_context)
    return NULL;

  for (ChannelIter ch=m_channels.begin(), end=m_channels.end(); ch!=end; ++ch)
  {
    Channel *channel = (*ch);
    if (channel->getSoundId() == sound_id)
      return channel;
  }

  return NULL;
}


Channel *get_channel(int sound_id)
{
  if (!m_context)
    return NULL;

  Channel *channel = get_exist_channel(sound_id);
  if (channel)
    return channel;

  channel = new Channel(sound_id);
  m_channels.push_back(channel);
  return channel;
}


void free_channel(int sound_id)
{
  if (!m_context)
    return;

  for (ChannelIter ch=m_channels.begin(), end=m_channels.end(); ch!=end; ++ch)
  {
    Channel *channel = (*ch);
    if (channel->getSoundId() == sound_id)
    {
      delete channel;
      m_channels.erase(ch);
      break;
    }
  }
}


void account_music(Music *music, bool to_add)
{
  if (!m_context)
    return;

  for (MusicIter m=m_musics.begin(), end=m_musics.end(); m!=end; ++m)
  {
    if ((*m) == music)
    {
      if (!to_add)
        m_musics.erase(m);
      return;
    }
  }

  if (to_add)
    m_musics.push_back(music);
}

} // namespace snd

} // namespace ep
