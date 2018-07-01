#ifndef __EPOD_SOUND_INTERNAL_H__
#define __EPOD_SOUND_INTERNAL_H__

///
/// For internal use only
///

struct OggVorbis_File;

namespace ep
{

class Channel;
class Music;

namespace snd
{

/// from system.cpp
Channel *get_channel(int sound_id);
Channel *get_exist_channel(int sound_id);
void free_channel(int sound_id);

void account_music(Music *music, bool to_add);

/// from ogg_helper.cpp
bool open_ogg(const char *filename, OggVorbis_File &ogg);
void close_ogg(OggVorbis_File &ogg);
long read_ogg(OggVorbis_File &ogg, char *buffer, long size);

} // namespace snd

} // namespace ep

#endif //__EPOD_SOUND_INTERNAL_H__
