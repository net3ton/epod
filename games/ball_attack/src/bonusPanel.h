#ifndef __BONUS_PANEL_H__
#define __BONUS_PANEL_H__

#include <vector>

namespace ep { class IGraphics; }
class IBonus;


class BonusPanel
{
public:
  BonusPanel();

  void addBonus(IBonus *bonus);
  void deleteBonus(IBonus *bonus);

  void update(long dt);
  void render(ep::IGraphics *graphics);

private:
  int findBonus(IBonus *bonus);

  struct BonusIcon
  {
    IBonus *bonus;
    float left;
  };

  std::vector<BonusIcon> m_panel;
};


#endif //__BONUS_PANEL_H__
 