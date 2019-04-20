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
	float yxq; //优先权
};
struct process pro[100];

void fcfs(struct process pro[],int n);
void sjf(struct process pro[],int n);
struct process *sortarrivetime(struct process pro[],int n); 
void print(struct process pro[],int n);

void main()
{
	int n,i;
	printf("请输入有几个进程：\n");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		fflush(stdin);
		printf("请输入第%d个进程名称(char):\n",i+1); scanf("%c",&pro[i].pname);
        printf("到达时间\n");
		scanf("%f",&pro[i].arrivetime);
		printf("服务时间\n");
		scanf("%f",&pro[i].servetime);
	}
	fcfs(pro,n);
	sjf(pro,n);
}

//按到达时间进行冒泡法排序
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
				flag=1; //交换标志
			}
		}
		if(flag==0) //如果一趟排序中没发生任何交换，则排序结束 
			break;
	}
	return pro;
}


//算平均数+输出函数
void print(struct process pro[],int n)
{
	int i;
	float Sumroundtime=0,Sumdroundtime=0;
	float averoundtime,avedroundtime;
	for(i=0;i<n;i++)//算平均值
	{
		Sumroundtime+=pro[i].roundtime;
		Sumdroundtime+=pro[i].droundtime;
	}
	averoundtime=Sumroundtime/n;
	avedroundtime=Sumdroundtime/n;
	printf("进程名\t到达时间\t服务时间\t完成时间\t周转时间\t带全周转"); 
	for(i=0;i<n;i++)//输出
	{
		printf("%c\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",pro[i].pname,pro[i].arrivetime,
			         pro[i].servetime,pro[i].finishtime,pro[i].roundtime,pro[i].droundtime);
	}
	printf("平均值\t\t\t\t\t\t\t%.2f\t\t%.2f\n",averoundtime,avedroundtime); 
}


//先来先服务算法
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
	printf("\n\n先来先服务：\n\n");
	print(pro,n);
}


//短作业优先算法
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
			for(int m=i;m<n;m++)//第i-1个进程完成时，找出k个进程已经到达。 
			{
				if(pro[i].arrivetime<=pro[i-1].finishtime)
					k++;
			}
			struct process min,temp;
			min.servetime=pro[i].servetime;
			int x=i;
			for(int l=i+1;l<i+k;l++)//在到达的K个进程中找出servetime最小的
			{
				if(min.servetime>pro[l].servetime)
				{
					min.servetime=pro[l].servetime;
					x=l;
				}
			}
			//排序
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
	printf("\n\n\n短作业优先：\n\n");
	print(pro,n);
}
