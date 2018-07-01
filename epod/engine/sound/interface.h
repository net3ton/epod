#ifndef __EPOD_SOUND_INTERFACE_H__
#define __EPOD_SOUND_INTERFACE_H__


namespace ep
{


class ISound
{
public:
  virtual ~ISound() {}

  virtual bool load() = 0;
  virtual void free() = 0;

  virtual void play(bool restart = true) = 0;
  virtual void pause() = 0;
  virtual void stop() = 0;

  virtual void setVolume(float volume) = 0;
  virtual float getVolume() = 0;
};

} // namespace ep

#endif //__EPOD_SOUND_INTERFACE_H__
