#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct list_node;

struct list {
  struct list_node *first;
  // struct list_node *last;
};

/*
 * Create an empty list
 */
void list_create(struct list *self);

/*
 * Destroy a list
 */
void list_destroy(struct list *self);

/*
 * Compare the list to an array (data and size)
 */
bool list_equals(const struct list *self, const int *data, size_t size);

/*
 * Add an element in the list at the end
 */
void list_add_back(struct list *self, int value);

/*
 * Add an element in the list at the beginning
 */
void list_add_front(struct list *self, int value);

/*
 * Insert an element in the list (preserving the order)
 */
void list_insert(struct list *self, int value, size_t index);

/*
 * Remove an element in the list (preserving the order)
 */
void list_remove(struct list *self, size_t index);

/*
 * Get a pointer to the element at the specified index in the list
 */
int *list_get(const struct list *self, size_t index);

/*
 * Tell if the list is empty
 */
bool list_is_empty(const struct list *self);

/*
 * Get the size of the list
 */
size_t list_size(const struct list *self);

/*
 * Search for an element in the list.
 */
size_t list_search(const struct list *self, int value);

/*
 * Import an array in a list
 */
void list_import(struct list *self, const int *other, size_t size);

/*
 * Print a list to stdout
 */
void list_dump(const struct list *self);

/*
 * Tell if a list is sorted
 */
bool list_is_sorted(const struct list *self);

/*
 * Sort a list with merge sort
 */
void list_merge_sort(struct list *self);

#ifdef __cplusplus
}
#endif

#endif // LIST_H
