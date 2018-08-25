#include "stdafx.h"
#include <process.h>
#include "Thread.h"


/*

CloseHandle:
在CreateThread成功之后会返回一个hThread的handle，
且内核对象的计数加1，CloseHandle之后，引用计数减1，当变为0时，系统删除内核对象。
但是这个handle并不能完全代表这个线程，它仅仅是线程的一个“标识”，
系统和用户可以利用它对相应的线程进行必要的操纵。
如果在线程成功创建后，不再需要用到这个句柄，就可以在创建成功后，线程退出前直接CloseHandle掉，但这并不会影响到线程的运行.
不执行CloseHandle() 带来的后果:将会造成内核对象的泄露，相当与句柄泄露，但不同于内存泄露， 这势必会对系统的效率带来一定程度上的负面影响。


BOOL TerminateThread( HANDLE hThread, DWORD dwExitCode);
作用：
在线程外终止一个线程，用于强制终止线程。
参数说明：
HANDLE htread：被终止的线程的句柄，为CWinThread指针。
DWORD dwExitCode：退出码。
返回值：
函数执行成功则返回非零值，执行失败返回0。调用getlasterror获得返回的值。


*/

unsigned int __stdcall threadFunction(void * object)
{
   Thread * thread = (Thread *) object;
   return (unsigned int ) thread->run(thread->param);
}

Thread::Thread()
{
   started = false;
   detached = false;
}

Thread::~Thread()
{
   stop();
}

int Thread::start(void* pra)
{
   if (!started)
   {
	   param = pra;
	   if (threadHandle = (HANDLE)_beginthreadex(NULL, 0, threadFunction, this, 0, &threadID))
	   {
		   if (detached)
		   {
		   CloseHandle(threadHandle);
		   }
		   started = true;
	   }
   }
   return started;
}

//wait for current thread to end.
void * Thread::join()
{
   DWORD status = (DWORD) NULL;
   if (started && !detached)
   {
   WaitForSingleObject(threadHandle, INFINITE);
   GetExitCodeThread(threadHandle, &status); 
   CloseHandle(threadHandle);
   detached = true;
   }

   return (void *)status;
}

void Thread::detach()
{
   if (started && !detached)
   {
   CloseHandle(threadHandle);
   }
   detached = true;
}

void Thread::stop()
{
   if (started && !detached)
   {
   TerminateThread(threadHandle, 0);
   CloseHandle(threadHandle);
   detached = true;
   }
}

void Thread::sleep(unsigned int delay)
{
   ::Sleep(delay);
}
