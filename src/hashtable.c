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

extern hashtable *create_table(const unsigned int size) {
	int index;
	hashtable *h = (hashtable*) malloc(sizeof(hashtable));
	h->size = size;
	h->count = 0;
	h->e = (element**) calloc(sizeof(element*), size);

	for(index = 0; index < size; index++) {
		h->e[index] = NULL;
	}
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

extern int add_element(struct hashtable *table, int key, char *value) {
	if(table->size == table->count - 1) {
		return TOO_SHORT;
	}
	if(check_element(table, key) < 0) {
		return EXIST_ELEMENT;
	}

	element *item = create_element(key, value);
	table->e[table->count++] = item;
	return ADD_ELEMENT;
}

extern struct element *get_item(struct hashtable *table, const int key) {
	int index;
	element *item;
	for(index = 0; index < table->size; index++) {
		item = table->e[index];
		if(item->key == key) {
			return item;
		}
	}
	return NULL;
}
static int check_element(const hashtable *table, const int key) {
	int index;
	element *item;
	for (index = 0; index < table->size; index++) {
		item = table->e[index];
		if(item == NULL)
			continue;
		if(item->key == key)
			return EXIST_ELEMENT;
	}
	return 0;
}

