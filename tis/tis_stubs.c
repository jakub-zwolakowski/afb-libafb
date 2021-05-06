
int json_c_get_random_seed(void)
{
  return 42;
}

int __sync_val_compare_and_swap_int32_t(int *p, int a, int b, ...)
{
  return __sync_bool_compare_and_swap_uint32_t(p, a, b);
  ;
}

#include <setjmp.h>
int setjmp(jmp_buf env)
{
  return 0;
}

int isatty(int fd)
{
  return 0;
}

#include <time.h>

// // Stub included thanks to TIS_DETERMINISTIC_LIBC
//
// time_t time(time_t *tloc)
// {
//   return 99;
// }

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
  tp->tv_sec = 42;
  tp->tv_nsec = 42;
  return 0;
}

#include <unistd.h>

pid_t getpid(void)
{
  return 42;
}

void srand(unsigned int seed)
{
  return;
}

int rand(void)
{
  return 137;
}

#include <sys/uio.h>

#if 0
// Simpliest of stubs
ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
  return 42;
}
#else
// Naive stub
ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
  ssize_t written_total = 0;
  int i;

  for (i = 0; i < iovcnt; i++)
  {
    ssize_t written_now;

    written_now = write(fd, iov[i].iov_base, iov[i].iov_len);
    if (written_now == -1)
    {
      written_total = -1;
      break;
    }
    written_total += written_now;
  }

  return written_total;
}
#endif

#include <stdlib.h>
#include <stdio.h>

char *secure_getenv(const char *name)
{
  printf("STUB CALL secure_getenv(%s)\n", name);
  return NULL;
}

#include <dlfcn.h>

void *dlopen(const char *filename, int flag)
{
  return NULL;
}

char *dlerror(void)
{
  return "STUB ERROR FROM dlerror";
}

// void *dlsym(void *handle, const char *symbol);

#include <pthread.h>

int pthread_attr_init(pthread_attr_t *attr)
{
  return 0;
}

// int pthread_attr_destroy(pthread_attr_t *attr);

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
  return 0;
}

// int pthread_attr_getdetachstate(const pthread_attr_t *attr,
//                                 int *detachstate);

#include <sys/socket.h>

int socketpair(int domain, int type, int protocol, int sv[2])
{
  return 0;
}

#include <sys/epoll.h>

int epoll_create(int size)
{
  return 42;
}

int epoll_create1(int flags)
{
  return 42;
}

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
  return 0;
}

int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout)
{
  return 1;
}

// int epoll_pwait(int epfd, struct epoll_event *events,
//                 int maxevents, int timeout,
//                 const sigset_t *sigmask);

// int epoll_pwait2(int epfd, struct epoll_event *events,
//                  int maxevents, const struct timespec *timeout,
//                  const sigset_t *sigmask);