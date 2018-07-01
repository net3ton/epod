#ifndef _EPOD_FSYSTEM_H_
#define _EPOD_FSYSTEM_H_


namespace ep
{

typedef int EPFILE;

const int MAX_PATH = 1024;
const int MAX_LINE_LEN = 512;

enum
{
  FILE_MODE_READ,
  FILE_MODE_WRITE,
};


// file operations
EPFILE *fopen(const char *filename, int mode);
int fclose(EPFILE *file);


// helpers
bool file_is_exist(const char *filename);
bool make_dirs(const char *path, bool filepath = true);


// helper class
// with autoclose and autofree in dctr()
class File
{
public:
  File();
  File(const char *filename, int mode);
  ~File();

  EPFILE *open(const char *filename, int mode);
  void close();

  bool opened();

  void *readEntire();
  void freeEntire();

  int readInt();
  float readFloat();
  const char *readStr();
  const char *readLine();

  void writeInt(int val);
  void writeFloat(float val);
  void writeStr(const char *val);

private:
  EPFILE *file;
  char *data;
  char buffer[MAX_LINE_LEN];
};

}

#endif //_EPOD_FSYSTEM_H_
