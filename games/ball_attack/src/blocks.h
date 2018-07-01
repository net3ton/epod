#ifndef __BLOCKS_H__
#define __BLOCKS_H__

#include <math/point2.h>

class Aabb;
namespace ep { class IGraphics; }
namespace ep { class Tile; }

/// Back style
enum
{
  PANEL_NONE = 0,

  PANEL_A,
  PANEL_B,
  PANEL_C,
  PANEL_D,
  PANEL_E,

  PANEL_ROUNDED_A = 20,
  PANEL_ROUNDED_B,
  PANEL_ROUNDED_C,
  PANEL_ROUNDED_D,
  PANEL_ROUNDED_E,
};

/// Blocks types
enum
{
  BLOCK_NONE = 0,

  BLOCK_SIMPLE = 1,
  BLOCK_STATIC,
  BLOCK_BONUS,
  BLOCK_EXPLO,
  BLOCK_DOUBLE,
  BLOCK_TRIPLE,
  BLOCK_MAGNET_IN,
  BLOCK_MAGNET_OUT,

  BLOCK_DYNAMIC = 30,

  BLOCK_ENEMY = 100,
};


class IBlockBase
{
public:
  IBlockBase();
  virtual ~IBlockBase() {}

  virtual void init(int x, int y) = 0;
  virtual void reset() = 0;
  virtual void update(long dt) = 0;
  virtual void render(ep::IGraphics *graphics) = 0;

  virtual void showAnim() = 0;

  virtual bool testCollide(const Aabb &b) = 0;
  virtual bool sweepCollide(const Point2f &p, const Point2f &dir, float r, float &tmin, Point2f &n) = 0;

  virtual void collide(const Point2f &from, bool power) = 0;

  virtual bool isDone() const = 0;  // for level end check
  virtual bool isDead() const = 0;  // for collision/update check
  virtual bool isBreakable() const = 0;

  virtual Point2f getPos() = 0; // center
  virtual const Aabb &getBbox() = 0;

  virtual void move(const Point2f &dir) = 0;

  ///
  virtual void playKnock() = 0;

  /// for editor simplify
  void setId(int id) { m_id = id; }
  int getId() const { return m_id; }

  /// back style
  void setBack(int type);
  const ep::Tile *getBaseTile() { return m_baseTile; }

private:
  int m_id;
  const ep::Tile *m_baseTile;
};


IBlockBase *create_block(int type);
IBlockBase *put_block(int type, int x, int y);

void destroy_block(IBlockBase *block);

#endif //__BLOCKS_H__
 