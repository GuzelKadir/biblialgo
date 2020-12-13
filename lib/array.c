#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
void array_create(struct array *self) {
	self->size=0;
	self->capacity=10;
	self->data=NULL;
}

void array_destroy(struct array *self) {
	self->size=0;
	free(self->data);
}

bool array_equals(const struct array *self, const int *content, size_t size) {
	if (self->size != size)
		return false;
	for (int i = 0; i < size; ++i){
		if(self->data[i]!=content[i])
			return false;
	}
  return true;
}

static void grow(struct array *self){
		self->capacity *= 2;
	  int *data = calloc(self->capacity, sizeof(int));
	  if (self->data == NULL){
	    printf("Erreur allocation memoire!\n");
	return;
	}
	memcpy(data, self->data, sizeof(int)*self->size);
	free(self->data);
	self->data=data;
}


void array_add(struct array *self, int value) {
	if(self->data==NULL){
	    self->data = calloc(self->capacity, sizeof(int));
	    if (self->data == NULL){
	        printf("Erreur allocation memoire!\n");
	        return;
       	}
	    self->size++;
	    self->data[0]= value;
  	}else{
	    if (self->size>=self->capacity){
	      grow(self);
	    }
	    self->data[self->size] = value;
	    self->size++;
  	}
}

void array_insert(struct array *self, int value, size_t index) {
	if (self->size+1>=self->capacity)
		grow(self);
	self->size++;
	for (int i = self->size-1; i >=0; --i)
	{	
		if (i>=index)
			self->data[i+1]=self->data[i];
		if(i==index){
			self->data[index]=value;
			return;
		}
	}
}

void array_remove(struct array *self, size_t index) {
	for (int i = index; i < self->size; ++i)
	{
		if(i>=index){
			self->data[i]=self->data[i+1];
		}
	}
	self->size--;
}

int *array_get(const struct array *self, size_t index) {
  if (index >=self->size){
  		return NULL;
	}
	int *res=&self->data[index];
	return res;
}

bool array_is_empty(const struct array *self) {
	return (self->size == 0);
}

size_t array_size(const struct array *self) {
  return self->size;
}

size_t array_search(const struct array *self, int value) {
	for (size_t i = 0; i < self->size; ++i)
	{
		if(value == self->data[i])
			return i;
	}
  return self->size;
}

static size_t array_binary_search(const int *data, size_t n,
int e, size_t lo, size_t hi) {
	if (lo == hi) {
	return n;
	}
	size_t mid = (lo + hi) / 2;
	if (e < data[mid]) {
	return array_binary_search(data, n, e, lo, mid);
	}
	if (data[mid] < e) {
	return array_binary_search(data, n, e, mid + 1, hi);
	}
	return mid;
}

size_t array_search_sorted(const struct array *self, int value) {
  	return array_binary_search(self->data,self->size,value,0,self->size);
}

void array_import(struct array *self, const int *other, size_t size) {
	if (self->data==NULL)
	{
		self->size = size;
		self->capacity = size;
		int *data = malloc(sizeof(int)*self->capacity);
		memcpy(data, other, size * sizeof(int));
		self->data = data;
		return;
	}
	self->size=0;
	for (int i = 0; i < size; ++i)
	{
		array_add(self,other[i]);
	}
}

void array_dump(const struct array *self) {
	printf("capacity:%ld\n",self->capacity);
  	printf("size:%ld\n",self->size);
  	for (int i = 0; i < self->size; ++i)
  	{
  		printf("element %d= %d\n",i,self->data[i]);
  	}
}

bool array_is_sorted(const struct array *self) {
	if(self->data==NULL)
		return true;
	for (int i = 1; i < self->size; ++i)
	{
		if (self->data[i]<self->data[i-1])
			return false;
	}
  return true;
}

static void array_swap(int *data,size_t a,size_t b){
	size_t tmp = data[a];
	data[a] = data[b];
	data[b] = tmp;
}

