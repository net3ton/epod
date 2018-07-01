#ifndef __EPOD_SOUND_SYSTEM_H__
#define __EPOD_SOUND_SYSTEM_H__


namespace ep
{

class Channel;
class Music;

namespace snd
{

bool init();
void destroy();
void suspend(bool to_suspend);

void set_volume_sound(float volume);
void set_volume_music(float volume);
float get_volume_sound();
float get_volume_music();

void set_music(Music *music);
void fade_to_music(Music *music);

void update(long dt);

} // namespace snd

} // namespace ep

#endif //__EPOD_SOUND_SYSTEM_H__
