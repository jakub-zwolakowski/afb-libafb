
int json_c_get_random_seed(void) {
  return 42;
}

int __sync_val_compare_and_swap_int32_t (int * p, int a, int b, ...) {
  return __sync_bool_compare_and_swap_uint32_t (p, a, b);;
}

#include <setjmp.h>
int setjmp(jmp_buf env) {
  return 0;
}
