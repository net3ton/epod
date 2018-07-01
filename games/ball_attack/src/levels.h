#ifndef __LEVELS_H__
#define __LEVELS_H__

class LevelBase;


namespace ba
{

LevelBase *level();

void start_level(int id);
void destroy_level(LevelBase *level);

void restart_level();
void start_next_level();

void complete_level(int id);

void start_survival();

} // namespace ba

#endif //__LEVELS_H__
