#ifndef _EPOD_THREADS_H_
#define _EPOD_THREADS_H_


namespace ep
{

void *create_thread(void *(*proc)(void *), void *arg);
void end_thread(void *thread);

} // namespace ep

#endif //_EPOD_THREADS_H_
