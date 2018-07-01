#ifndef __PROFILE_H__
#define __PROFILE_H__


namespace pf
{

void load();
void save();
void reset();

void cheat_open_levels(int count);

int get_level_last();
void set_level_last(int id);

int get_level_stars(int id);
void set_level_stars(int id, int stars);

void set_levels_hard(bool hard);
bool is_levels_hard();

bool is_music();
bool is_sound();

void set_music(bool enabled);
void set_sound(bool enabled);

bool is_levels_bought();
void buy_levels();

}; // namespace pf


#endif //__PROFILE_H__
