#ifndef __FX_PATH_H__
#define __FX_PATH_H__

#include <math/point2.h>
#include <vector>
#include <timer_int.h>

namespace ep { class IGraphics; }
namespace ep { class Tile; }


class StarPath
{
public:
  StarPath();

  void init(const Point2f &pos, int type);

  void update(long dt);
  void render(ep::IGraphics *graphics);

private:
  const ep::Tile *m_tile;
  int m_color, m_type;
  Point2f m_pos, m_dir;
  TimerInt m_timer;
};


class PathManager
{
public:
  PathManager();

  void cast(const Point2f &pos, int type);

  void update(long dt);
  void render(ep::IGraphics *graphics);

private:
  std::vector<StarPath> m_stars;
  int m_starMarker;
  int m_type;
};

#endif //__FX_PATH_H__
