#define ADD_ERROR -1
#define GET_ERROR -1
#define ADD_SUCESS 0
#define GET_SUCESS 0

struct element {
	int key;
	char *value;
};

struct hashtable {
	struct element **e;
	int size;
	int count;
};

extern struct element *create_element(const int key, const char restrict* value);
extern struct hashtable *create_table(const int);
