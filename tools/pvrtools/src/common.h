#ifndef _PVR_TOOLS_COMMON_H_
#define _PVR_TOOLS_COMMON_H_

#include <vector>
#include <string>


namespace ttex
{

const int RAW_PIXEL_SIZE = 4;

const char *get_error();

bool make_pvr(void *pixels, int width, int height, const char *outformat, const char *outfile);
bool png_to_pvr(const char *infile, const char *outformat, const char *outfile, int &width, int &height);


struct TileInfo
{
  TileInfo(const char *tile_name, const char *tile_id = "")
    :x(0), y(0), width(0), height(0), xoffset(0), yoffset(0), wtex(0), htex(0)
    ,tileName(tile_name), id(tile_id)
  {
  }

  int x, y;              // out (tile pos in atlas)
  int width, height;     // out (tile size in atlas)
  int xoffset, yoffset;  // out (tile pos in orig tex)
  int wtex, htex;        // out (orig tex size)
  std::string id;        // not modified, optional
  std::string tileName;  // in
  std::string texName;   // out (atlas name)
};

typedef std::vector<TileInfo *> TileVector;

bool make_atlas(TileVector &tiles, int width, int height, const char *outformat, const char *outfile);

}

#endif //_PVR_TOOLS_COMMON_H_
