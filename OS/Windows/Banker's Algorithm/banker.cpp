/*                              *
* author: é��ѫ                *
* ID: 200926680110              *
* solution: ���м��㷨          *
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
#define NUM 50          //�����Դ��
#define PRO 50          //��������
int claim[PRO][NUM];    //����i�Խ���j������
int resource[NUM];      //ÿ����Դ����
int req[NUM];           //�������Դ��
struct state
{ 
	int available[NUM];     //δ��������̵�ÿ����Դ�Ŀ�������
	int alloc[PRO][NUM];    //��ǰ����i�е���Դj������
};
state st;                 //��ʼ״̬
int ask,n,m;              //����Ľ��̱�ţ�����������Դ��
bool safe(state cur)      //����������������������������������ȫ
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
		printf("��������������������ĸ���������ģ�:\n",ask);
		scanf("%d",&ask);
		printf("������������̶�%d����Դ������:\n",m);
		for(int i=1;i<=m;i++)  scanf("%d",&req[i]);
		state temp=st;
		next=1;
		for(int j=1;j<=m;j++)
		{
			if(st.alloc[ask][j]+req[j]>claim[ask][j])
			{
				next=0;
				puts("ERROR,��������������");
				break;
			}
			else if(req[j]>st.available[j])
			{
				next=0;
				printf("������������Դ���������޷���ɣ����󱻾ܾ�\n");
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
			if(safe(temp))   printf("û�з�������,����˳��ִ�����!\n");
			else  printf("��������!\n\a");
		}
		printf("�������ٴ����������?���밴y/Y,���밴������: ");
		char again[2];
		scanf("%s",again);
		if(again[0]=='y'||again[0]=='Y')  flag=1;
		else 
		{
			printf("�ô����м��㷨����\n");
			puts("--------------------------------------------------------");
			flag=0;
		}
	}
}
int main()
{
	int i,j,k,l,t,g=1;
	printf("----------------------Case %d:-----------------------------\n",g++);
	printf("�������ܹ��Ľ�����Ŀ����Դ����Ŀ����Ϊ0ʱ�˳���: ");
	while(scanf("%d%d",&n,&m)!=EOF&&(n||m))
	{
		puts("������ÿһ�����̶�ÿһ����Դ��Ŀǰռ����:");
		for(i=1;i<=n;i++)  
		{
			printf("����%d: ",i);	
			for(j=1;j<=m;j++)  scanf("%d",&st.alloc[i][j]);
		}
		puts("������ÿһ�����̶�ÿһ����Դ��������:");
		for(i=1;i<=n;i++)  
		{
			printf("����%d: ",i);	
			for(j=1;j<=m;j++)  scanf("%d",&claim[i][j]);
		}
		puts("������ÿһ����Դ������(�Կո����):");
		for(j=1;j<=m;j++)  scanf("%d",&resource[j]);

		puts("�����뵱ǰ��ÿһ����Դ��δ���������Դ��(�Կո����): ");
		for(j=1;j<=m;j++)  scanf("%d",&st.available[j]);
		Banker();
		printf("--------------------Case %d:------------------------\n",g++);
		printf("\n\n�������ܹ��Ľ�����Ŀ����Դ����Ŀ����Ϊ0ʱ�˳���: ");
	}
}