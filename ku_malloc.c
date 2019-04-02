#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "ku_malloc.h"

void *malloc(size_t size);
void *first_symalloc(size_t size);
void *normal_symalloc(size_t size);
void free(void *ptr);
void coalescing();
void *nextFitFind(size_t size);

hnode_t *nextFitPointer;

void *malloc(size_t size){
	void *newHeap;
	if(free_list==NULL){
		newHeap=first_symalloc(size);
	}else if(free_list!=NULL){
		newHeap=normal_symalloc(size);
	}
	fprintf(stderr,"[ALLOC] Addr : 0x%p Length : %d \n",newHeap, size);
	return newHeap;
}

void *first_symalloc(size_t size){
	void *request;
	hheader_t *header;

	header=sbrk(0); //header는 heap->top pointer
	sbrk(HEAD_SIZE); //HEAD_SIZE만큼 heap확장
	request=sbrk(0); //request는 책에나오는 ptr(free할 때 쓰는)
	sbrk(size); 

	header->size=size;
	header->magic=(unsigned long)request^KU_MAGIC;
	
	return request; //할당된 곳의 ptr, free할 때 쓰는 ptr ->마지막으로 할당된 곳 위치로 next fit 할  사용하면 될 것 같음. 
}
void *normal_symalloc(size_t size){
	void *request; 

	hheader_t *current;

	current=nextFitFind(size); //빈 자리 찾아내서 
	
	current->size = size;
	current->magic=(unsigned long)request^KU_MAGIC; //할당해주기 
	
	request = (unsigned long)current + HEAD_SIZE;	
	return request; //할당된 곳의 head adderss
}

void free(void *ptr){
	hheader_t *hhead;
	hnode_t *head;
	hnode_t *prevHead;
	hnode_t *nextHead;

	hhead=ptr-HEAD_SIZE; //free해줄 애의 head address
	
	if(free_list==NULL){
		free_list=sbrk(0);
		nextFitPointer = free_list;
	}
	if(hhead->magic==(unsigned long)ptr^KU_MAGIC){
		head=(hnode_t*)hhead;
		if(head<free_list){ //free가 free_list 윗부분에서 될 경우
			head->next=free_list;
			free_list = head;
		}else{ 
			prevHead = free_list;
			nextHead = free_list->next;
			while(nextHead != NULL){
				if(nextHead > head){
					prevHead->next=head;
					head->next = nextHead;
					break;				
				}else{
					prevHead=nextHead;
					nextHead=nextHead->next; 
				}
			}		
			if(nextHead == NULL){
				prevHead->next = head;
			}							
		}
	}
	coalescing();
	fprintf(stderr,"[FREE] : %p Length : %d\n",ptr,head->size);
}

void coalescing(){
	hnode_t *FB; //새로 free될 곳
	hnode_t *nextFB; //새로 free된 곳이랑 합쳐질 원래 free_list인 곳 

	FB=free_list;
	nextFB=free_list->next;
	
	while(nextFB!=NULL){
		if((unsigned long)FB+HEAD_SIZE+FB->size==(unsigned long)nextFB){
			FB->size=FB->size+HEAD_SIZE+nextFB->size;
			FB->next=nextFB->next;
		}else{
			FB=nextFB;
			nextFB=nextFB->next;
		}
	}
}

void *nextFitFind(size_t size){ //find free block 
	hnode_t *free_listTop;
	hnode_t *find;
	
	
	while(nextFitPointer!=NULL){
		if(nextFitPointer->size>=size+HEAD_SIZE){ //충분한 크기의 빈공간이면 
			find=nextFitPointer;	
			nextFitPointer=nextFitPointer->next;	
			break;
		}else{
			nextFitPointer=nextFitPointer->next;
		}
	}
	
	if(nextFitPointer== NULL){  //free_list 꼭대기부터 nextFitPointer 전까지 빈공간 찾기 
	/*	free_listTop=free_list;	
		if(free_listTop<nextFitPointer){
			if(free_listTop->size>=size){
				find=free_listTop;
				free_listTop=free_listTop->next;
			}else{
				free_listTop=free_listTop->next;
			}
		}else{	//결국 어디에도 없으면 힙확장시키기  */
			find=sbrk(size+HEAD_SIZE);	
	}
	return find;
}

