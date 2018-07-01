#include <epod_threads.h>
#include <windows.h>

namespace ep
{

void *create_thread(void *(*proc)(void *), void *arg)
{
  LPTHREAD_START_ROUTINE callback = reinterpret_cast<LPTHREAD_START_ROUTINE>(proc);
  HANDLE thread = CreateThread(NULL, 0, callback, arg, 0, NULL);
  return (void *) thread;
}


void end_thread(void *thread)
{
  WaitForSingleObject((HANDLE) thread, INFINITE);
  CloseHandle((HANDLE) thread);
}

}
