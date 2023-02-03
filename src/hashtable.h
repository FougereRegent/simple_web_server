#define ADD_ELEMENT 0
#define EXIST_ELEMENT -1
#define NO_VALUE -2

struct element {
	int key;
	char *value;
};

struct hashtable {
	struct element **e;
	int size;
	int count;
};

extern struct hashtable *create_table(const int);
extern int add_element(const struct element* element);
extern void free_item(struct element *item);
extern void free_table(struct hashtable *table);
