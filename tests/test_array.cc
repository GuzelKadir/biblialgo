#include "array.h"

#include "gtest/gtest.h"

TEST(ArrayTest, CreateEmpty) {
  struct array a;

  array_create(&a);

  EXPECT_TRUE(array_is_empty(&a));
  EXPECT_TRUE(array_size(&a) == 0);

  int *ptr = array_get(&a, 0);

  EXPECT_TRUE(ptr == NULL);

  array_destroy(&a);
}

TEST(ArrayTest, CreateOneElement) {
  struct array a;

  array_create(&a);
  array_add(&a, 1);

  EXPECT_FALSE(array_is_empty(&a));
  EXPECT_TRUE(array_size(&a) == 1);

  int *ptr = array_get(&a, 0);

  EXPECT_TRUE(ptr != NULL);

  if (ptr) {
    EXPECT_EQ(*ptr, 1);
  }

  array_destroy(&a);
}

#define BIG_SIZE 1000

TEST(ArrayTest, CreateManyElements) {
  struct array a;

  array_create(&a);

  for (int i = 0; i < BIG_SIZE; ++i) {
    array_add(&a, i);
  }

  EXPECT_FALSE(array_is_empty(&a));
  
  EXPECT_TRUE(array_size(&a) == BIG_SIZE);

  int *ptr = array_get(&a, 0);

  EXPECT_TRUE(ptr != NULL);

  if (ptr) {
    EXPECT_EQ(*ptr, 0);
  }

  array_destroy(&a);
}

TEST(ArrayTest, Search) {
  struct array a;

  array_create(&a);

  for (int i = 0; i < 1000; ++i) {
    array_add(&a, i);
  }

  size_t index;

  for (int i = 0; i < 1000; ++i) {
    index = array_search(&a, i);
    EXPECT_EQ(index, static_cast<size_t>(i));
  }

  index = array_search(&a, -1);
  EXPECT_TRUE(index == array_size(&a));

  array_destroy(&a);
}

TEST(ArrayTest, SearchSorted) {
  struct array a;

  array_create(&a);

  for (int i = 0; i < 1000; ++i) {
    array_add(&a, 2*i);
  }

  size_t index;

  for (int i = 0; i < 1000; ++i) {
    index = array_search_sorted(&a, 2*i);
    EXPECT_EQ(index, static_cast<size_t>(i));
  }

  index = array_search_sorted(&a, -1); // before everything
  EXPECT_TRUE(index == array_size(&a));

  index = array_search_sorted(&a, 1001); // in the middle of other elements
  EXPECT_TRUE(index == array_size(&a));

  index = array_search_sorted(&a, 2001); // after everything
  EXPECT_TRUE(index == array_size(&a));

  array_destroy(&a);
}

TEST(ArrayTest, Insert) {
  struct array a;

  array_create(&a);

  array_add(&a, 0);
  array_add(&a, 1);
  array_add(&a, 2);

  array_insert(&a, 3, 0); // insert at the beginning

  EXPECT_TRUE(array_size(&a) == 4);

  static const int expected1[] = { 3, 0, 1, 2 };

  EXPECT_TRUE(array_equals(&a, expected1, 4));

  array_insert(&a, 4, 2); // insert in  the middle

  EXPECT_TRUE(array_size(&a) == 5);

  static const int expected2[] = { 3, 0, 4, 1, 2 };

  EXPECT_TRUE(array_equals(&a, expected2, 5));

  array_insert(&a, 5, 5); // insert at the end

  EXPECT_TRUE(array_size(&a) == 6);

  static const int expected3[] = { 3, 0, 4, 1, 2, 5 };

  EXPECT_TRUE(array_equals(&a, expected3, 6));

  array_destroy(&a);
}

TEST(ArrayTest, Remove) {
  struct array a;

  array_create(&a);

  array_add(&a, 0);
  array_add(&a, 1);
  array_add(&a, 2);
  array_add(&a, 3);
  array_add(&a, 4);
  array_add(&a, 5);

  array_remove(&a, 0); // remove at the beginning

  EXPECT_TRUE(array_size(&a) == 5);

  static const int expected1[] = { 1, 2, 3, 4, 5 };

  EXPECT_TRUE(array_equals(&a, expected1, 5));

  array_remove(&a, 2); // remove in  the middle

  EXPECT_TRUE(array_size(&a) == 4);

  static const int expected2[] = { 1, 2, 4, 5 };

  EXPECT_TRUE(array_equals(&a, expected2, 4));

  array_remove(&a, 3); // remove at the end

  EXPECT_TRUE(array_size(&a) == 3);

  static const int expected3[] = { 1, 2, 4 };

  EXPECT_TRUE(array_equals(&a, expected3, 3));

  array_remove(&a, 0);
  array_remove(&a, 0);
  array_remove(&a, 0);

  EXPECT_TRUE(array_is_empty(&a));

  array_destroy(&a);
}

