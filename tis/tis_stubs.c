
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

// // Stub included thanks to TIS_DETERMINISTIC_LIBC
//
// int rand(void)
// {
//   return 137;
// }

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

char *secure_getenv(const char *name) {
  printf("secure_getenv(%s)\n", name);
  return NULL;
}