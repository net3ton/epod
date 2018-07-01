#ifndef __FX_EFFECTS_H__
#define __FX_EFFECTS_H__

#include <math/point2.h>
#include <vector>

namespace ep { class Tile; }

namespace fx
{

//void cast_spark(const Point2f &pos, const Point2f &dir);
void cast_fraction(const ep::Tile *tile, const Point2f &pos, const Point2f &dir);
void cast_path(const Point2f &pos, int type);

} // namespace fx

#endif //__FX_EFFECTS_H__
