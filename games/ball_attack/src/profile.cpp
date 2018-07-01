#include "profile.h"

#include <epod.h>
#include <epod_fsystem.h>
#include <epod_sound.h>
#include <vector>

enum
{
  VER_1 = 1,

  VER_LAST = VER_1,
};

typedef std::vector<int> LevelStars;

const char *PROFILE_PATH = "user/profile.data";

bool music_enabled = true;
bool sound_enabled = true;

bool levels_hard = false;

LevelStars level_stars;
LevelStars level_hard_stars;
int level_last = 0;
int level_hard_last = 0;

bool levels_bought = false;  // TODO: Secure holding


void write_vec_int(ep::File &file, const std::vector<int> &vec)
{
  file.writeInt(vec.size());
  for (int i=0; i<vec.size(); ++i)
    file.writeInt(vec[i]);
}

void read_vec_int(ep::File &file, std::vector<int> &vec)
{
  int size = file.readInt();
  vec.resize(size);
  for (int i=0; i<vec.size(); ++i)
    vec[i] = file.readInt();
}


namespace pf
{

void load()
{
  ep::File p(PROFILE_PATH, ep::FILE_MODE_READ);
  if (!p.opened())
  {
    ep::log("can't open profile!");
    return;
  }

  int sig = p.readInt();
  if (sig > VER_LAST)
  {
    ep::log("wrong profile signature!");
    return;
  }

  if (sig < VER_LAST)
    ep::log("obsolete profile format(%d)!", sig);

  music_enabled = p.readInt();
  sound_enabled = p.readInt();

  ep::snd::set_volume_music(music_enabled ? 1.f : 0.f);
  ep::snd::set_volume_sound(sound_enabled ? 1.f : 0.f);

  levels_bought = p.readInt();
  levels_hard = p.readInt();

  level_last = p.readInt();
  ::read_vec_int(p, level_stars);

  level_hard_last = p.readInt();
  ::read_vec_int(p, level_hard_stars);
}


bool ensure_dir = false;

void save()
{
  if (!ensure_dir)
    ep::make_dirs(PROFILE_PATH);

  ep::File p(PROFILE_PATH, ep::FILE_MODE_WRITE);
  if (!p.opened())
  {
    ep::log("profile saving failed!");
    return;
  }

  ensure_dir = true;

  p.writeInt(VER_LAST);

  p.writeInt(music_enabled);
  p.writeInt(sound_enabled);
  p.writeInt(levels_bought);
  p.writeInt(levels_hard);

  p.writeInt(level_last);
  ::write_vec_int(p, level_stars);

  p.writeInt(level_hard_last);
  ::write_vec_int(p, level_hard_stars);
}


void reset()
{
  level_stars.clear();
  level_hard_stars.clear();

  level_last = 0;
  level_hard_last = 0;

  levels_hard = false;
  levels_bought = false;

  save();
}


bool is_music() { return music_enabled; }
bool is_sound() { return sound_enabled; }

void set_music(bool enabled)
{
  ep::snd::set_volume_music(enabled ? 1.f : 0.f);
  music_enabled = enabled;
  save();
}

void set_sound(bool enabled)
{
  ep::snd::set_volume_sound(enabled ? 1.f : 0.f);
  sound_enabled = enabled;
  save();
}


void set_levels_hard(bool hard) { levels_hard = hard; }
bool is_levels_hard() { return levels_hard; }


void cheat_open_levels(int count)
{
  int &last = levels_hard ? level_hard_last : level_last;
  last = count - 1;

  LevelStars &stars_mas = levels_hard ? level_hard_stars : level_stars;
  stars_mas.resize(count);

  for (int i=0; i<stars_mas.size(); ++i)
    stars_mas[i] = rand() % 3 + 1;

  save();
}


int get_level_last()
{
  return levels_hard ? level_hard_last : level_last;
}

void set_level_last(int id)
{
  int &last = levels_hard ? level_hard_last : level_last;

  if (id <= last)
    return;

  last = id;
  save();
}


int get_level_stars(int id)
{
  LevelStars &stars_mas = levels_hard ? level_hard_stars : level_stars;

  if (id < stars_mas.size())
    return stars_mas[id];

  return 0;
}

void set_level_stars(int id, int stars)
{
  LevelStars &stars_mas = levels_hard ? level_hard_stars : level_stars;

  if (id >= stars_mas.size())
    stars_mas.resize(id + 1, 0);

  if (stars_mas[id] > stars)
    return;

  stars_mas[id] = stars;
  save();
}


bool is_levels_bought() { return levels_bought; }
void buy_levels() { levels_bought = true; }

}; // namespace pf
