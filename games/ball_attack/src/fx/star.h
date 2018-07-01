#ifndef __FX_STAR_H__
#define __FX_STAR_H__

#include <math/point2.h>
#include <vector>
#include <timer_int.h>

namespace ep { class IGraphics; }
namespace ep { class Tile; }


class Star
{
public:
  Star();

  void init(const Point2f &pos, int color, int period);

  void update(long dt);
  void render(ep::IGraphics *graph);

private:
  bool m_dead;
  int m_color;
  Point2f m_pos;
  TimerInt m_timer;
};


class StarManager
{
public:
  StarManager();

  void cast(const Point2f &pos);

  void update(long dt);
  void render(ep::IGraphics *graph);

private:
  std::vector<Star> stars;
  int starMarker;
};

#endif //__FX_STAR_H__
