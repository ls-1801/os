#include "static_linked_list.h"
#include <gtest/gtest.h>

MK_LIST(test_list, uint32_t, 32);

TEST(TestStaticLinkedList, TestAllocation) {

  //[32]
  uint32_t input = 32;
  sll_insert_front(test_list, &input);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 32);

  //[31, 32]
  input = 31;
  sll_insert_front(test_list, &input);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 31);

  //[32, 31]
  sll_rotate(test_list);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 32);

  //[31, 32, 31]
  input = 31;
  sll_insert_front(test_list, &input);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 31);

  //[30, 31, 32, 31]
  input = 30;
  sll_insert_front(test_list, &input);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 30);

  //[31, 32, 31, 30]
  sll_rotate(test_list);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 31);

  //[32, 31, 30, 31]
  sll_rotate(test_list);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 32);

  //[31, 30, 31, 32]
  sll_rotate(test_list);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 31);

  //[31, 30, 31, 32, 20]
  input = 20;
  sll_insert_back(test_list, &input);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 31);

  sll_rotate(test_list);
  sll_rotate(test_list);
  sll_rotate(test_list);
  sll_rotate(test_list);
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 20);

  input = 99;
  sll_pop_first(test_list, &input);
  EXPECT_EQ(20, input);

  uint32_t *ptr = (uint32_t *)sll_get_first(test_list);
  EXPECT_EQ(*ptr, 31);
  *ptr = 99;
  EXPECT_EQ(*(uint32_t *)sll_get_first(test_list), 99);
  input = 20;
  sll_pop_first(test_list, &input);
  EXPECT_EQ(99, input);

  // sll_rotate();
  // EXPECT_EQ(sll_get_first(), 30);
  // sll_rotate();
  // EXPECT_EQ(sll_get_first(), 31);
  // sll_rotate();
  // EXPECT_EQ(sll_get_first(), 32);
  // //[32, 30, 31]

  // sll_insert_back(33);
  // EXPECT_EQ(test_list->first, 0);
  // EXPECT_EQ(sll_list_length, 4);
  // EXPECT_EQ(*test_list->last->data, 3);
  // //[ 32, 30, 31, 33 ]
  // sll_rotate();
  // sll_rotate();
  // sll_rotate();

  // EXPECT_EQ(sll_get_first(), 33);
  // EXPECT_EQ(sll_pop_first(), 33);
  // //[32, 30, 31]

  // EXPECT_EQ(sll_list_length, 3);
  // EXPECT_EQ(sll_get_first(), 32);

  // sll_rotate();
  // EXPECT_EQ(sll_get_first(), 30);
  // sll_rotate();
  // EXPECT_EQ(sll_get_first(), 31);
  // sll_rotate();
  // EXPECT_EQ(sll_get_first(), 32);

  // EXPECT_EQ(sll_pop_first(), 32);
  // EXPECT_EQ(sll_pop_first(), 30);
  // EXPECT_EQ(sll_pop_first(), 31);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}