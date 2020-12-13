#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct array {
  int *data;
  size_t capacity;
  size_t size;
};

/*
 * Create an empty array
 */
void array_create(struct array *self);

/*
 * Destroy an array
 */
void array_destroy(struct array *self);

/*
 * Compare the array to another array (content and size)
 */
bool array_equals(const struct array *self, const int *content, size_t size);

/*
 * Add an element at the end of the array
 */
void array_add(struct array *self, int value);

/*
 * Insert an element in the array (preserving the order)
 */
void array_insert(struct array *self, int value, size_t index);

/*
 * Remove an element in the array (preserving the order)
 */
void array_remove(struct array *self, size_t index);

/*
 * Get a pointer to the element at the specified index in the array
 */
int *array_get(const struct array *self, size_t index);

/*
 * Tell if the array is empty
 */
bool array_is_empty(const struct array *self);

/*
 * Get the size of the array
 */
size_t array_size(const struct array *self);

/*
 * Search for an element in the array.
 */
size_t array_search(const struct array *self, int value);

/*
 * Search for an element in the sorted array.
 */
size_t array_search_sorted(const struct array *self, int value);

/*
 * Import an array
 */
void array_import(struct array *self, const int *other, size_t size);

/*
 * Print an array to stdout
 */
void array_dump(const struct array *self);

/*
 * Tell if the array is sorted
 */
bool array_is_sorted(const struct array *self);

/*
 * Sort the array with selection sort
 */
void array_selection_sort(struct array *self);

/*
 * Sort the array with bubble sort
 */
void array_bubble_sort(struct array *self);

/*
 * Sort the array with insertion sort
 */
void array_insertion_sort(struct array *self);

/*
 * Sort the array with quick sort
 */
void array_quick_sort(struct array *self);

/*
 * Sort the array with heap sort
 */
void array_heap_sort(struct array *self);

/*
 * Tell if the array is a heap
 */
bool array_is_heap(const struct array *self);

/*
 * Add a value into the array considered as a heap
 */
void array_heap_add(struct array *self, int value);

/*
 * Get the value at the top of the heap
 */
int array_heap_top(const struct array *self);

/*
 * Remove the top value in the array considered as a heap
 */
void array_heap_remove_top(struct array *self);


#ifdef __cplusplus
}
#endif

#endif // ARRAY_H
