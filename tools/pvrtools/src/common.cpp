#include "common.h"
#include <string.h>
#include <assert.h>
#include <algorithm>

#include <PVRTexLib.h>
#include <corona.h>

#pragma comment(lib, "PVRTexLib.lib")
#pragma comment(lib, "corona.lib")


using namespace pvrtexlib;


namespace ttex
{

std::string errorText;

const char *get_error()
{
  if (errorText.empty())
    return NULL;

  return errorText.c_str();
}

void set_error(const char *error, ...)
{
  va_list args;
  const int BUFF_SIZE = 4096;
  char buffer[BUFF_SIZE];

  va_start(args, error);
  vsnprintf_s(buffer, BUFF_SIZE, BUFF_SIZE, error, args);
  va_end(args);

  errorText = buffer;
}

void reset_error()
{
  errorText.clear();
}


PixelType get_pixel_type(const char *format)
{
  if (!strcmp(format, "rgba_8888"))
    return OGL_RGBA_8888;
  if (!strcmp(format, "rgba_4444"))
    return OGL_RGBA_4444;
  if (!strcmp(format, "rgb_888"))
    return OGL_RGB_888;
  if (!strcmp(format, "rgb_565"))
    return OGL_RGB_565;
  if (!strcmp(format, "pvrtc4"))
    return OGL_PVRTC4;
  if (!strcmp(format, "pvrtc2"))
    return OGL_PVRTC2;

  return OGL_RGBA_4444;
}


bool make_pvr(void *pixels, int width, int height, const char *outformat, const char *outfile)
{
  PVRTRY
  {
    PVRTextureUtilities *PVRU = PVRTextureUtilities::getPointer();

    // make a new texture
    CPVRTexture sResTexture(
      (unsigned int)width,      // u32Width,
      (unsigned int)height,     // u32Height,
      0,                        // u32MipMapCount,
      1,                        // u32NumSurfaces,
      false,                    // bBorder,
      true,                     // bTwiddled,
      false,                    // bCubeMap,
      false,                    // bVolume,
      false,                    // bFalseMips,
      true,                     // bHasAlpha,
      false,                    // bVerticallyFlipped,
      eInt8StandardPixelType,   // ePixelType,
      0.0f,                     // fNormalMap,
      (uint8 *)pixels           // pPixelData
      );

    // compress texture
    CPVRTexture sOutTexture(sResTexture.getHeader());
    sOutTexture.setPixelType(get_pixel_type(outformat));
    PVRU->CompressPVR(sResTexture, sOutTexture);

    sOutTexture.writeToFile(outfile);
  }
  PVRCATCH(myException)
  {
    set_error("Exception: %s", myException.what());
    return false;
  }

  return true;
}


bool png_to_pvr(const char *infile, const char *outformat, const char *outfile, int &width, int &height)
{
  corona::Image *image = corona::OpenImage(infile, corona::PF_R8G8B8A8);
  if (!image)
  {
    set_error("Error: can't open PNG file '%s'\n", infile);
    return false;
  }

  // data in pvr is flipped
  image = corona::FlipImage(image, corona::CA_X);

  width = image->getWidth();
  height = image->getHeight();

  bool res = make_pvr(image->getPixels(), width, height, outformat, outfile);
  delete image;

  return res;
}


///////////////////////////////////////////////////////////////////////////////

const int STRIDE = 1;

struct AtlasRect
{
  AtlasRect(int nx, int ny, int nw, int nh)
    :x(nx), y(ny), w(nw), h(nh)
  {
  }

  bool place(int width, int height) const
  {
    return width <= w && height <= h;
  }

  bool fitsStrict(int width, int height) const
  {
    return width == w && height == h;
  }

  int x, y, w, h;
};


struct AtlasNode
{
  AtlasNode(int x, int y, int w, int h)
    :image(-1), one(NULL), two(NULL), rect(x, y, w, h)
  {
  }

