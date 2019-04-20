#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>  

typedef struct node   
{
	char name[20];       //进程ID  
    int prio;            //优先级  
    int round;           //分配CPU的时间片   
    int cputime;         //CPU执行时间  
    int needtime;        //服务时间 
	int arrivetime;      //到达时间
    char state;          //进程的状态，W——就绪态，R——执行态，F——完成态 
    int count;           //记录执行的次数 
    struct node *next;  
}PCB;  
 
PCB *ready=NULL,*run=NULL,*finish=NULL,*buf=NULL; /*定义三个队列，就绪队列，执行队列和完成队列*/   
int num;   

void GetFirst();                //从就绪队列取得第一个节点  
void Output();                  //输出队列信息  

void InsertTime(PCB *in);       //时间片队列   
void InsertFinish(PCB *in);     //时间片队列 
void InsertBuf(PCB *in); 

void TimeCreate();              //时间片输入函数  
  
void RoundRun();                //时间片轮转调度   


int main(void)   
{   
	char chose;   
    printf("请输入要创建的进程数目:\n");   
    scanf("%d",&num);   
    getchar();   
	TimeCreate();   
    RoundRun();   
	Output();   
    return 0;   
}   


/*取得第一个就绪队列节点*/ 
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


/*输出队列信息*/   
void Output() 
{   
	PCB *p;   
    p = ready;   
    printf("进程名\t优先级\t时间片\tcpu时间\t服务时间\t进程状态\t计数器\n");   
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

/*将进程插入到就绪队列尾部*/   
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


/*将进程插入到缓冲队列尾部*/   
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



/*将进程插入到完成队列尾部*/
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

/*时间片输入函数*/
void TimeCreate()   
{   
	PCB *tmp;   
    int i;   
    printf("输入进程名字、进程时间片所需时间和进程到达时间：\n");   
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
        tmp ->round = 1;  /*假设每个进程所分配的时间片是1*/   
        tmp ->count = 0; 
		InsertBuf(tmp);
	}

}   


/*时间片轮转调度算法*/  
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
            if(run->needtime == 0) /*进程执行完毕*/   
			{   
				run ->state = 'F';   
                InsertFinish(run);   
                flag = 0;   
			}   
			else if(run->count == run->round)/*时间片用完*/   
			{   
				run->state = 'W';   
                run->count = 0;   /*计数器清零，为下次做准备*/   
                InsertTime(run);   
                flag = 0;   
			}   
		}   
		flag = 1;          
		GetFirst();  
		
	}   
}