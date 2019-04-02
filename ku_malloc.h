#define KU_MAGIC 0x19311946U
#define HEAD_SIZE sizeof(hheader_t)

typedef struct __hnode_t{
	int size;
	struct __hnode_t *next; 
}hnode_t;

hnode_t *free_list; //freelist head pointer


typedef struct __hheader_t{
	int size;
	unsigned long magic; 
}hheader_t;

void *symalloc(size_t size);
void *first_symalloc(size_t size);
void *normal_symalloc(size_t size);
void syfree(void *ptr);
void coalescing();
void *nextFitFind(size_t size);

