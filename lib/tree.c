#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

struct tree_node {
  int value;
  struct tree_node *left;
  struct tree_node *right;
};

void tree_create(struct tree *self) {
	self->root=NULL;
}

static void node_destroy(struct tree_node *self){
	if (self==NULL){
		return;
	}
	node_destroy(self->right);
	node_destroy(self->left);
	free(self);
}

void tree_destroy(struct tree *self) {
	if (self->root == NULL){
		return;
	}
	node_destroy(self->root);
}

static bool tree_node_contains(const struct tree_node *self, int value){
	if (self == NULL) {
		return false;
	}

	if (value < self->value) {
		return tree_node_contains(self->left, value);
	}

	if (value > self->value) {
		return tree_node_contains(self->right, value);
	}

	return true;
}

bool tree_contains(const struct tree *self, int value) {
	return tree_node_contains(self->root, value);
}

static struct tree_node *insert(struct tree_node *self, int data) {
	if (self == NULL) {
		struct tree_node *other = malloc(sizeof(struct tree_node));
		other->left = other->right = NULL; 
		other->value = data;
		self = other;
	}

	if (data < self->value) {
		self->left = insert(self->left, data);
	}

	if (data > self->value) {
		self->right = insert(self->right, data);
	}

	return self;
}
void tree_insert(struct tree *self, int value) {
	self->root = insert(self->root,value);
}


static struct tree_node *b_delete_minimum(struct tree_node *self, struct tree_node **min) {
	if (self->left == NULL) {
		struct tree_node *right = self->right;
		self->right = NULL;
		*min = self;
		return right;
	}

	self->left = b_delete_minimum(self->left, min);
	return self;
}



static struct tree_node *b_delete(struct tree_node *self) {
	struct tree_node *left = self->left;
	struct tree_node *right = self->right;
	free(self); 
	self = NULL;

	if (left == NULL && right == NULL) {
		return NULL;
	}

	if (left == NULL) {
		return right;
	}

	if (right == NULL) {
		return left;
	}

	right = b_delete_minimum(right, &self);
	self->left = left; self->right = right;
	return self;
}

static struct tree_node *b_remove(struct tree_node *self, int data) {
	if (self == NULL) {
		return NULL;
	}
	if (data < self->value) {
		self->left = b_remove(self->left, data);
		return self;
	}
	if (data > self->value) {
		self->right = b_remove(self->right, data);
		return self;
	}
	return b_delete(self);
}


void tree_remove(struct tree *self, int value) {
	self->root = b_remove(self->root,value);
}

bool tree_is_empty(const struct tree *self) {
  return self->root==NULL;
}

static size_t tree_node_size(struct tree_node *self) {
	if (self==NULL){
		return 0;
	}
	return tree_node_size(self->right) + tree_node_size(self->left) + 1 ;
}

size_t tree_size(const struct tree *self) {
	return tree_node_size(self->root);
}

static size_t tree_node_height(const struct tree_node *self){

	if (self==NULL){
		return 0;
	}

	size_t left = tree_node_height(self->left);
	size_t right = tree_node_height(self->right);

	return  1 + ((left>right)?left:right);
}

size_t tree_height(const struct tree *self) {
  return tree_node_height(self->root);
}

static void tree_node_walk_pre_order(struct tree_node *self, tree_func_t func, void *user_data){
	if(self==NULL){
		return;
	}
	func(self->value,user_data);
	tree_node_walk_pre_order(self->right,func,user_data);
	tree_node_walk_pre_order(self->left,func,user_data);
}
void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data){
	tree_node_walk_pre_order(self->root,func,user_data);
}


static void tree_node_walk_in_order(struct tree_node *self, tree_func_t func, void *user_data){
	if(self==NULL){
		return;
	}
	tree_node_walk_in_order(self->left,func,user_data);
	func(self->value,user_data);
	tree_node_walk_in_order(self->right,func,user_data);
}
void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data) {
	tree_node_walk_in_order(self->root,func,user_data);
}
static void tree_node_walk_post_order(struct tree_node *self, tree_func_t func, void *user_data){
	if(self==NULL){
		return;
	}
	tree_node_walk_post_order(self->right,func,user_data);
	tree_node_walk_post_order(self->left,func,user_data);
	func(self->value,user_data);
}
void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data) {
	tree_node_walk_post_order(self->root,func,user_data);
}

void tree_dump(const struct tree *self) {
	 if (self->root==NULL){
		return ;
	}

	struct tree self_left;
	tree_create(&self_left);
	self_left.root = self->root->left;

	struct tree self_right;
	tree_create(&self_right);
	self_right.root = self->root->right;
	
	printf("-> %d -> ", self->root->value);
	
	tree_dump(&self_left);
	tree_dump(&self_right);
  
}
