typedef struct _Node{
	int pid;   
	int nice;         
	float vruntime;	
	struct _Node* next;   //다음노드가리키는포인터
}Node;

typedef struct _List{	
	Node *head;
	Node *tail;
	int NumOfData;
}List;

List *list;
void init();
void insert(int pid, int nice);
void Vruntime();
void shandler();
void sort();
