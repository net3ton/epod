#ifndef _EPOD_PLATFORM_H_
#define _EPOD_PLATFORM_H_

#include <math/rect2.h>

///
/// EPod platform specific code
///

namespace ep
{

int get_view_orient();

void log(const char *format, ...);
void logError(const char *format, ...);

int get_time();

const char *get_data_path();
const char *get_user_path();

/// localization

const char *get_loc();
void set_loc(const char *loc);
void set_supported_locs(const char *locs);

///

void set_accelerometer_enabled(int enabled);

}

#endif //_EPOD_PLATFORM_H_
