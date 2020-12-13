#include "list.h"

#include <stdlib.h>
#include <stdio.h>

struct list_node {
  int value;
  struct list_node *next;
  // struct list_node *prev;
};

void list_create(struct list *self) {
	self->first=NULL;
}

void list_destroy(struct list *self) {
	struct list_node *current=self->first;
	struct list_node *next=current->next;
	while(current!=NULL){
		next=current->next;
		free(current);
		current=next;
	}
}


bool list_equals(const struct list *self, const int *data, size_t size) {
	if (list_size(self)!=size)
		return false;
	struct list_node *current=self->first;
	for (int i = 0; i < size; ++i)
	{
		if (data[i]!=current->value)
			return false;
		current=current->next;
	}
  return true;
}

void list_add_back(struct list *self, int value) {
	struct list_node *new = malloc(sizeof(struct list_node));
	new->value=value;
	new->next=NULL;
	if (self->first==NULL)
	{
		self->first=new;
	}else{
		struct list_node *current=self->first;
		while(current->next!=NULL){
			current=current->next;
		}
		current->next=new;
	}		
}

void list_add_front(struct list *self, int value) {
	struct list_node *new = malloc(sizeof(struct list_node));
	new->value=value;
	new->next=self->first;
	self->first=new;
}

void list_insert(struct list *self, int value, size_t index) {
	if (index==0){
		list_add_front(self, value);
	}
	else {
		struct list_node *other = malloc(sizeof(struct list_node)); 
		struct list_node *current = self->first;
		size_t count = 0;
		other->value = value;

		while((current!=NULL) && (count<index-1)){
			current = current->next;
			count++;
		}

		struct list_node *preview = current;
		current = current->next;
		preview->next = other;
		other->next = current;
	}
}

void list_remove(struct list *self, size_t index) {
	struct list_node *current = self->first;
	if (index==0)
	{
		self->first=current->next;
		free(current);
	}else{
		size_t count = 0;
		while((current!=NULL) && (count<index-1)){
			current = current->next;
			count++;
		}
		struct list_node *preview = current;
		current = current->next;
		preview->next = current->next;
		free(current);
	}
}

int *list_get(const struct list *self, size_t index) {
	size_t count=0;
	struct list_node *current = self->first;
	while(current!=NULL){
		if (count==index)
			return &current->value;
		current=current->next;
	}
	return NULL;
}

bool list_is_empty(const struct list *self) {
  return self->first==NULL;
}

size_t list_size(const struct list *self) {
	struct list_node *current=self->first;
	size_t count=0;
	while(current!=NULL){
		count++;
		current=current->next;
	}
	return count;
}

size_t list_search(const struct list *self, int value) {
	size_t index=0;
	struct list_node *current = self->first;
	while(current!=NULL){
		if(current->value==value){
			return index;
		}
		index++;
		current=current->next;
	}
  return list_size(self);
}

void list_import(struct list *self, const int *other, size_t size) {
	list_destroy(self);
	list_create(self);
	for (int i = 0; i < size; ++i)
	{
		list_add_back(self,other[i]);
	}
}

void list_dump(const struct list *self) {
	struct list_node *current = self->first;
	while(current!=NULL){
		printf("element: %d\n",current->value);
		current=current->next;
	}
}

bool list_is_sorted(const struct list *self) {
	if(list_is_empty(self))
		return true;
	struct list_node *current = self->first;
	int prev=current->value;
	while(current!=NULL){
		if(prev>current->value)
			return false;
		prev=current->value;
		current=current->next;
	}
	return true;
}

static void list_merge(struct list *self, size_t i, size_t m, size_t j) {

	size_t a = i;
	size_t b = m;

	for (size_t k = i; k < j; ++k) {

			int *aa = list_get(self, a);
			int *ab = list_get(self, b);

		if (a < m && (b == j || aa < ab)) {

			size_t count_1 = 0;
			struct list_node *current_1 = self->first;

			while((current_1!=NULL) && count_1 < k){
				current_1 = current_1->next;
				count_1++;
			}

			size_t count_2 = 0;
			struct list_node *current_2 = self->first;

			while((current_2!=NULL) && count_2 < a){
				current_2 = current_2->next;
				count_2++;
			}
			
			size_t tmp = current_1->value;
			current_1->value = current_2->value;
			current_2->value = tmp;
			
			a++;
		} 
		else {
			size_t count_1 = 0;
			struct list_node *current_1 = self->first;

			while((current_1!=NULL) && count_1 < k){
				current_1 = current_1->next;
				count_1++;
			}

			size_t count_2 = 0;
			struct list_node *current_2 = self->first;

			while((current_2!=NULL) && count_2 < b){
				current_2 = current_2->next;
				count_2++;
			}
			
			size_t tmp = current_1->value;
			current_1->value = current_2->value;
			current_2->value = tmp;

			b++;
		}
	}
}

static void list_merge_sort_partial(struct list *self, size_t i, size_t j) {
	if (j - i < 2) {
		return;
	} 
	size_t m = (i + j) / 2;
	list_merge_sort_partial(self, i, m);
	list_merge_sort_partial(self, m, j);
	list_merge(self, i, m, j);

}

void list_merge_sort(struct list *self) {
	list_destroy(self);
	list_create(self);

	size_t n = list_size(self);
	list_merge_sort_partial(self, 0, n);
}
