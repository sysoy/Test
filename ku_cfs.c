#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include "ku_cfs.h"
int kill_count=0;

int main(int argc, char *argv[])
{
	
	int i,j;
	char tmp;
	char str[2];
	int count; // count process
	pid_t pid;
      	
	list=(List*)malloc(sizeof(List));
	struct itimerval timeslice;
	
	str[0]=64;
	str[1]='\0';
	count=64;
	init();
		
	for(i=0;i<5;i++) //fork_count=5->make five nodes
	{	
		int k = atoi(argv[i+1]);
		for(j=0;j<k;j++) 
		{
			tmp = str[0];
			str[0]=tmp+1;
			
			pid = fork();
			
			switch(pid)
			{
				case -1:
					printf("fork failed\n");
					break;
				case 0:
					execlp("./ku_app","ku_app",str,NULL);
					printf("exec failed\n");
					break;
				default:
					insert(pid, i-2); 
					//printf("ku_app completed\n");
			}
		}
	}

	
		
	signal(SIGALRM, shandler);	
	
	timeslice.it_interval.tv_sec=1;
	timeslice.it_interval.tv_usec=0;
	timeslice.it_value.tv_sec=1;
	timeslice.it_value.tv_usec=0;
	
	if(setitimer(ITIMER_REAL, &timeslice, NULL)==-1){
		perror("failed\n");
	};
	
	
	while(kill_count<=atoi(argv[6])){
		pause();
	}
	
	return 0;
}

void shandler(){
	kill(list->head->pid,SIGSTOP);
	kill_count++;
	sort();
	kill(list->head->pid,SIGCONT);
	Vruntime();

}

void sort(){
	
	Node *pHead = list->head;
	Node *tmp =list->head->next;
	Node *node = tmp;

	while(tmp!=NULL)
	{
		if(pHead->vruntime>tmp->vruntime){
			node=tmp;
			tmp=tmp->next;
		}
		else{
			break;
		}
	}
	
	
	if(pHead->next!=tmp)
	{
		list->head=pHead->next;
		node->next=pHead;
		pHead->next=tmp;
	}	
}

void Vruntime(){

	Node *temp=list->head;
		switch(temp->nice)
		{
			case -2:
			temp->vruntime+=(pow(1.25,(temp->nice)));
			break;
	
			case -1:
			temp->vruntime+=(pow(1.25,(temp->nice)));
			break;
	
			case 0:
			temp->vruntime+=(pow(1.25,(temp->nice)));
			break;	
		
			case 1:
			temp->vruntime+=(pow(1.25,(temp->nice)));
			break;

			case 2:
			temp->vruntime+=(pow(1.25,(temp->nice)));
			break;
		}
	
}

void init(){
	list->head=NULL;
	list->tail=NULL;
	list->NumOfData=0;
}

void insert(int pid, int nice){
	//새 노드를 꼬리에 추가	

	Node *newnode = (Node*)malloc(sizeof(Node));	
	
	newnode->pid=pid;
	newnode->nice=nice;
	newnode->vruntime=0;
	newnode->next=NULL;

		if(list->head==NULL && list->tail==NULL)
		{
			list->head=newnode;	
			list->tail=newnode;	
		}
		else
		{
			list->tail->next=newnode;
			list->tail=newnode;
		}
		(list->NumOfData)++;
}









