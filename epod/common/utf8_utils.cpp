#include "utf8_utils.h"

// Basic UTF-8 manipulation routines
// by Jeff Bezanson
// placed in the public domain Fall 2005

static const unsigned offsetsFromUTF8[6] = 
{
  0x00000000UL, 0x00003080UL, 0x000E2080UL,
  0x03C82080UL, 0xFA082080UL, 0x82082080UL
};

static const char trailingBytesForUTF8[256] = 
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};


unsigned UTF8ToCode(const char **inout_utf8str, const char *end_utf8str)
{
  const char *src = *inout_utf8str;
  int nb = trailingBytesForUTF8[(unsigned char)*src];

  //if (nb+1 >= end_utf8str - src)
  //    (nb+1 == end_utf8str - src) || DIE("invalid utf-8 string, trailing invalid chars");

  unsigned ch = 0;
  switch (nb)
  {
    // these fall through deliberately
    case 3: ch += (unsigned char)*src++; ch <<= 6;
    case 2: ch += (unsigned char)*src++; ch <<= 6;
    case 1: ch += (unsigned char)*src++; ch <<= 6;
    case 0: ch += (unsigned char)*src++;
  }

  ch -= offsetsFromUTF8[nb];

  //(ch <= WCHAR_MAX) || DIE("can't convert string from UTF-8 to wide char (may be surrogate character or  it's not UTF-8, but one-byte encoding?");

  *inout_utf8str = src;
  return ch;
}


int UTF8ToCodes(unsigned *out, const char *utf8str, int size)
{
  const char *utf8str_end = utf8str + size;
  int i=0;
  while (utf8str < utf8str_end)
    out[i++] = UTF8ToCode(&utf8str, utf8str_end);
  return i;
}
