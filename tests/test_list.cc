#include "list.h"

#include "gtest/gtest.h"

TEST(ListTest, CreateEmpty) {
  struct list l;

  list_create(&l);

  EXPECT_TRUE(list_is_empty(&l));
  EXPECT_TRUE(list_size(&l) == 0);

  list_destroy(&l);
}

TEST(ListTest, CreateOneElement) {
  struct list l;

  list_create(&l);
  list_add_back(&l, 1);

  EXPECT_FALSE(list_is_empty(&l));
  EXPECT_TRUE(list_size(&l) == 1);

  int *ptr = list_get(&l, 0);

  EXPECT_TRUE(ptr != NULL);

  if (ptr) {
    EXPECT_EQ(*ptr, 1);
  }

  list_destroy(&l);
}

#define BIG_SIZE 1000

TEST(ListTest, CreateManyElements) {
  struct list l;

  list_create(&l);

  for (int i = 0; i < BIG_SIZE; ++i) {
    list_add_back(&l, i);
  }

  EXPECT_FALSE(list_is_empty(&l));
  EXPECT_TRUE(list_size(&l) == BIG_SIZE);

  int *ptr = list_get(&l, 0);

  EXPECT_TRUE(ptr != NULL);

  if (ptr) {
    EXPECT_EQ(*ptr, 0);
  }

  list_destroy(&l);
}

TEST(ListTest, Search) {
  struct list l;

  list_create(&l);

  for (int i = 0; i < 1000; ++i) {
    list_add_back(&l, i);
  }

  size_t index;

  for (int i = 0; i < 1000; ++i) {
    index = list_search(&l, i);
    EXPECT_EQ(index, static_cast<size_t>(i));
  }

  index = list_search(&l, -1);
  EXPECT_TRUE(index == list_size(&l));

  list_destroy(&l);
}

TEST(ListTest, Insert) {
  struct list l;

  list_create(&l);

  list_add_back(&l, 0);
  list_add_back(&l, 1);
  list_add_back(&l, 2);

  list_insert(&l, 3, 0); // insert at the beginning

  EXPECT_TRUE(list_size(&l) == 4);

  static const int expected1[] = { 3, 0, 1, 2 };

  EXPECT_TRUE(list_equals(&l, expected1, 4));

  list_insert(&l, 4, 2); // insert in  the middle

  EXPECT_TRUE(list_size(&l) == 5);

  static const int expected2[] = { 3, 0, 4, 1, 2 };

  EXPECT_TRUE(list_equals(&l, expected2, 5));

  list_insert(&l, 5, 5); // insert at the end

  EXPECT_TRUE(list_size(&l) == 6);

  static const int expected3[] = { 3, 0, 4, 1, 2, 5 };

  EXPECT_TRUE(list_equals(&l, expected3, 6));

  list_destroy(&l);
}

TEST(ListTest, Remove) {
  struct list l;

  list_create(&l);

  list_add_back(&l, 0);
  list_add_back(&l, 1);
  list_add_back(&l, 2);
  list_add_back(&l, 3);
  list_add_back(&l, 4);
  list_add_back(&l, 5);

  list_remove(&l, 0); // remove at the beginning

  EXPECT_TRUE(list_size(&l) == 5);

  static const int expected1[] = { 1, 2, 3, 4, 5 };

  EXPECT_TRUE(list_equals(&l, expected1, 5));

  list_remove(&l, 2); // remove in  the middle

  EXPECT_TRUE(list_size(&l) == 4);

  static const int expected2[] = { 1, 2, 4, 5 };

  EXPECT_TRUE(list_equals(&l, expected2, 4));

  list_remove(&l, 3); // remove at the end

  EXPECT_TRUE(list_size(&l) == 3);

  static const int expected3[] = { 1, 2, 4 };

  EXPECT_TRUE(list_equals(&l, expected3, 3));

  list_remove(&l, 0);
  list_remove(&l, 0);
  list_remove(&l, 0);

  EXPECT_TRUE(list_is_empty(&l));

  list_destroy(&l);
}

TEST(ListTest, Import) {
  struct list l;

  list_create(&l);

  static const int origin1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

  list_import(&l, origin1, 9);
  EXPECT_TRUE(list_equals(&l, origin1, 9));

  static const int origin2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

  list_import(&l, origin2, 12); // grow
  EXPECT_TRUE(list_equals(&l, origin2, 12));

  static const int origin3[] = { 0, 1, 2, 3, 4 };

  list_import(&l, origin3, 5); // shrink
  EXPECT_TRUE(list_equals(&l, origin3, 5));

  list_destroy(&l);
}

TEST(ListTest, IsSorted) {
  struct list l;

  list_create(&l);

  EXPECT_TRUE(list_is_sorted(&l));

  static const int origin1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

  list_import(&l, origin1, 9);
  EXPECT_TRUE(list_is_sorted(&l));

  static const int origin2[] = { 8, 0, 1, 2, 3, 4, 5, 6, 7 };

  list_import(&l, origin2, 9);
  EXPECT_FALSE(list_is_sorted(&l));

  static const int origin3[] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };

  list_import(&l, origin3, 9);
  EXPECT_FALSE(list_is_sorted(&l));

  static const int origin4[] = { 0, 1, 2, 3, 5, 4, 6, 7, 8 };

  list_import(&l, origin4, 9);
  EXPECT_FALSE(list_is_sorted(&l));

  list_destroy(&l);
}

TEST(ListTest, MergeSort) {
  struct list l;

  list_create(&l);

  static const int unsorted[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

  list_import(&l, unsorted, 11);
  list_merge_sort(&l);
  EXPECT_TRUE(list_is_sorted(&l));

  static const int modified[] = { 8, 4, 1, 6, 10, 3, 0, 9, 5, 2, 7 };

  list_import(&l, modified, 11);
  list_merge_sort(&l);
  EXPECT_TRUE(list_is_sorted(&l));

  static const int sorted[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  list_import(&l, sorted, 11);
  list_merge_sort(&l);
  EXPECT_TRUE(list_is_sorted(&l));

  list_destroy(&l);
}
