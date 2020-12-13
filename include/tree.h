#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct tree_node;

struct tree {
  struct tree_node *root;
};

void tree_create(struct tree *self);

void tree_destroy(struct tree *self);

bool tree_contains(const struct tree *self, int value);

void tree_insert(struct tree *self, int value);

void tree_remove(struct tree *self, int value);

bool tree_is_empty(const struct tree *self);

size_t tree_size(const struct tree *self);

size_t tree_height(const struct tree *self);

typedef void (*tree_func_t)(int value, void *user_data);

void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data);

void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data);

void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data);

void tree_dump(const struct tree *self);

#ifdef __cplusplus
}
#endif

#endif /* TREE_H */
