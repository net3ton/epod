#include <windows.h>
#include <stdio.h>
#include "common.h"


int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    printf("Format: pvratlas.exe png_tiles_list.txt pvr_file pvr_format\n");
    return 0;
  }

  const char *fileListPath = argv[1];
  const char *filePvr = argv[2];
  const char *formatPvr = argv[3];

  ttex::TileVector tiles;

  FILE *flist = NULL;
  fopen_s(&flist, fileListPath, "r");
  if (flist)
  {
    while (!feof(flist))
    {
      char fpath[260];
      fscanf_s(flist, "%s", fpath, 260);

      tiles.push_back(new ttex::TileInfo(fpath));
    }
    fclose(flist);
  }
  else
    printf("Error: can't open file '%s'\n", fileListPath);

  if (!ttex::make_atlas(tiles, 256, 256, formatPvr, filePvr))
    printf(ttex::get_error());

  // tiles info
  std::string textureName;
  for (size_t i=0 ; i<tiles.size() ; ++i)
  {
    if (textureName != tiles[i]->texName)
    {
      textureName = tiles[i]->texName;
      printf("-------------------------------------------------------------------\n");
      printf("Texture: '%s'\n", textureName.c_str());
      printf("-------------------------------------------------------------------\n");
    }

    printf("Tile: '%s'\n", tiles[i]->tileName.c_str());
    printf("Info: x:%d, y:%d, w:%d, h:%d\n", tiles[i]->x, tiles[i]->y, tiles[i]->width, tiles[i]->height);
    printf("-----\n");

    delete tiles[i];
  }

  return 1;
}
