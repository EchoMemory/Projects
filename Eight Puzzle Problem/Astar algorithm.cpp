#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;
#define N 362885
struct mlx
{
	char tm;          //记录次数
	int h;            //记录h(n)
	char pos;         //记录现在空格所在位置
	int state;        //hash码
	char now[9];      //记录状态
};
bool operator<(const mlx &a,const mlx &b) 
{
	if(a.tm+a.h==b.tm+b.h) return a.h>b.h;
	return a.tm+a.h>b.tm+b.h;
} 
bool flag1[N];
bool flag2[N];
int pre1[N];
int pre2[N];
char path1[N];
char path2[N];
int fact[9]={40320,5040,720,120,24,6,2,1,1};
const int dxdy[ 10 ][ 4 ] = 
{   
	{-1,-1, 1, 3}, 
	{-1, 0, 2, 4}, 
	{-1, 1,-1, 5}, 
	{ 0,-1, 4, 6}, 
	{ 1, 3, 5, 7}, 
	{ 2, 4,-1, 8}, 
	{ 3,-1, 7,-1},
	{ 4, 6, 8,-1},
	{ 5, 7,-1,-1} 
}; 
const char goal[9]={1,2,3,4,5,6,7,8,9};
const char end[9]={9,8,7,6,5,4,3,2,1};
const char track1[4]={'u','l','r','d'};
const char track2[4]={'d','r','l','u'};
mlx st1,st2,en1,en2;
inline bool ok(char *a)
{
	char cn=0,temp[9];
	for(int i=0;i<9;i++) if(a[i]!=9) temp[cn++]=a[i];
	int num=0;
	for(int i=1;i<8;i++)
		for(int j=0;j<i;j++)
			if(temp[i]>temp[j]) num++;
	return !(num&1);
}
inline int gethash(char *a)
{
	int cc,j,sum=0;
	bool use[10];
	memset(use,0,sizeof(use));
	for(int i=0;i<9;i++) 
	{
		for(cc=0,j=1;j<a[i];j++) if(!use[j]) cc++;
		sum+=fact[i]*cc;
		use[a[i]]=1;
	}
	return sum;
}
inline int getdis(char *a)
{
	int man=0,k;
	for(int i=0;i<9;i++) man+=abs((a[i]+8)%9%3-i%3)+abs((a[i]+8)%9/3-i/3);
	return man*25;
} 
void DAstar()
{
	memset(flag1,0,sizeof(flag1));
	memset(flag2,0,sizeof(flag2));
	st1.state=gethash(st1.now);
	st2.state=0;
	pre1[st1.state]=-1;
	pre2[0]=-1;
	flag1[st1.state]=1;
	flag2[0]=1;
	priority_queue<mlx> q[2];
	q[].push(st1);
	while(!q.empty())
	{
		en=q.top();
		q.pop();
		if(en.state==0) return ;
		en.tm++;
		char x=en.pos;
		for(int i=0;i<4;i++)
		{
			if(dxdy[x][i]!=-1)
			{
				st=en;
				st.pos=dxdy[x][i];
				swap(st.now[x],st.now[dxdy[x][i]]);
				if(ok())
				{
					gethash();
					int stnow=st.state;
					if(!flag[stnow])
					{
						getdis();
						flag[stnow]=1;
						pre[stnow]=en.state;
						path[stnow]=track[i];
						q.push(st);
					}
				}
			}
		}
	}
}
void print(int n)
{
	if(pre[n]!=-1)
	{
		print(pre[n]); 
		printf("%c",path[n]);
	}
}
int main()
{
	int cas,ptr,i,j,k;
	char *in=st1.now;
	while(scanf("%s",&in[0])!=EOF)
	{
		scanf("%s%s%s%s%s%s%s%s",&in[1],&in[2],&in[3],&in[4],&in[5],&in[6],&in[7],&in[8]);
		for(st2.tm=st1.tm=0,j=0;j<9;j++) 
		{
			if(in[j]=='x') 
			{
				in[j]='9';
				st1.pos=j;
			}
			in[j]-=48;
			st2.now[j]=j+1;
		}
		if(ok(in)) 
		{
			DAstar();
			print(0);
			puts("");
		}
		else  puts("unsolvable");
	}
}