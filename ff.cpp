#include<stdlib.h>
#include<stdio.h> 

#define Free 0           //空闲  
#define Zhanyong 1       //占用 
#define FINISH 1         //完成 
#define ERROR 0          //出错  
#define memory 512       //最大内存 
#define min 10           //碎片值   

//************************************************************************************
typedef struct Body 
{  
	int ID;
	int size;
	int address;
	int sign;  
};

typedef struct Node  
{     
	Body data;
	struct Node *prior;
	struct Node *next;
}*DLinkList;  
DLinkList head; //头结点 
DLinkList tou;  //尾结点   
 

 //************************************************************************************
int Create()//初始化 
{  
	 head=(DLinkList)malloc(sizeof(Node));
	 tou=(DLinkList)malloc(sizeof(Node)); 
	 head->prior=NULL; 
	 head->next=tou; 
	 tou->prior=head; 
	 tou->next=NULL; 
	 tou->data.address=0; 
	 tou->data.size=memory; 
	 tou->data.ID=0;
	 tou->data.sign=Free; 
	 return FINISH; 
}   
 

 //************************************************************************************
 int FirstFit(int ID,int space)//首次适应算法 
 {
	 DLinkList NewNode=(DLinkList)malloc(sizeof(Node));//新建作业的结点  
	 NewNode->data.ID=ID; 
	 NewNode->data.size=space; 
	 NewNode->data.sign=Zhanyong; 
	 Node *p=head; 
	 while(p) 
	 {   
		if(p->data.sign==Free && p->data.size==space)//剩余大小恰好满足  
		{  
			p->data.sign=Zhanyong;
			p->data.ID=ID;  
			return FINISH;   
			break;   
		}    
		if(p->data.sign==Free && p->data.size>space && (p->data.size-space>min))//满足需求且有剩余且不产生碎片  
		{   
			NewNode->prior=p->prior;  
			NewNode->next=p;   
			NewNode->data.address=p->data.address;  
			p->prior->next=NewNode;  
			p->prior=NewNode;  
			p->data.address=NewNode->data.address+NewNode->data.size;  
			p->data.size=p->data.size-space;  
			return FINISH;  
			break;   
		}    
		if(p->data.sign==Free && p->data.size>space && p->data.size-space<=min)//产生碎片时  
		{   
			p->data.sign=Zhanyong;
			p->data.ID=ID;
			return FINISH;
			break;
		}  
		p=p->next;//若已有分配，P指针后移 
	 } 
	 return ERROR; 
 }   
 
 
 //************************************************************************************
int Allocation()//内存分配 
{  
	 int ID,space; 
	 printf("请输入分区号（不能输入相同的两个分区号）："); 
	 scanf("%d",&ID);
	 printf("输入分配内存大小(单位:KB)：");
	 scanf("%d",&space); 
	 if(space<=0)
	 {   
		 printf("分配的内存大小必须是正整数！\n"); 
		 return ERROR;  
	 }  
	 if(FirstFit(ID,space)==FINISH)  
		 printf("分配成功！\n");  
	 else   
		 printf("内存不足，分配失败！\n");
}  
 
 
 //************************************************************************************
 int Recycle(int ID)//释放
 {  
	 Node *p=head; 
	 while(p) 
	 {   
		 if(p->data.ID==ID)  
		 {    
			 p->data.sign=Free;    
			 p->data.ID=0;    
			 if((p->prior->data.sign==Free)&&(p->next->data.sign==Free))   //与前后的空闲块相连   
			 {     
				 p->prior->data.size=p->prior->data.size+p->data.size+p->next->data.size;  
				 p->prior->next=p->next->next;     
				 if(p->next->next==NULL)    //若p->next是最后一个结点    
				 {
					 p->prior->data.ID=0;
					 p->next=NULL;
				 }     
				 else
				 {
					 p->next->next->prior=p->prior;
				 }     
				 break;     
			 }   
			 if(p->prior->data.sign==Free)   //与前面的空闲块相连  
			 {     
				 p->prior->data.size+=p->data.size; 
				 p->prior->next=p->next; 
				 p->next->prior=p->prior;
				 break;
			 }
			 if(p->next->data.sign==Free)//与后面的空闲块相连 
			 {
				 p->data.size+=p->next->data.size;
				 if(p->next->next==NULL)//若p->next是最后一个结点 
					 p->next=NULL;
				 else
				 {
					 p->next->next->prior=p;
					 p->next=p->next->next;
				 }
				 break;
			 }
			 break;
		 }
		 p=p->next;
	 }
	 printf("分区号为%d的内存回收成功\n",ID);
	 return FINISH;
 }


 //************************************************************************************
void show()
{
	printf("************************当前*************************\n");
	Node *p=head->next;
	while(p)
	{
		printf("分区号：");
		if(p->data.ID==Free)
			printf("未分配");
		else
			printf("%6d",p->data.ID);
		printf(" 始地址：%4d",p->data.address);
		printf(" 分区大小：%4dKB",p->data.size);
		printf(" 状态：");
		if(p->data.sign==Free)
			printf("空闲\n");
		else if(p->data.sign==Zhanyong)
			printf("已分配\n");
		p=p->next;
	} //存分配情况内
	printf("\n");
}


//************************************************************************************
int main()
{
	Create();
	int choice;
	int i;
	for(i=0;;i++)
	{
		printf("请选择操作：\n");
		printf("1.分配内存 \n"); 
		printf("2.回收内存 \n");
		printf("3.显示内存分配情况 \n"); 
		printf("0.退出程序\n"); 
		scanf("%d",&choice); 
		if(choice==1)
			Allocation();
		else if(choice==2)
		{
			int ID;
			printf("输入要回收的分区号：");
			scanf("%d",&ID);
			Recycle(ID);
		}
		else if(choice==3)
			show();
		else if(choice==0)
			break;
		else
		{
			printf("输入有误！\n");
			continue;
		}
	}
	return 0;
}