void array_selection_sort(struct array *self) {
	for (size_t i=0 ; i<self->size-1 ; ++i){
		size_t j = i;
		for (size_t k=j+1 ; k<self->size ; ++k){
			if (self->data[k] < self->data[j]){
				j=k;
			}
		}
		array_swap(self->data,i,j);
	}
}

void array_bubble_sort(struct array *self) {
	for (size_t i=0 ; i<self->size-1 ; ++i){
		for (size_t j=self->size-1 ; j>i ; --j){
			if (self->data[j] < self->data[j-1]){
				array_swap(self->data,j,j-1);
			}
		}
	}
}

void array_insertion_sort(struct array *self) {
	for (size_t i=1 ; i<self->size ; ++i){
			int x = self->data[i];
			size_t j = i;
			while(j>0 && self->data[j-1]>x){
				self->data[j] = self->data[j-1];
				j--;
			}
			self->data[j] = x;
	}
}

static ssize_t array_partition(int *data, ssize_t a, ssize_t b) {
	ssize_t pivot_index = a;
	const int pivot = data[pivot_index];
	array_swap(data, pivot_index, b);
	ssize_t c = a;
	for (ssize_t i = a; i < b; i++) {
		if (data[i] < pivot) {
			array_swap(data, i, c);
			c++;
		}
	}
	array_swap(data, c, b);
	return c;
}
static void array_sort_partial(int *data, ssize_t a, ssize_t b ) {
	if (a < b) {
		ssize_t c = array_partition(data, a, b);
		array_sort_partial(data, a, c - 1);
		array_sort_partial(data, c + 1, b);
	}
}

void array_quick_sort(struct array *self) {
	array_sort_partial(self->data, 0, self->capacity - 1);
}

static void heap_insert(int *heap, size_t n, int value) {
	size_t i = n;
	 heap[i]=value;
	 while(i>0){
	     size_t j = (i-1)/2;
	     if(heap[i]<heap[j]){
	         break;
	     }
	     array_swap(heap, i, j);
	     i=j;
	 }
}
static void heap_remove(int *heap, size_t n) {
    heap[0] = heap[n - 1];
    size_t i = 0;
    while (i < (n - 1) / 2) {
        size_t lt = 2 * i + 1;
        size_t rt = 2 * i + 2;
        if (heap[i] > heap[lt] && heap[i] > heap[rt]) {
            break;
        }
        size_t j = (heap[lt] > heap[rt]) ? lt : rt;
        array_swap(heap, i, j);
        i = j;
    }
}

void array_heap_sort(struct array *self) {
	  for (size_t i = 0; i < self->capacity; ++i) {
        int value = self->data[i];
        heap_insert(self->data, i, value);
    }

    for (size_t i = 0; i < self->capacity; ++i) {
        int value = self->data[0];
        heap_remove(self->data, self->capacity - i);
        self->data[self->capacity - i - 1] = value;
    }
}


bool array_is_heap(const struct array *self) {
  for (size_t i=1;i<self->size;++i){
		if (self->data[i]>self->data[(i-1)/2]){
			return false;
		}
	}
	return true;
}

void array_heap_add(struct array *self, int value) {
	if(self->data==NULL){
	    self->data = calloc(self->capacity, sizeof(int));
	    if (self->data == NULL){
	        printf("Erreur allocation memoire!\n");
	        return;
       	}
	    self->size++;
	    self->data[0]= value;
  	}else{
	    if(self->size >= self->capacity){
	        grow(self);
	    }
	    heap_insert(self->data, self->size, value);

	    self->size++;
	}
}

int array_heap_top(const struct array *self) {
  return self->data[0];
}

void array_heap_remove_top(struct array *self) {
    self->data[0] = self->data[self->size - 1];
    size_t i = 0;
    while (i < (self->size - 1) / 2) {
        size_t lt = 2 * i + 1;
        size_t rt = 2 * i + 2;
        if (self->data[i] > self->data[lt] && self->data[i] > self->data[rt]) {
            break;
        }
    size_t j = (self->data[lt] > self->data[rt]) ? lt : rt;
    array_swap(self->data, i, j);
    i = j;
    }
    self->size--;
}
