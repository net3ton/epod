#include <epod_threads.h>
#include <pthread.h>

namespace ep
{

void *create_thread(void *(*proc)(void *), void *arg)
{
  pthread_t *thread = new pthread_t();

  if (pthread_create(thread, NULL, proc, arg))
  {
    delete thread;
    return NULL;
  }

  return (void *)thread;
}


void end_thread(void *thread)
{
  pthread_t *t = (pthread_t *)thread;
  void *ret = NULL;

  pthread_join(*t, &ret);
  delete t;
}

}
