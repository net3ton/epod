#include <windows.h>
#include <stdio.h>
#include "common.h"


int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    printf("Format: pvrconvert.exe png_file pvr_file pvr_format\n");
    return 0;
  }

  const char *filePng = argv[1];
  const char *filePvr = argv[2];
  const char *formatPvr = argv[3];

  int width, height;
  if (!ttex::png_to_pvr(filePng, formatPvr, filePvr, width, height))
    printf(ttex::get_error());

  return 1;
}
