#include<stdio.h>
#include<stdlib.h>

struct process
{
	char pname;
	float arrivetime;
	float servetime;
	float finishtime;
	float roundtime;
	float droundtime;
	float waittime;
	float yxq; //����Ȩ
};
struct process pro[100];

void fcfs(struct process pro[],int n);
void sjf(struct process pro[],int n);
struct process *sortarrivetime(struct process pro[],int n); 
void print(struct process pro[],int n);

void main()
{
	int n,i;
	printf("�������м������̣�\n");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		fflush(stdin);
		printf("�������%d����������(char):\n",i+1); scanf("%c",&pro[i].pname);
        printf("����ʱ��\n");
		scanf("%f",&pro[i].arrivetime);
		printf("����ʱ��\n");
		scanf("%f",&pro[i].servetime);
	}
	fcfs(pro,n);
	sjf(pro,n);
}

//������ʱ�����ð�ݷ�����
struct process *sortarrivetime(struct process pro[],int n)
{
	int i,j;
	struct process itemp;
	int flag;
	for(i=1;i<n;i++)
	{
		flag=0;
		for(j=0;j<n-i;j++)
		{
			if(pro[j].arrivetime>pro[j+1].arrivetime)
			{
				itemp=pro[j];
				pro[j]=pro[j+1];
				pro[j+1]=itemp;
				flag=1; //������־
			}
		}
		if(flag==0) //���һ��������û�����κν�������������� 
			break;
	}
	return pro;
}


//��ƽ����+�������
void print(struct process pro[],int n)
{
	int i;
	float Sumroundtime=0,Sumdroundtime=0;
	float averoundtime,avedroundtime;
	for(i=0;i<n;i++)//��ƽ��ֵ
	{
		Sumroundtime+=pro[i].roundtime;
		Sumdroundtime+=pro[i].droundtime;
	}
	averoundtime=Sumroundtime/n;
	avedroundtime=Sumdroundtime/n;
	printf("������\t����ʱ��\t����ʱ��\t���ʱ��\t��תʱ��\t��ȫ��ת"); 
	for(i=0;i<n;i++)//���
	{
		printf("%c\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",pro[i].pname,pro[i].arrivetime,
			         pro[i].servetime,pro[i].finishtime,pro[i].roundtime,pro[i].droundtime);
	}
	printf("ƽ��ֵ\t\t\t\t\t\t\t%.2f\t\t%.2f\n",averoundtime,avedroundtime); 
}


//�����ȷ����㷨
void fcfs(struct process pro[],int n)
{
	int i;
	pro=sortarrivetime(pro,n);
	pro[0].finishtime=pro[0].arrivetime+pro[0].servetime; 
	pro[0].roundtime=pro[0].finishtime-pro[0].arrivetime;
	pro[0].droundtime=pro[0].roundtime/pro[0].servetime; 
	for(i=1;i<n;i++)
	{
		if(pro[i].arrivetime<=pro[i-1].finishtime)
		{
			pro[i].finishtime=pro[i-1].finishtime+pro[i].servetime; 
			pro[i].roundtime=pro[i].finishtime-pro[i].arrivetime; 
			pro[i].droundtime=pro[i].roundtime/pro[i].servetime; 
		}
		else
		{
			pro[i].finishtime=pro[i].arrivetime+pro[i].servetime; 
			pro[i].roundtime=pro[i].finishtime-pro[i].arrivetime;
			pro[i].droundtime=pro[i].roundtime/pro[i].servetime;
		}
	}
	printf("\n\n�����ȷ���\n\n");
	print(pro,n);
}


//����ҵ�����㷨
void sjf(struct process pro[],int n)
{
	int i;
	pro=sortarrivetime(pro,n);
	pro[0].finishtime=pro[0].arrivetime+pro[0].servetime;
	for(i=1;i<n;i++)
	{
		if(pro[i].arrivetime>pro[i-1].finishtime)
		{
			pro[i].finishtime=pro[i].arrivetime+pro[i].servetime;
		}
		else
		{
			int k=0;
			for(int m=i;m<n;m++)//��i-1���������ʱ���ҳ�k�������Ѿ���� 
			{
				if(pro[i].arrivetime<=pro[i-1].finishtime)
					k++;
			}
			struct process min,temp;
			min.servetime=pro[i].servetime;
			int x=i;
			for(int l=i+1;l<i+k;l++)//�ڵ����K���������ҳ�servetime��С��
			{
				if(min.servetime>pro[l].servetime)
				{
					min.servetime=pro[l].servetime;
					x=l;
				}
			}
			//����
			temp=pro[i];
			pro[i]=pro[x];
			pro[x]=temp;
			pro[i].finishtime=pro[i-1].finishtime+pro[i].servetime; 
		}
	}
	for(i=0;i<n;i++)
	{
		pro[i].roundtime=pro[i].finishtime-pro[i].arrivetime;
		pro[i].droundtime=pro[i].roundtime/pro[i].servetime;
	}
	printf("\n\n\n����ҵ���ȣ�\n\n");
	print(pro,n);
}
