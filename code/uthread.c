#include "types.h"
#include "stat.h"
#include "user.h"

#define THREAD_STATE_FREE    0x0
#define THREAD_STATE_RUNNING 0x1
#define THREAD_STATE_RUNNABLE 0x2

#define THREAD_STACK_SIZE  8192
#define MAX_THREADS  4

typedef struct {
  int sp;
  char stack[THREAD_STACK_SIZE];
  int state;
} Thread;

static Thread allThreads[MAX_THREADS];
Thread *currentThread;
Thread *nextThread;

extern void threadSwitch(void);

void initThreads(void) {
  currentThread = &allThreads[0];
  currentThread->state = THREAD_STATE_RUNNING;
}

static void scheduleThread(void) {
  nextThread = 0;

  for (Thread *t = allThreads; t < allThreads + MAX_THREADS; t++) {
    if (t->state == THREAD_STATE_RUNNABLE && t != currentThread) {
      nextThread = t;
      break;
    }
  }

  if (nextThread == 0 && currentThread->state == THREAD_STATE_RUNNABLE) {
    nextThread = currentThread;
  }

  if (nextThread == 0) {
    printf(2, "scheduleThread: no runnable threads\n");
    exit();
  }

  if (currentThread != nextThread) { /* switch threads? */
    nextThread->state = THREAD_STATE_RUNNING;
    threadSwitch();
  } else {
    nextThread = 0;
  }
}

void createThread(void (*func)()) {
  Thread *newThread;

  for (newThread = allThreads; newThread < allThreads + MAX_THREADS; newThread++) {
    if (newThread->state == THREAD_STATE_FREE) {
      break;
    }
  }

  newThread->sp = (int)(newThread->stack + THREAD_STACK_SIZE); 
  newThread->sp -= 4;                                         
  *((int *)(newThread->sp)) = (int)func;                      
  newThread->sp -= 32;                                        
  newThread->state = THREAD_STATE_RUNNABLE;
}

void yieldThread(void) {
  currentThread->state = THREAD_STATE_RUNNABLE;
  scheduleThread();
}

static void threadFunction(void) {
  int i;

  printf(1, "thread running\n");

  for (i = 0; i < 100; i++) {
    printf(1, "thread 0x%x\n", (int)currentThread);
    yieldThread();
  }

  printf(1, "thread: exit\n");
  currentThread->state = THREAD_STATE_FREE;
  scheduleThread();
}

int main(int argc, char *argv[]) {
  initThreads();
  createThread(threadFunction);
  createThread(threadFunction);
  scheduleThread();

  return 0;
}
