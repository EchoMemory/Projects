/*                              *
* author: 茅立勋                *
* ID: 200926680110              *
* solution: 银行家算法          *
*                               */
#include <iostream>
#include <cstdio>
#include <cmath>
#include <queue>
#include <cstring>
#include <string>
#include <windows.h>
#include <algorithm>
using namespace std;
#define NUM 50          //最大资源数
#define PRO 50          //最大进程数
int claim[PRO][NUM];    //进程i对进程j的需求
int resource[NUM];      //每种资源总数
int req[NUM];           //请求的资源数
struct state
{ 
	int available[NUM];     //未分配给进程的每种资源的可用总量
	int alloc[PRO][NUM];    //当前进程i有的资源j的数量
};
state st;                 //初始状态
int ask,n,m;              //请求的进程编号，进程数，资源数
bool safe(state cur)      //搜索所有情况，如果都不满足条件，不安全
{
	bool finish[PRO];
	for(int i=1;i<=n;i++) finish[i]=0;    
	bool flag=1;
	int holdavailable[NUM];
	memcpy(holdavailable+1,cur.available+1,sizeof(int)*m);
	while(flag)
	{
		int find=0;
		for(int i=1;i<=n;i++)
		{
			if(!finish[i])
			{
				find=i;
				for(int j=1;j<=m&&find;j++)  
				{
					if(claim[i][j]-cur.alloc[i][j]>holdavailable[j]) find=0;
				}
				if(find) break;
			}
		}
		if(find)
		{
			finish[find]=1;
			for(int j=1;j<=m;j++) holdavailable[j]+=cur.alloc[find][j];
		}
		else flag=0;
	}
	bool ok=1;
	for(int i=1;i<=n&&ok;i++) ok&=finish[i];
	return ok;
}
void Banker()
{
	bool flag=1,next;
	while(flag)
	{
		printf("请输入现在这个请求是哪个进程提出的？:\n",ask);
		scanf("%d",&ask);
		printf("请输入这个进程对%d个资源的请求:\n",m);
		for(int i=1;i<=m;i++)  scanf("%d",&req[i]);
		state temp=st;
		next=1;
		for(int j=1;j<=m;j++)
		{
			if(st.alloc[ask][j]+req[j]>claim[ask][j])
			{
				next=0;
				puts("ERROR,请重新输入数据");
				break;
			}
			else if(req[j]>st.available[j])
			{
				next=0;
				printf("该请求由于资源不够现在无法完成，请求被拒绝\n");
				break;
			}
		}
		for(int j=1;j<=m;j++)
		{
			temp.alloc[ask][j]+=req[j];
			temp.available[j]-=req[j];
		}
		if(next)
		{
			if(safe(temp))   printf("没有发生死锁,进程顺利执行完毕!\n");
			else  printf("发生死锁!\n\a");
		}
		printf("您还想再次请求分配吗?是请按y/Y,否请按其它键: ");
		char again[2];
		scanf("%s",again);
		if(again[0]=='y'||again[0]=='Y')  flag=1;
		else 
		{
			printf("该次银行家算法结束\n");
			puts("--------------------------------------------------------");
			flag=0;
		}
	}
}
int main()
{
	int i,j,k,l,t,g=1;
	printf("----------------------Case %d:-----------------------------\n",g++);
	printf("请输入总共的进程数目和资源的数目（都为0时退出）: ");
	while(scanf("%d%d",&n,&m)!=EOF&&(n||m))
	{
		puts("请输入每一个进程对每一种资源的目前占有数:");
		for(i=1;i<=n;i++)  
		{
			printf("进程%d: ",i);	
			for(j=1;j<=m;j++)  scanf("%d",&st.alloc[i][j]);
		}
		puts("请输入每一个进程对每一种资源的需求数:");
		for(i=1;i<=n;i++)  
		{
			printf("进程%d: ",i);	
			for(j=1;j<=m;j++)  scanf("%d",&claim[i][j]);
		}
		puts("请输入每一种资源的总数(以空格隔开):");
		for(j=1;j<=m;j++)  scanf("%d",&resource[j]);

		puts("请输入当前下每一种资源的未被分配的资源数(以空格隔开): ");
		for(j=1;j<=m;j++)  scanf("%d",&st.available[j]);
		Banker();
		printf("--------------------Case %d:------------------------\n",g++);
		printf("\n\n请输入总共的进程数目和资源的数目（都为0时退出）: ");
	}
}