#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#define t_assert(message, test)                                                \
  do {                                                                         \
    if (!(test)) {                                                             \
      failed_tests++;                                                          \
      return message;                                                          \
    }                                                                          \
  } while (0)
#define run_test(test)                                                         \
  do {                                                                         \
    const char *message = test();                                              \
    tests_run++;                                                               \
    if (message)                                                               \
      return message;                                                          \
  } while (0)
extern int tests_run;
extern int failed_tests;
void run_all_tests();

#endif /*TEST_RUNNER_H*/
