#include<stdlib.h>
#include<stdio.h> 

#define Free 0           //����  
#define Zhanyong 1       //ռ�� 
#define FINISH 1         //��� 
#define ERROR 0          //����  
#define memory 512       //����ڴ� 
#define min 10           //��Ƭֵ   

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
DLinkList head; //ͷ��� 
DLinkList tou;  //β���   
 

 //************************************************************************************
int Create()//��ʼ�� 
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
 int FirstFit(int ID,int space)//�״���Ӧ�㷨 
 {
	 DLinkList NewNode=(DLinkList)malloc(sizeof(Node));//�½���ҵ�Ľ��  
	 NewNode->data.ID=ID; 
	 NewNode->data.size=space; 
	 NewNode->data.sign=Zhanyong; 
	 Node *p=head; 
	 while(p) 
	 {   
		if(p->data.sign==Free && p->data.size==space)//ʣ���Сǡ������  
		{  
			p->data.sign=Zhanyong;
			p->data.ID=ID;  
			return FINISH;   
			break;   
		}    
		if(p->data.sign==Free && p->data.size>space && (p->data.size-space>min))//������������ʣ���Ҳ�������Ƭ  
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
		if(p->data.sign==Free && p->data.size>space && p->data.size-space<=min)//������Ƭʱ  
		{   
			p->data.sign=Zhanyong;
			p->data.ID=ID;
			return FINISH;
			break;
		}  
		p=p->next;//�����з��䣬Pָ����� 
	 } 
	 return ERROR; 
 }   
 
 
 //************************************************************************************
int Allocation()//�ڴ���� 
{  
	 int ID,space; 
	 printf("����������ţ�����������ͬ�����������ţ���"); 
	 scanf("%d",&ID);
	 printf("��������ڴ��С(��λ:KB)��");
	 scanf("%d",&space); 
	 if(space<=0)
	 {   
		 printf("������ڴ��С��������������\n"); 
		 return ERROR;  
	 }  
	 if(FirstFit(ID,space)==FINISH)  
		 printf("����ɹ���\n");  
	 else   
		 printf("�ڴ治�㣬����ʧ�ܣ�\n");
}  
 
 
 //************************************************************************************
 int Recycle(int ID)//�ͷ�
 {  
	 Node *p=head; 
	 while(p) 
	 {   
		 if(p->data.ID==ID)  
		 {    
			 p->data.sign=Free;    
			 p->data.ID=0;    
			 if((p->prior->data.sign==Free)&&(p->next->data.sign==Free))   //��ǰ��Ŀ��п�����   
			 {     
				 p->prior->data.size=p->prior->data.size+p->data.size+p->next->data.size;  
				 p->prior->next=p->next->next;     
				 if(p->next->next==NULL)    //��p->next�����һ�����    
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
			 if(p->prior->data.sign==Free)   //��ǰ��Ŀ��п�����  
			 {     
				 p->prior->data.size+=p->data.size; 
				 p->prior->next=p->next; 
				 p->next->prior=p->prior;
				 break;
			 }
			 if(p->next->data.sign==Free)//�����Ŀ��п����� 
			 {
				 p->data.size+=p->next->data.size;
				 if(p->next->next==NULL)//��p->next�����һ����� 
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
	 printf("������Ϊ%d���ڴ���ճɹ�\n",ID);
	 return FINISH;
 }


 //************************************************************************************
void show()
{
	printf("************************��ǰ*************************\n");
	Node *p=head->next;
	while(p)
	{
		printf("�����ţ�");
		if(p->data.ID==Free)
			printf("δ����");
		else
			printf("%6d",p->data.ID);
		printf(" ʼ��ַ��%4d",p->data.address);
		printf(" ������С��%4dKB",p->data.size);
		printf(" ״̬��");
		if(p->data.sign==Free)
			printf("����\n");
		else if(p->data.sign==Zhanyong)
			printf("�ѷ���\n");
		p=p->next;
	} //����������
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
		printf("��ѡ�������\n");
		printf("1.�����ڴ� \n"); 
		printf("2.�����ڴ� \n");
		printf("3.��ʾ�ڴ������� \n"); 
		printf("0.�˳�����\n"); 
		scanf("%d",&choice); 
		if(choice==1)
			Allocation();
		else if(choice==2)
		{
			int ID;
			printf("����Ҫ���յķ����ţ�");
			scanf("%d",&ID);
			Recycle(ID);
		}
		else if(choice==3)
			show();
		else if(choice==0)
			break;
		else
		{
			printf("��������\n");
			continue;
		}
	}
	return 0;
}

