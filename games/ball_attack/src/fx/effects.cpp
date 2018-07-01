#include "effects.h"
#include "../levelBase.h"
#include "../levels.h"


namespace fx
{

//void cast_spark(const Point2f &pos, const Point2f &dir)
//{
//  ba::level()->fxCastSpark(pos, dir);
//}

void cast_fraction(const ep::Tile *tile, const Point2f &pos, const Point2f &dir)
{
  ba::level()->fxCastFraction(tile, pos, dir);
}

void cast_path(const Point2f &pos, int type)
{
  ba::level()->fxCastPath(pos, type);
}

} // namespace fx