  ~AtlasNode()
  {
    if (one)
      delete one;
    if (two)
      delete two;
  }

  AtlasNode *insert(int image_id, int w, int h)
  {
    if (one && two)
    {
      AtlasNode *node = one->insert(image_id, w, h);
      if (node)
        return node;

      return two->insert(image_id, w, h);
    }
    else
    {
      assert(!one && !two && "it must be strict tree leaf");

      if (image != -1)
        return NULL;

      if (!rect.place(w, h))
        return NULL;

      if (rect.fitsStrict(w, h))
      {
        image = image_id;
        return this;
      }

      int dw = rect.w - w;
      int dh = rect.h - h;

      if (dw > dh)
      {
        one = new AtlasNode(rect.x, rect.y, w, rect.h);
        two = new AtlasNode(rect.x + (w + STRIDE), rect.y, rect.w - (w + STRIDE), rect.h);
      }
      else
      {
        one = new AtlasNode(rect.x, rect.y, rect.w, h);
        two = new AtlasNode(rect.x, rect.y + (h + STRIDE), rect.w, rect.h - (h + STRIDE));
      }

      return one->insert(image_id, w, h);
    }
  }

  AtlasNode *one, *two;
  AtlasRect rect;
  int image;
};


void place_tile_to_atlas(char *atlas, int width, int height, char *data, AtlasRect &rect, TileInfo &tile)
{
  assert((rect.x + rect.w) <= width && (rect.y + rect.h) <= height);

  /*
  // not flipped
  for (int iy=0 ; iy<rect.h ; ++iy)
  {
    int inIndex = ((iy + tile.yoffset) * tile.wtex + tile.xoffset) * RAW_PIXEL_SIZE;
    int outIndex = ((iy + rect.y) * width + rect.x) * RAW_PIXEL_SIZE;

    memcpy(atlas + outIndex, data + inIndex, (size_t)rect.w * RAW_PIXEL_SIZE);
  }
  */

  // up-down flipped (pvr data is up-downed)
  for (int iy = 0; iy < rect.h; ++iy)
  {
    int inIndex = ((iy + tile.yoffset) * tile.wtex + tile.xoffset) * RAW_PIXEL_SIZE;
    int outIndex = (((rect.h - iy - 1) + (height - rect.y - rect.h)) * width + rect.x) * RAW_PIXEL_SIZE;

    memcpy(atlas + outIndex, data + inIndex, (size_t)rect.w * RAW_PIXEL_SIZE);
  }
}


int get_alpha(unsigned int color)
{
  return (color & 0xFF000000) >> 24;
}

int find_left(unsigned int *data, int width, int height, int min_alpha)
{
  for (int left = 0; left < width; ++left)
    for (int j = 0; j < height; ++j)
      if (get_alpha(data[left + j*width]) > min_alpha)
        return left;
  return -1;
}

int find_right(unsigned int *data, int width, int height, int min_alpha)
{
  for (int right = width - 1; right >= 0; --right)
    for (int j = 0; j < height; ++j)
      if (get_alpha(data[right + j*width]) > min_alpha)
        return right;
  return -1;
}

int find_top(unsigned int *data, int width, int height, int min_alpha)
{
  for (int top = 0; top < height; ++top)
    for (int i = 0; i < width; ++i)
      if (get_alpha(data[i + top*width]) > min_alpha)
        return top;
  return -1;
}

int find_botom(unsigned int *data, int width, int height, int min_alpha)
{
  for (int bottom = height - 1; bottom >= 0; --bottom)
    for (int i = 0; i < width; ++i)
      if (get_alpha(data[i + bottom*width]) > min_alpha)
        return bottom;
  return -1;
}

void check_tile_bbox(unsigned int *data, int width, int height, int min_alpha, TileInfo &tile)
{
  int left = find_left(data, width, height, min_alpha);
  if (left == -1)
    return;

  int right = find_right(data, width, height, min_alpha);
  int top = find_top(data, width, height, min_alpha);
  int bottom = find_botom(data, width, height, min_alpha);

  tile.xoffset = left;
  tile.yoffset = top;
  tile.width = right - left + 1;
  tile.height = bottom - top + 1;
  tile.wtex = width;
  tile.htex = height;
};


bool init_tile_size(TileInfo &tile)
{
  corona::Image *image = corona::OpenImage(tile.tileName.c_str(), corona::PF_R8G8B8A8);
  if (!image)
  {
    set_error("Error: can't open file '%s'\n", tile.tileName.c_str());
    return false;
  }

  check_tile_bbox((unsigned int *)image->getPixels(), image->getWidth(), image->getHeight(), 0, tile);

  delete image;
  return true;
}

bool tile_cmp(const TileInfo *a, const TileInfo *b)
{
  return (a->width + a->height) > (b->width + b->height);
}


bool place_tiles(TileVector &tiles, int width, int height, const char *outformat, const char *outfile)
{
  TileInfo *first = NULL;

  // find first not placed tile
  for (size_t i=0 ; i<tiles.size() ; ++i)
  {
    if (tiles[i]->texName.empty())
    {
      first = tiles[i];
      break;
    }
  }

  // all tiles have been placed already
  if (!first)
    return false;

  assert((first->width <= width) && (first->height <= height) && "Tile is bigger then atlas!");
  //if ((first->width > width) || (first->height > height))
  //{
  //  set_error("Error: tile is bigger then atlas '%s'\n", first->tileName.c_str());
  //  return false;
  //}

  // root tree node for placing tiles to atlas
  AtlasNode root(0, 0, width, height);

  // atlas data
  const unsigned int atlasSize = width * height * RAW_PIXEL_SIZE;
  char *atlasPixels = new char[atlasSize];
  memset(atlasPixels, 0, atlasSize);

  // placing tiles
  for (size_t i=0 ; i<tiles.size() ; ++i)
  {
    TileInfo *tile = tiles[i];

    if (!tile->texName.empty())
      continue;

    // try to place tile
    AtlasNode *node = root.insert(i, tile->width, tile->height);
    if (node)
    {
      corona::Image *image = corona::OpenImage(tile->tileName.c_str(), corona::PF_R8G8B8A8);

      // copy tile data to atlas
      place_tile_to_atlas(atlasPixels, width, height, (char *)image->getPixels(), node->rect, *tile);

      // save position in altas and atlas name
      tile->texName = outfile;
      tile->x = node->rect.x;
      tile->y = node->rect.y;

      delete image;
    }
    // tile hasn't been placed - will try to place next time
    else
    {
    }
  }

  bool res = make_pvr(atlasPixels, width, height, outformat, outfile);

  delete[] atlasPixels;
  return res;
}


bool make_atlas(TileVector &tiles, int width, int height, const char *outformat, const char *outfile)
{
  // initing tile sizes (for sorting)
  for (size_t i=0 ; i<tiles.size() ; ++i)
  {
    if (!init_tile_size(*tiles[i]))
      return false;

    if ((tiles[i]->width > width) || (tiles[i]->height > height))
    {
      set_error("Error: tile is bigger then atlas (%dx%d) '%s'\n",
        tiles[i]->width, tiles[i]->height, tiles[i]->tileName.c_str());
      return false;
    }
  }

  // sorting tiles (from bigger to smaller)
  std::sort(tiles.begin(), tiles.end(), tile_cmp);

  const int NAME_MAX_LEN = 1024;
  char atlasName[NAME_MAX_LEN];

  reset_error();
  int atlasNum = 0;

  do
  {
    sprintf_s(atlasName, NAME_MAX_LEN, "%s_%02d.pvr", outfile, atlasNum);
    atlasNum++;
  }
  while (place_tiles(tiles, width, height, outformat, atlasName));

  if (get_error())
    return false;

  return true;
}

}
