#ifndef __FX_EXPLOSION_H__
#define __FX_EXPLOSION_H__

#include <math/point2.h>
#include <vector>
#include <timer_int.h>

namespace ep { class IGraphics; }
namespace ep { class Tile; }


class ExploPart
{
public:
  ExploPart();

  void init(const Point2f &pos, const Point2f &dir);

  void update(long dt);
  void render(ep::IGraphics *graphics);

private:
  int m_color;
  float m_speed;
  Point2f m_pos, m_dir;
  TimerInt m_timer;
};


class ExploManager
{
public:
  ExploManager();

  void cast(const Point2f &pos);

  void update(long dt);
  void render(ep::IGraphics *graphics);

private:
  std::vector<ExploPart> m_parts;
  int m_partMarker;
};

#endif //__FX_EXPLOSION_H__
