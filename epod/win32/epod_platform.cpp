#include "epod.h"
#include "epod_platform.h"
#include "epod_fsystem.h"

#include <math/rect2.h>
#include <gles/gl.h>
#include <stdio.h>
#include <windows.h>
#include <string>


namespace ep
{

int get_view_orient()
{
  return VIEW_ORIENT_NORMAL;
  //return ep::get_screen_orientation();
}


void log(const char *format, ...)
{
#ifdef _DEBUG
  va_list args;
  const int buffSize = 4096;
  char buffer[buffSize];

  va_start(args, format);
  vsnprintf_s(buffer, buffSize, buffSize, format, args);
  va_end(args);

  strcat_s(buffer, buffSize, "\n");

  OutputDebugString(buffer);
#endif
}


void logError(const char *format, ...)
{
  va_list args;
  const int buffSize = 4096;
  char buffer[buffSize];

  sprintf_s(buffer, buffSize, "Error: ");
  int size = strlen(buffer);

  va_start(args, format);
  vsnprintf_s(buffer + size, buffSize - size, buffSize - size, format, args);
  va_end(args);

  strcat_s(buffer, buffSize, "\n");

  OutputDebugString(buffer);
}


int get_time()
{
  return GetTickCount();
}


void get_exe_dir(char *buf, int size)
{
  GetModuleFileName(GetModuleHandle(NULL), (LPTSTR)buf, MAX_PATH);

  char *p = strrchr(buf, '\\');
  if (p)
    p[1] = '\0';
  else
    buf[0] = '\0';
}


std::string data_path;
const char *get_data_path()
{
  if (data_path.empty())
  {
    char buf[MAX_PATH];
    get_exe_dir(buf, MAX_PATH);

    strcat_s(buf, MAX_PATH, "data.bundle\\");
    data_path = buf;
  }

  return data_path.c_str();
}


std::string user_path;
const char *get_user_path()
{
  if (user_path.empty())
  {
    char buf[MAX_PATH];
    get_exe_dir(buf, MAX_PATH);

    strcat_s(buf, MAX_PATH, "user\\");
    user_path = buf;
  }

  return user_path.c_str();
}


std::string cur_loc;
const char *get_loc()
{
  if (cur_loc.empty())
  {
    LCID lcid = GetThreadLocale();

    const int BUFFSIZE = 3;
    char buf[BUFFSIZE];

    if (GetLocaleInfo(lcid, LOCALE_SISO639LANGNAME, buf, BUFFSIZE))
      cur_loc = buf;
    else
      cur_loc = "en";
  }

  return cur_loc.c_str();
}

void set_loc(const char *loc)
{
  cur_loc = loc;
}


void set_accelerometer_enabled(int enabled) {}

} // namespace ep
