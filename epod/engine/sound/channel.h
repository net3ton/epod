#ifndef __EPOD_SOUND_CHANNEL_H__
#define __EPOD_SOUND_CHANNEL_H__


namespace ep
{

class Channel
{
public:
  Channel();
  Channel(int sound_id);
  ~Channel();

  void init();
  void destroy();

  void play();
  void pause();
  void stop();

  bool isPlaying();
  bool isPaused();

  void setVolume(float volume);
  float getVolume();

  void setLooped(bool looped);

  bool queueBuffer(int id);
  int unqueueBuffer();
  int queueProccessedCount();

  int getSoundId() const { return m_soundId; }

protected:
  int m_id;
  int m_soundId;
};

} // namespace ep

#endif //__EPOD_SOUND_CHANNEL_H__
