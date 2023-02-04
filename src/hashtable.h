#define ADD_ELEMENT 0
#define EXIST_ELEMENT -1
#define NO_VALUE -2
#define TOO_SHORT -3

struct element {
	int key;
	char *value;
};

struct hashtable {
	struct element **e;
	int size;
	int count;
};

extern struct hashtable *create_table(const unsigned int size);
extern int add_element(struct hashtable *table, int key, char *value);
extern void free_item(struct element *item);
extern void free_table(struct hashtable *table);
