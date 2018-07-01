#import "epod_delegate.h"

#include <epod.h>
#include <epod_platform.h>
#include <gles/gl.h>
#include <cstdarg>
#include <sys/time.h>

#include <string>

namespace ep
{

int get_view_orient()
{
  return ep::get_screen_orientation();
}


void log(const char *format, ...)
{
  std::va_list args;
  va_start(args, format);
  NSLogv([NSString stringWithUTF8String:format], args);
  va_end(args);
}


void logError(const char *format, ...)
{
  std::va_list args;
  va_start(args, format);
  NSLogv([NSString stringWithUTF8String:format], args);
  va_end(args);
}


int get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_usec / 1000 + tv.tv_sec * 1000;
}


std::string data_path;
const char *get_data_path()
{
  if (data_path.empty())
  {
    NSString *value = [[NSBundle mainBundle] bundlePath];
    data_path = [value cStringUsingEncoding:NSASCIIStringEncoding];

    data_path += "/data.bundle/";
  }

  return data_path.c_str();
}


std::string user_path;
const char *get_user_path()
{
  if (user_path.empty())
  {
    NSString *value = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    user_path = [value cStringUsingEncoding:NSASCIIStringEncoding];
	  
    user_path += "/";
  }

  return user_path.c_str();
}


std::string cur_loc;
const char *get_loc()
{
  if (cur_loc.empty())
  {
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *langs = [defaults objectForKey:@"AppleLanguages"];
    NSString *curLang = [langs objectAtIndex:0];
    cur_loc = [curLang cStringUsingEncoding:NSASCIIStringEncoding];
  }

  return cur_loc.c_str();
}

void set_loc(const char *loc)
{
  cur_loc = loc;
}


void set_accelerometer_enabled(int enabled)
{
  if (enabled)
    [[UIAccelerometer sharedAccelerometer] setDelegate: [EPodAppDelegate sharedAppDelegate]];
  else
    [[UIAccelerometer sharedAccelerometer] setDelegate: nil];
}

} // namespace ep
