#include "test_runner.h"
#include "../lib/ds/ringbuffer.h"
#include "../lib/io.h"
#include "../lib/strings.h"
#include <common.h>
#include <limits.h>

int tests_run = 0;
int failed_tests = 0;

const char *strncmp_test() {
  t_assert("strcmp1 failed", strncmp("TEST1", "TEST", 7) == 1);
  t_assert("strcmp2 failed", strncmp("TEST", "TEST1", 7) == -1);
  t_assert("strcmp3 failed", strncmp("TEST", "TEST", 7) == 0);

  return NULL;
}

const char *atoi_test() {
  t_assert("atoi dec failed int max", INT_MAX == atoi_dec("2147483647"));
  t_assert("atoi dec failed int min + 1",
           INT_MIN + 1 == atoi_dec("-2147483647"));
  t_assert("atou dec failed uint max", UINT_MAX == atou_dec("4294967295"));
  t_assert("atou hex failed uint max lowercase",
           UINT_MAX == atou_hex("ffffffff"));
  t_assert("atou hex failed uint max uppercase",
           UINT_MAX == atou_hex("FFFFFFFF"));
  t_assert("atou hex failed uint max", UINT_MAX == atou_hex("0xFFFFFFFF"));
  t_assert("atou hex failed uint max", UINT_MAX == atou_hex("0XFFFFFFFF"));
  t_assert("atou failed uint min", 0 == atou_dec("0"));
  t_assert("atou hex failed uint min", 0 == atou_hex("0"));
  t_assert("atou hex failed uint min", 0 == atou_hex("0x0"));
  t_assert("atou hex failed uint min", 0 == atou_hex("0X0"));

  return NULL;
}

const char *ring_buffer_test() {
  char buf[10];
  struct Buffer rBuf = {buf, buf, buf, 0, 10};
  t_assert("buffer should be empty", buffer_empty(&rBuf));
  buffer_put(&rBuf, 'c');
  t_assert("buffer should not be empty", !buffer_empty(&rBuf));
  t_assert("buffer first should be a c", buffer_first(&rBuf) == 'c');
  t_assert("buffer should still not be empty after buffer_first",
           !buffer_empty(&rBuf));
  char c;
  buffer_get(&rBuf, &c, 1);
  t_assert("should retrieve c from buffer", c == 'c');
  t_assert("buffer should be empty now", buffer_empty(&rBuf));

  char *str = "0123456789";
  while (*str != '\0') {
    buffer_put(&rBuf, *str++);
  }

  t_assert("buffer first should be a 0", buffer_first(&rBuf) == '0');

  char tenCharBuf[11];
  buffer_get(&rBuf, tenCharBuf, 10);
  tenCharBuf[10] = '\0';
  t_assert("buffer should contain 0123456789",
           strncmp(tenCharBuf, "0123456789", 10) == 0);

  return NULL;
}

const char *itoa_test() {
  char buf[50];
  t_assert("itoa_dec int max failed",
           strncmp("2147483647", itoa(INT_MAX, buf, 10), 20) == 0);
  t_assert("itoa_dec int min failed",
           strncmp("-2147483648", itoa(INT_MIN, buf, 10), 20) == 0);

  t_assert("utoa_dec uint max failed",
           strncmp("4294967295", utoa(UINT_MAX, buf, 10), 20) == 0);
  t_assert("utoa_dec uint min failed", strncmp("0", utoa(0, buf, 10), 20) == 0);

  t_assert("utoa_hex uint max failed",
           strncmp("ffffffff", utoa(UINT_MAX, buf, 16), 20) == 0);
  t_assert("utoa_hex uint min failed", strncmp("0", utoa(0, buf, 16), 20) == 0);

  t_assert("utoa_bin uint max failed",
           strncmp("11111111111111111111111111111111", utoa(UINT_MAX, buf, 2),
                   20) == 0);
  t_assert("utoa_bin uint min failed", strncmp("0", utoa(0, buf, 2), 20) == 0);

  return NULL;
}

const char *test_runner() {
  run_test(strncmp_test);
  run_test(itoa_test);
  run_test(atoi_test);
  run_test(ring_buffer_test);
  return NULL;
}

void run_all_tests() {
  kprintf("\033[1;32mRunning Tests...\033[0m\n");

  const char *tst_result = test_runner();

  if (tst_result) {
    kprintf("\033[1;31mTest Failed: %s \033[0m\n", tst_result);
  } else {
    kprintf("\033[1;32mAll Tests passed.\033[0m\n");
  }
}