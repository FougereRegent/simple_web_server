#define ADD_ERROR -1
#define GET_ERROR -1
#define ADD_SUCESS 0
#define GET_SUCESS 0

struct http_codes {
	int *key;
	char **value;
};

struct codes {
	int http_code;
	char *http_val;
};

extern struct codes get_code(int key);
extern struct http_codes create_dict();
