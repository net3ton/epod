#include "epod.h"
#include "epod_fsystem.h"

#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <fstream>
#include <string.h>
#include <assert.h>

#ifndef __GNUC__
  #include <direct.h>
#endif

#pragma warning(push)
#pragma warning(disable: 4996) // 'fopen' was declared deprecated (in vs) - but it's crossplatform!
#pragma warning(disable: 4267) // conversion from 'size_t' to 'int'
#pragma warning(disable: 4800) // forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4018) // '<' : signed/unsigned mismatch


namespace ep
{

const char *USER_FOLDER_MARK = "user/";


static void normalize_slashes(char *path)
{
  if (!path)
    return;

  int ind = 0;
  for (int i=0; i<strlen(path); ++i)
  {
    if (path[i] == '\\')
      path[i] = '/';

    if (path[i] == '/' && ind && path[ind-1] == '/')
      continue;

    path[ind] = path[i];
    ind++;
  }

  if (ind && path[ind-1] == '/')
    ind--;

  path[ind] = '\0';
}


const char *normalize_path(const char *path, char *path_out, int size)
{
  if (!path)
    return 0;

  while (*path == '/' || *path == '\\')
    path++;

  if (path == strstr(path, USER_FOLDER_MARK))
  {
    assert((strlen(ep::get_user_path()) + strlen(path) - strlen(USER_FOLDER_MARK)) < size - 1
      && "Path is too long!");

    strcpy(path_out, ep::get_user_path());
    strcat(path_out, path + strlen(USER_FOLDER_MARK));
  }
  else
  {
    assert((strlen(ep::get_data_path()) + strlen(path)) < size - 1
      && "Path is too long!");

    strcpy(path_out, ep::get_data_path());
    strcat(path_out, path);
  }

  normalize_slashes(path_out);

  return path_out;
}


EPFILE *fopen(const char *filename, int mode)
{
  const char *fmode = (mode == FILE_MODE_READ) ? "rb" : (mode == FILE_MODE_WRITE) ? "wb" : NULL;
  assert(fmode && "Unknown file open mode!");

  char buf[MAX_PATH];
  normalize_path(filename, buf, MAX_PATH);

  //log("file to open: '%s'", buf);
  return (EPFILE*)::fopen(buf, fmode);
}


int fclose(EPFILE *file)
{
  return ::fclose((FILE *)file);
}


int fread(EPFILE *file, void *buffer, int size, int count)
{
  return ::fread(buffer, size, count, (FILE *)file);
}


int fwrite(EPFILE *file, const void *buffer, int size, int count)
{
  return ::fwrite(buffer, size, count, (FILE *)file);
}


int feof(EPFILE *file)
{
  return ::feof((FILE *)file);
}


int fsize(EPFILE *file)
{
  struct stat fileStat;
  ::fstat(::fileno((FILE *)file), &fileStat);

  return fileStat.st_size;
}


char *fgets(EPFILE *file, char *buffer, int size)
{
  return ::fgets(buffer, size, (FILE *)file);
}


int fgetc(EPFILE *file)
{
  return ::fgetc((FILE *)file);
}


///////////////////////////////////////////////////////////////////////////////


bool file_is_exist(const char *filename)
{
  char buf[MAX_PATH];
  return std::ifstream(normalize_path(filename, buf, MAX_PATH)) != NULL;
}


static bool mk_dir(const char *dir_path)
{
  #ifndef __GNUC__
  int res = ::mkdir(dir_path);
  #else
  int res = ::mkdir(dir_path, S_IRWXU);
  #endif

  return (res == 0) || (errno == EEXIST);
}

bool make_dirs(const char *path, bool filepath)
{
  if (!path || !*path)
    return false;

  char pathClean[MAX_PATH];
  normalize_path(path, pathClean, MAX_PATH);

  bool res = true;

  char *slash = strchr(pathClean, '/');
  while (slash && res)
  {
    *slash = '\0';
    res = mk_dir(pathClean);
    *slash = '/';

    slash = strchr(slash + 1, '/');
  }

  if (!filepath && res)
    mk_dir(pathClean);

  return res;
}


///////////////////////////////////////////////////////////////////////////////


File::File()
  :file(NULL), data(NULL)
{
}

File::File(const char *filename, int mode)
  :file(NULL), data(NULL)
{
  open(filename, mode);
}

File::~File()
{
  freeEntire();
  close();
}


EPFILE *File::open(const char *filename, int mode)
{
  if (file)
    return file;

  file = fopen(filename, mode);
  return file;
}


void File::close()
{
  if (file)
  {
    fclose(file);
    file = NULL;
  }
}


bool File::opened()
{
  return (bool)file;
}


void *File::readEntire()
{
  if (!file)
    return NULL;

  if (data)
    return data;

  int size = fsize(file);
  data = new char[size];
  fread(file, data, 1, size);
  return data;
}


void File::freeEntire()
{
  if (data)
  {
    delete[] data;
    data = NULL;
  }
}


const char *File::readLine()
{
  if (!file)
    return NULL;

  return fgets(file, buffer, MAX_LINE_LEN);
}


void File::writeInt(int val)
{
  if (file)
    fwrite(file, &val, sizeof(int), 1);
}


void File::writeFloat(float val)
{
  if (file)
    fwrite(file, &val, sizeof(float), 1);
}


void File::writeStr(const char *val)
{
  if (file)
    fwrite(file, &val, sizeof(char), ::strlen(val) + 1);
}


int File::readInt()
{
  if (!file)
    return 0;

  int res = 0;
  fread(file, &res, sizeof(int), 1);
  return res;
}


float File::readFloat()
{
  if (!file)
    return 0.f;

  float res = 0.f;
  fread(file, &res, sizeof(float), 1);
  return res;
}


const char *File::readStr()
{
  if (!file)
    return NULL;

  int ind = 0;
  while (!feof(file))
  {
    int val = fgetc(file);
    buffer[ind++] = (char)val;
    if (!val)
      break;
  }

  buffer[ind++] = '\0';
  return buffer;
}

} // namespace ep

#pragma warning(pop)
