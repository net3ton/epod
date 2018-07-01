#ifndef __FX_FRACTION_H__
#define __FX_FRACTION_H__

#include <math/point2.h>
#include <vector>
#include <timer_int.h>

namespace ep { class IGraphics; }
namespace ep { class Tile; }


class Fraction
{
public:
  Fraction();

  void init(const ep::Tile *tile, const Point2f &pos, const Point2f &dir, int time);

  void update(long dt);
  void render(ep::IGraphics *graphics);

private:
  const ep::Tile *m_tile;
  int m_color;
  Point2f m_pos, m_dir;
  TimerInt m_timer;
};


class FractionManager
{
public:
  FractionManager();

  void cast(const ep::Tile *tile, const Point2f &pos, const Point2f &dir);

  void update(long dt);
  void render(ep::IGraphics *graphics);

private:
  std::vector<Fraction> parts;
  int partMarker;
};

#endif //__FX_FRACTION_H__
