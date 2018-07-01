#ifndef __BONUS_MANAGER_H__
#define __BONUS_MANAGER_H__

#include <math/point2.h>
#include <vector>

namespace ep { class IGraphics; }
namespace ep { class Tile; }
class IBonus;


const ep::Tile *get_bonus_drop_tile(int id);
const char *get_bonus_descr(int id);

struct BonusDrop
{
  BonusDrop():dead(true) {}

  int id;
  const ep::Tile *icon;
  Point2f pos;
  bool dead;
};


class BonusManager
{
public:
  BonusManager();

  void drop(int id, int x, int y);
  void dropRandom(int x, int y);
  bool endBonus(int id);

  void reset();

  void update(long dt);
  void render(ep::IGraphics *graphics);

private:
  const float DROP_SPEED;
  const float WIDTH2;

  bool checkCollision(int x, int y);

  std::vector<BonusDrop> m_drops;
  int m_dropIndex;

  std::vector<IBonus*> m_bonuses;

  IBonus *getBonus(int id);
};

#endif //__BONUS_MANAGER_H__
 