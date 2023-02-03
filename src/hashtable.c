#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "hashtable.h"

#define DEFAULT_NUMBER 5;

extern struct element *create_element(const int key, const char *value){
	struct element* e = (struct element*) malloc(sizeof(struct element));
	e->value = (char *) malloc(sizeof(char) * strlen(value) + 1);
	e->key = key;
	strcpy(e->value, value);
	return e;
}

extern struct hashtable *create_table(int size){
	return NULL;
}

