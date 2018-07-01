#include "../epod.h"
#include "../epod_fsystem.h"

#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>


namespace ep
{

namespace snd
{

bool open_ogg(const char *filename, OggVorbis_File &ogg)
{
  EPFILE *oggFile = ep::fopen(filename, ep::FILE_MODE_READ);
  if (!oggFile)
  {
    logError("[Sound] Opening vorbis. Can't open file '%s'", filename);
    return false;
  }

  int res = ov_open((FILE *)oggFile, &ogg, NULL, 0);
  if (res)
  {
    ep::fclose(oggFile);
    logError("[Sound] Opening vorbis. Can't recognize correct vorbis format (error code: '%d', file: '%s')", res, filename);
    return false;
  }

  return true;
}


void close_ogg(OggVorbis_File &ogg)
{
  ov_clear(&ogg); // ov_clear() cleans OggVorbis_File struct
}


long read_ogg(OggVorbis_File &ogg, char *buffer, long size)
{
  long res = 0;

  while (size > 0)
  {
    int bstream = 0;
    int bytes = ov_read(&ogg, buffer, size, &bstream);
    if (bytes <= 0)
      break;

    res += bytes;
    size -= bytes;
    buffer = buffer + bytes;
  }

  return res;
}

} // namespace snd

} // namespace ep
