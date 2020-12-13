#include "tree.h"

#include "gtest/gtest.h"

TEST(TreeTest, CreateEmpty) {
  struct tree t;

  tree_create(&t);

  EXPECT_TRUE(tree_is_empty(&t));
  EXPECT_EQ(0u, tree_size(&t));
  EXPECT_EQ(0u, tree_height(&t));

  tree_destroy(&t);
}

TEST(TreeTest, CreateOneElement) {
  struct tree t;

  tree_create(&t);
  tree_insert(&t, 0);

  EXPECT_FALSE(tree_is_empty(&t));
  EXPECT_EQ(1u, tree_size(&t));
  EXPECT_EQ(1u, tree_height(&t));

  tree_destroy(&t);
}

TEST(TreeTest, CreateManyElements) {

  struct tree t;

  tree_create(&t);

  tree_insert(&t, 5);
  tree_insert(&t, 7);

  EXPECT_FALSE(tree_is_empty(&t));
  EXPECT_EQ(2u, tree_size(&t));
  EXPECT_EQ(2u, tree_height(&t));

  tree_insert(&t, 7); // already present

  EXPECT_FALSE(tree_is_empty(&t));
  EXPECT_EQ(2u, tree_size(&t));
  EXPECT_EQ(2u, tree_height(&t));

  tree_insert(&t, 3);

  EXPECT_FALSE(tree_is_empty(&t));
  EXPECT_EQ(3u, tree_size(&t));

  tree_destroy(&t);
}

#define BIG_SIZE 1000

static size_t log_2(size_t x) {
  size_t res = 0;

  while (x != 0) {
    res++;
    x /= 2;
  }

  return res;
}

TEST(TreeTest, CreateStressed) {
  struct tree t;

  tree_create(&t);

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();
    tree_insert(&t, value);
  }

  EXPECT_FALSE(tree_is_empty(&t));

  size_t size = tree_size(&t);
  size_t height = tree_height(&t);

  EXPECT_LE(size, static_cast<size_t>(BIG_SIZE));
  EXPECT_LE(height, size);
  EXPECT_LE(log_2(size + 1), height);

  tree_destroy(&t);
}

const int values[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

TEST(TreeTest, Contains) {
  struct tree t;

  tree_create(&t);

  for (size_t i = 0; i < 9; ++i) {
    tree_insert(&t, values[i]);
  }

  for (size_t i = 0; i < 9; ++i) {
    EXPECT_TRUE(tree_contains(&t, values[i]));
  }

  for (int i = 1; i <= 19; i += 2) {
    EXPECT_FALSE(tree_contains(&t, i));
  }

  tree_destroy(&t);
}

TEST(TreeTest, ContainsStressed) {
  struct tree t;

  tree_create(&t);
  srand(0);

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();
    tree_insert(&t, value);
  }

  srand(0); // reinit

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();
    EXPECT_TRUE(tree_contains(&t, value));
  }

  tree_destroy(&t);
}

TEST(TreeTest, Insert) {
  struct tree t;

  tree_create(&t);

  for (size_t i = 0; i < 9; ++i) {
    tree_insert(&t, values[i]);
    EXPECT_EQ(i + 1, tree_size(&t));
  }

  tree_destroy(&t);
}

TEST(TreeTest, InsertStressed) {
  struct tree t;

  tree_create(&t);

  size_t expected_size = 0;

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();

    if (!tree_contains(&t, value)) {
      expected_size++;
    }

    tree_insert(&t, value);
    EXPECT_EQ(expected_size, tree_size(&t));
  }

  tree_destroy(&t);
}

TEST(TreeTest, Remove) {
  struct tree t;

  tree_create(&t);

  for (size_t i = 0; i < 9; ++i) {
    tree_insert(&t, values[i]);
  }

  for (size_t i = 0; i < 9; ++i) {
    EXPECT_TRUE(tree_contains(&t, values[i]));
    tree_remove(&t, values[i]);
    EXPECT_FALSE(tree_contains(&t, values[i]));
    EXPECT_EQ(8 - i, tree_size(&t));
  }

  tree_destroy(&t);
}

TEST(TreeTest, RemoveStressed) {
  struct tree t;

  tree_create(&t);
  srand(0);

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();
    tree_insert(&t, value);
  }

  size_t expected_size = tree_size(&t);
  EXPECT_TRUE(tree_size(&t) > 0);

  srand(0); // reinit

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();

    if (tree_contains(&t, value)) {
      expected_size--;
    }

    tree_remove(&t, value);
    EXPECT_EQ(expected_size, tree_size(&t));
  }

  EXPECT_EQ(0u, tree_size(&t));

  tree_destroy(&t);
}

static void check_tree(int value, void *user_data) {
  int *expected = static_cast<int *>(user_data);

  ASSERT_TRUE(expected != NULL);
  EXPECT_EQ(*expected, value);

  (*expected) += 2;
}

TEST(TreeTest, WalkInOrder) {
  struct tree t;

  tree_create(&t);

  for (size_t i = 0; i < 9; ++i) {
    tree_insert(&t, values[i]);
  }

  EXPECT_EQ(9u, tree_size(&t));

  int expected = 2;
  tree_walk_in_order(&t, check_tree, &expected);

  EXPECT_EQ(expected, 20);

  tree_destroy(&t);
}


static void check_once(int value, void *user_data) {
  int *count = static_cast<int *>(user_data);

  ASSERT_TRUE(count != NULL);
  ASSERT_TRUE(2 <= value && value <= 18);
  EXPECT_EQ(count[value / 2], 0);

  count[value / 2] = 1;
}



TEST(TreeTest, WalkPrePostOrder) {
  struct tree t;

  tree_create(&t);

  for (size_t i = 0; i < 9; ++i) {
    tree_insert(&t, values[i]);
  }

  int count[10];

  // pre order

  memset(count, 0, sizeof count);
  tree_walk_pre_order(&t, check_once, count);

  EXPECT_EQ(count[0], 0);

  for (int i = 1; i < 10; ++i) {
    EXPECT_EQ(count[i], 1);
  }

  // pre order

  memset(count, 0, sizeof count);
  tree_walk_post_order(&t, check_once, count);

  EXPECT_EQ(count[0], 0);

  for (int i = 1; i < 10; ++i) {
    EXPECT_EQ(count[i], 1);
  }

  // end

  tree_destroy(&t);
}
