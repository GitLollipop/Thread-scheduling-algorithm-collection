#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>  

typedef struct node   
{
	char name[20];       //����ID  
    int prio;            //���ȼ�  
    int round;           //����CPU��ʱ��Ƭ   
    int cputime;         //CPUִ��ʱ��  
    int needtime;        //����ʱ�� 
	int arrivetime;      //����ʱ��
    char state;          //���̵�״̬��W��������̬��R����ִ��̬��F�������̬ 
    int count;           //��¼ִ�еĴ��� 
    struct node *next;  
}PCB;  
 
PCB *ready=NULL,*run=NULL,*finish=NULL,*buf=NULL; /*�����������У��������У�ִ�ж��к���ɶ���*/   
int num;   

void GetFirst();                //�Ӿ�������ȡ�õ�һ���ڵ�  
void Output();                  //���������Ϣ  

void InsertTime(PCB *in);       //ʱ��Ƭ����   
void InsertFinish(PCB *in);     //ʱ��Ƭ���� 
void InsertBuf(PCB *in); 

void TimeCreate();              //ʱ��Ƭ���뺯��  
  
void RoundRun();                //ʱ��Ƭ��ת����   


int main(void)   
{   
	char chose;   
    printf("������Ҫ�����Ľ�����Ŀ:\n");   
    scanf("%d",&num);   
    getchar();   
	TimeCreate();   
    RoundRun();   
	Output();   
    return 0;   
}   


/*ȡ�õ�һ���������нڵ�*/ 
void GetFirst()  
{
	run = ready;   
    if(ready!=NULL)   
	{
		run ->state = 'R';   
        ready = ready ->next;   
        run ->next = NULL;   
	}   
}


/*���������Ϣ*/   
void Output() 
{   
	PCB *p;   
    p = ready;   
    printf("������\t���ȼ�\tʱ��Ƭ\tcpuʱ��\t����ʱ��\t����״̬\t������\n");   
    while(p!=NULL)   
	{   
		printf("%s\t%d\t%d\t%d\t%d\t\t%c\t\t%d\n",p->name,p->prio,p->round,p->cputime,p->needtime,p->state,p->count);   
        p = p->next;   
	}   
	p = finish;   
	while(p!=NULL)   
	{   
		printf("%s\t%d\t%d\t%d\t%d\t\t%c\t\t%d\n",p->name,p->prio,p->round,p->cputime,p->needtime,p->state,p->count);   
        p = p->next;   
	}   
	p = run;   
	while(p!=NULL)   
	{   
		printf("%s\t%d\t%d\t%d\t%d\t\t%c\t\t%d\n",p->name,p->prio,p->round,p->cputime,p->needtime,p->state,p->count);   
        p = p->next;   
	}   
}   

/*�����̲��뵽��������β��*/   
void InsertTime(PCB *in)     
{
	PCB *fst;   
    fst = ready;   
	if(ready == NULL)   
	{   
		in->next = ready;   
        ready = in;   
	}   
	else   
	{   
		while(fst->next != NULL)   
		{   
			fst = fst->next;   
		}   
		in ->next = fst ->next;   
		fst ->next = in;   
	}   
}   


/*�����̲��뵽�������β��*/   
void InsertBuf(PCB *in)     
{
	PCB *fst;   
    fst = buf;   
	if(buf == NULL)   
	{   
		in->next = buf;   
        buf = in;   
	}   
	else   
	{   
		while(fst->next != NULL)   
		{   
			fst = fst->next;   
		}   
		in ->next = fst ->next;   
		fst ->next = in;   
	}   
}   



/*�����̲��뵽��ɶ���β��*/
void InsertFinish(PCB *in)   
{   
	PCB *fst;   
    fst = finish;   
    if(finish == NULL)   
	{   
		in->next = finish;   
        finish = in;   
	}   
    else   
	{   
		while(fst->next != NULL)   
		{   
			fst = fst->next;   
		}   
		in ->next = fst ->next;   
        fst ->next = in;   
	}   
}   

/*ʱ��Ƭ���뺯��*/
void TimeCreate()   
{   
	PCB *tmp;   
    int i;   
    printf("����������֡�����ʱ��Ƭ����ʱ��ͽ��̵���ʱ�䣺\n");   
    for(i = 0;i < num; i++)   
	{   
		if((tmp = (PCB *)malloc(sizeof(PCB)))==NULL)   
		{   
			perror("malloc");   
            exit(1);   
		}   
		scanf("%s",tmp->name);   
        getchar();   
        scanf("%d",&(tmp->needtime));  
		scanf("%d",&(tmp->arrivetime));
        tmp ->cputime = 0;   
        tmp ->state ='W';   
        tmp ->prio = 0;   
        tmp ->round = 1;  /*����ÿ�������������ʱ��Ƭ��1*/   
        tmp ->count = 0; 
		InsertBuf(tmp);
	}

}   


/*ʱ��Ƭ��ת�����㷨*/  
void RoundRun() 
{   
	int i;
	int num=0;
	int flag = 1; 
	while(buf!= NULL)
		{
		
			if(buf->arrivetime == num)
			{
				InsertTime(buf);
			
			}
			buf = buf->next;
	}
    GetFirst(); 
    while(run != NULL)   
	{   
		Output();
		num++;
		while(buf != NULL)
		{
			if(buf->arrivetime == num)
			{

				InsertTime(buf);
			}
			buf = buf->next;
		}
	
        while(flag)   
		{   

			run->count++;   
            run->cputime++;   
            run->needtime--;   
            if(run->needtime == 0) /*����ִ�����*/   
			{   
				run ->state = 'F';   
                InsertFinish(run);   
                flag = 0;   
			}   
			else if(run->count == run->round)/*ʱ��Ƭ����*/   
			{   
				run->state = 'W';   
                run->count = 0;   /*���������㣬Ϊ�´���׼��*/   
                InsertTime(run);   
                flag = 0;   
			}   
		}   
		flag = 1;          
		GetFirst();  
		
	}   
}