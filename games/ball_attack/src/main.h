#ifndef __MAIN_H__
#define __MAIN_H__

class BaseScene;


namespace ba
{

bool is_cheats();

void set_scene(BaseScene *scene);
void move_to_scene(BaseScene *scene, bool to_right);

void show_sky(bool show);
float get_move_progress();

} // namespace ba

#endif //__MAIN_H__