TEST(ArrayTest, Import) {
  struct array a;

  array_create(&a);

  static const int origin1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

  array_import(&a, origin1, 9);
  EXPECT_TRUE(array_equals(&a, origin1, 9));

  static const int origin2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

  array_import(&a, origin2, 12); // grow
  EXPECT_TRUE(array_equals(&a, origin2, 12));

  static const int origin3[] = { 0, 1, 2, 3, 4 };

  array_import(&a, origin3, 5); // shrink
  EXPECT_TRUE(array_equals(&a, origin3, 5));

  array_destroy(&a);
}

static const int unsorted[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
static const int modified[] = { 8, 4, 1, 6, 10, 3, 0, 9, 5, 2, 7 };
static const int sorted[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

TEST(ArrayTest, IsSorted) {
  struct array a;

  array_create(&a);

  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, unsorted, 11);
  EXPECT_FALSE(array_is_sorted(&a));

  array_import(&a, sorted, 11);
  EXPECT_TRUE(array_is_sorted(&a));

  array_destroy(&a);
}

TEST(ArrayTest, SelectionSort) {
  struct array a;

  array_create(&a);

  array_import(&a, unsorted, 11);
  array_selection_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, modified, 11);
  array_selection_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, sorted, 11);
  array_selection_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_destroy(&a);
}

TEST(ArrayTest, BubbleSort) {
  struct array a;

  array_create(&a);

  array_import(&a, unsorted, 11);
  array_bubble_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, modified, 11);
  array_bubble_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, sorted, 11);
  array_bubble_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_destroy(&a);
}

TEST(ArrayTest, InsertionSort) {
  struct array a;

  array_create(&a);

  array_import(&a, unsorted, 11);
  array_insertion_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, modified, 11);
  array_insertion_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, sorted, 11);
  array_insertion_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_destroy(&a);
}

TEST(ArrayTest, QuickSort) {
  struct array a;

  array_create(&a);

  array_import(&a, unsorted, 11);
  array_quick_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, modified, 11);
  array_quick_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, sorted, 11);
  array_quick_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_destroy(&a);
}

TEST(ArrayTest, HeapSort) {
  struct array a;

  array_create(&a);

  array_import(&a, unsorted, 11);
  array_heap_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, modified, 11);
  array_heap_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_import(&a, sorted, 11);
  array_heap_sort(&a);
  EXPECT_TRUE(array_is_sorted(&a));

  array_destroy(&a);
}

TEST(ArrayTest, IsHeap) {
  struct array a;

  array_create(&a);

  EXPECT_TRUE(array_is_heap(&a));

  static const int origin1[] = { 13, 6, 8, 5, 3 };

  array_import(&a, origin1, 5);
  EXPECT_TRUE(array_is_heap(&a));

  static const int origin2[] = { 81, 45, 24, 21, 6, 17, 19, 14 };

  array_import(&a, origin2, 8);
  EXPECT_TRUE(array_is_heap(&a));

  static const int origin3[] = { 6, 5, 4, 3, 1, 8 };

  array_import(&a, origin3, 6);
  EXPECT_FALSE(array_is_heap(&a));

  static const int origin4[] = { 6, 5, 4, 3, 1, 0, 8 };

  array_import(&a, origin4, 7);
  EXPECT_FALSE(array_is_heap(&a));

  array_destroy(&a);
}

TEST(ArrayTest, HeapAdd) {
  struct array a;

  array_create(&a);

  for (int i = 0; i < BIG_SIZE; ++i) {
    array_heap_add(&a, i);
    EXPECT_EQ(i, array_heap_top(&a));
    EXPECT_TRUE(array_is_heap(&a));
  }
  array_destroy(&a);
}

TEST(ArrayTest, HeapRemoveTop) {
  struct array a;

  array_create(&a);

  for (int i = 0; i < BIG_SIZE; ++i) {
    array_heap_add(&a, i);
  }

  EXPECT_TRUE(array_is_heap(&a));

  for (int i = 0; i < BIG_SIZE; ++i) {
    EXPECT_EQ(BIG_SIZE - i - 1, array_heap_top(&a));
    array_heap_remove_top(&a);
  }

  EXPECT_TRUE(array_is_empty(&a));

  array_destroy(&a);
}


