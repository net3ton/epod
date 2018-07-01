#ifndef _COMMON_UTF8_UTILS_H_
#define _COMMON_UTF8_UTILS_H_


const int UTF8_CODE_MIN = 0;
const int UTF8_CODE_MAX = 0xffff;

/// convert several bytes to code
unsigned UTF8ToCode(const char **inout_utf8str, const char *end_utf8str);

/// convert size bytes from utf8str to out, returns codes count (return count <= size for non-latine)
int UTF8ToCodes(unsigned *out, const char *utf8str, int size);

#endif // _COMMON_UTF8_UTILS_H_
