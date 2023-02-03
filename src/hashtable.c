#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "hashtable.h"

typedef struct element element;
typedef struct hashtable hashtable;

element *create_element(const int key, const char *value);
static int check_element(const hashtable *table, const int key);

extern element *create_element(const int key, const char *value){
	element* e = (element*) malloc(sizeof(element));
	e->value = (char *) malloc(sizeof(char) * strlen(value) + 1);
	e->key = key;
	strcpy(e->value, value);
	return e;
}

extern hashtable *create_table(int size) {
	hashtable *h = (hashtable*) malloc(sizeof(hashtable));
	h->size = size;
	h->count = 0;
	h->e = (element**) malloc(sizeof(element*) * size);
	return h;
}

extern void free_item(element *item) {
	free(item->value);
	free(item);
}

extern void free_table(hashtable *table) {
	int i;
	for(i = 0; i < table->size; i++) {
		element *item = table->e[i];
		
		if(item != NULL) {
			free_item(item);
		}
	}
	free(table->e);
	free(table);
}

static int check_element(const hashtable *table, const int key) {
	
	return 0;
}

