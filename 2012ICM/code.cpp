/***
Author: Mlx
Date:   2012-2-13
***/

#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <conio.h>
#include <cstdio>
#include <deque>
#include <queue>
using namespace std;
#define PERSONS 83
#define TOPICS 15
#define MESSAGES 20000
#define LAST 99999999

struct mlx
{
	int next,to;
};
int eid;							 //  edge number
int head[PERSONS+1];          //  head list
mlx adjacency_list[MESSAGES];//  edges
struct res
{
	int id,order;
};
res ans[PERSONS];			    //  ans set
map<string,int> per2num;		//  names mapping  nums
map<int,string> num2per;		//  nums  mapping  names 
vector<int> edge[PERSONS+1][PERSONS+1];  //  messages links
set<string> nocons;				//  record innocent persons number
set<string> cons;			   //  record suspected persons number
map<int,int> tops;			   //  topics mapping value 
vector<int> others;			   //  the person only links to innocents
int Related[PERSONS+1];	   //  topics relation distance
int Potential[PERSONS+1];	   //	 potential relation distance
bool use[PERSONS+1];		   //  mark
int visit[PERSONS+1],fk;	   //  use for dfs
set<int>	susTop;			   //  record suspected topics
int dis[PERSONS+1];			   //  use for clustering


string Rank( int num,int type )
{
	char c[5]={0};
	string s="";
	itoa(num,c,10);
	if(type==1) 
	{
		s=c;
		return s;
	}
	s="(";
	if(num==1) 
		s+="1st";
	else if(num==2) 
		s+="2nd";
	else if(num==3) 
		s+="3rd";
	else 
	{
		s+=c;s+="th";
	}
	s+=")";
	return s;
}
void Clear()
{
	eid=0;
	susTop.clear();
	per2num.clear();
	num2per.clear();
	nocons.clear();        
	cons.clear();			
	tops.clear();	
	for(int i=0;i<=PERSONS;i++) 
	{
		for(int j=0;j<=PERSONS;j++)edge[i][j].clear(); 
		ans[i].id=i,ans[i].order=0;
	}
}
void Read()
{
	int id=0;
	string name,hold;
	ifstream in1("names.txt");
	/*
	read the names inf
	ex: 
	names Id
	*/
	while(!in1.eof())
	{
		in1>>name;
		name=name+"_"+Rank(id,1);
		per2num[name]=id;
		num2per[id++]=name;
	}
	in1.close();

	ifstream in2("nocons.txt");
	/*
	read the noconspirators inf
	ex: 
	noconspiratorsId (to distinguish between duplicate names)
	*/
	name="";
	while(!in2.eof())
	{
		in2>>name;
		if(name=="") break;
		nocons.insert(num2per[atoi(name.c_str())]);
		ans[atoi(name.c_str())].order=LAST;
	}
	in2.close();

	ifstream in3("cons.txt");
	/*
	read the conspirators inf
	ex: 
	conspiratorsId (to distinguish between duplicate names)
	*/
	name="";
	while(!in3.eof())
	{
		in3>>name;
		if(name=="") break;
		cons.insert(num2per[atoi(name.c_str())]);
		ans[atoi(name.c_str())].order=LAST;
	}
	in3.close();

	ifstream in4("topic.txt");
	/*
	read the topic inf
	ex: 
	topicId(with * is suspicious)  Priority
	*/
	string topic;
	while(!in4.eof())
	{
		getline(in4,hold);
		istringstream line(hold);
		line>>topic;
		if(topic[0]=='*') 
		{
			topic=topic.substr(1);
			susTop.insert(atoi(topic.c_str()));
		}
		line>>hold;
		tops[atoi(topic.c_str())]=atoi(hold.c_str());
	}
	in4.close();

	ifstream in5("edges.txt");
	/*
	read the message inf
	ex: 
	fromId toId topicType1 topicType2 topicType3 
	*/
	int from,to;
	while(!in5.eof())
	{
		getline(in5,hold);
		istringstream line(hold);
		line>>from>>to;
		/*
		if the person id is greater than 82 ,then it's an error.
		*/
		if(from>=PERSONS||to>=PERSONS) continue;
		/*
		Consider that the graph is an Acyclic graph.Regularize the graph.
		*/
		if(from==to) continue;
		if(nocons.find(num2per[from])!=nocons.end()||nocons.find(num2per[to])!=nocons.end()) continue;
		while(line>>topic)
		{
			int kkk=atoi(topic.c_str());
			/* 
			if the topic id is greater than 15,then it's an error.
			*/
			if(kkk>TOPICS) continue;          
			edge[from][to].push_back(kkk);
		}
	}
	in5.close();

	for(int i=0;i<PERSONS;i++)
	{
		head[i]=-1;
		for(int j=0;j<PERSONS;j++)
		{
			sort(edge[i][j].begin(),edge[i][j].end());
			/* 
			To avoid redundancies,unique the information.
			*/
			unique(edge[i][j].begin(),edge[i][j].end());
			if(!edge[i][j].empty())
			{
				adjacency_list[eid].next=head[i];
				adjacency_list[eid].to=j;
				head[i]=eid++;

				
			}
		}
	}
}
void Init()
{
	Clear();
	Read();
}
void Dfs(int pos,int step)
{
	if(visit[pos]==2) 
	{
		fk=min(fk,step);
		return;
	}
	for(int i=head[pos];i!=-1;i=adjacency_list[i].next)
	{
		int hh=adjacency_list[i].to;
		if( (!visit[hh]||visit[hh]==2)&&step+1<fk)
		{
			visit[hh]=1;
			Dfs(hh,step+1);
			visit[hh]=0;
		}
	}
}
void Calculate(set<int> &group)
{
	memset(Related,0,sizeof(Related));
	memset(Potential,0,sizeof(Potential));
	for(int i=0;i<PERSONS;i++) 
	{
		if(nocons.find(num2per[i])!=nocons.end()||cons.find(num2per[i])!=cons.end()) continue;
		memset(visit,0,sizeof(visit));
		for(int j=0;j<PERSONS;j++) 
		{
			if(group.find(j)!=group.end()) visit[j]=2;
			else visit[j]=0;
		}
		fk=6;
		visit[i]=1;
		Dfs(i,0);
		Potential[i]=6-fk;
	}
	for(int i=0;i<PERSONS;i++)
	{
		for(int j=i+1;j<PERSONS;j++)
		{
			if(nocons.find(num2per[i])!=nocons.end()||cons.find(num2per[i])!=cons.end()) continue;
			if(nocons.find(num2per[j])!=nocons.end()||cons.find(num2per[j])!=cons.end()) continue;
			for(int k=0;k<edge[i][j].size();k++) 
			{
				if(susTop.find(edge[i][j][k])!=susTop.end())
					Related[i]+=tops[edge[i][j][k]];
			}
			for(int k=0;k<edge[j][i].size();k++) 
			{
				if(susTop.find(edge[j][i][k])!=susTop.end())
					Related[i]+=tops[edge[j][i][k]];
			}
		}
	}
}
void Pre_Cluster(set<int> &group )
{
	memset(use,0,sizeof(use));
	others.clear();
	for(set<string>::iterator ii=cons.begin();ii!=cons.end();ii++){
		use[per2num[*ii]]=1;group.insert(per2num[*ii]);
	}
	for(set<string>::iterator ii=nocons.begin();ii!=nocons.end();ii++){
		use[per2num[*ii]]=1;
	}
}
void Cluster()
{
	set<int> group;
	int pp=1;
	Pre_Cluster(group);
	for(int i=cons.size()+nocons.size();i<PERSONS;)
	{
		int temp=-1,flag=0;
		vector<int> hold;
		memset(dis,0,sizeof(dis));
		for(int j=0;j<PERSONS;j++)
		{
			if(!use[j])
			{
				flag=1;
				for(set<int>::iterator ii=group.begin();ii!=group.end();ii++)
				{
					for(int k=0;k<(int)edge[*ii][j].size();k++) {
						dis[j]+=tops[edge[*ii][j][k]];
					}
					for(int k=0;k<(int)edge[j][*ii].size();k++) {
						dis[j]+=tops[edge[j][*ii][k]];
					}
				}
			}
		}
		if(flag) 
		{
			for(int j=0;j<PERSONS;j++)
			{
				if(!use[j])
				{
					if(dis[j]>temp) 
					{
						hold.clear();
						hold.push_back(j);
						temp=dis[j];
					}
					else if(dis[j]==temp) 
						hold.push_back(j);
				}
			}
			for(int j=0;j<hold.size();j++)
			{
				use[hold[j]]=1;
				group.insert(hold[j]);
				ans[hold[j]].order=pp;
			}
			i+=hold.size();
			pp++;
		}
		else break;
	}
	for(int i=0;i<PERSONS;i++) 
	{
		if(!use[i]) 
			others.push_back(i);
	}
}
void Heuristic_Cluster()
{
	set<int> group;
	int pp=1;
	Pre_Cluster(group);
	Calculate(group);
	for(int i=cons.size()+nocons.size();i<PERSONS;)
	{
		int temp=-1,flag=0;
		vector<int> hold;
		memset(dis,0,sizeof(dis));
		for(int j=0;j<PERSONS;j++)
		{
			if(!use[j])
			{
				flag=1;
				for(set<int>::iterator ii=group.begin();ii!=group.end();ii++)
				{
					for(int k=0;k<(int)edge[*ii][j].size();k++) 
						{
							dis[j]+=tops[edge[*ii][j][k]];
					}
					for(int k=0;k<(int)edge[j][*ii].size();k++) 
						{
							dis[j]+=tops[edge[j][*ii][k]];
					}
				}
			}
		}
		if(flag) 
		{
			for(int j=0;j<PERSONS;j++)
			{
				if(!use[j])
				{
					if(dis[j]+Potential[j]+Related[j]>temp) 
					{
						hold.clear();
						hold.push_back(j);
						temp=dis[j]+Potential[j]+Related[j];
					}
					else if(dis[j]+Potential[j]+Related[j]==temp) 
						hold.push_back(j);
				}
			}
			for(int j=0;j<hold.size();j++)
			{
				use[hold[j]]=1;
				group.insert(hold[j]);
				ans[hold[j]].order=pp;
			}
			i+=hold.size();
			pp++;
		}
		else break;
	}
	for(int i=0;i<PERSONS;i++) 
	{
		if(!use[i]) 
			others.push_back(i);
	}
}
bool cmpOrder(const res &a,const res &b)
{
	return a.order<b.order;
}
void print(char *c)
{
	sort(ans,ans+PERSONS,cmpOrder);
	cout<<"\t||-------------------";
	printf("The %s",c);
	cout<<"-----------------||\n";

	cout<<"\tThe conspirators:\n";
	cout<<"\t||----  \n\t";
	for(set<string>::iterator ii=cons.begin();ii!=cons.end();ii++) {
		cout<<*ii<<"\t";
	}
	cout<<"\n\t||----\n\n\n";

	cout<<"\tThe Priority list (desending) is :\n";
	cout<<"\t||----  \n";
	int pp=1;
	cout<<"\t     "<<Rank(pp,2)<<":        "<<num2per[ans[0].id];
	for(int i=1;i<PERSONS;i++) 
	{
		if(ans[i].order==pp) 
		{
			cout<<"\t"<<num2per[ans[i].id];
		}
		else if(ans[i].order==LAST) break;
		else
		{
			pp++;
			cout<<"\n\t     "<<Rank(pp,2)<<":        ";
			cout<<num2per[ans[i].id];
		}
	}
	cout<<"\n\t||----\n\n\n";

	cout<<"\tThe person only link to innocents:\n";
	cout<<"\t||----  \n\t";
	for(int i=0;i<others.size();i++)  
	{
		if(i) cout<<"\t";
		cout<<num2per[others[i]];
	}
	cout<<"\n\t||----\n\n\n";

	cout<<"\tThe innocents:\n";
	cout<<"\t||----  \n\t";
	for(set<string>::iterator ii=nocons.begin();ii!=nocons.end();ii++) 
	{
		if(ii!=nocons.begin()) cout<<"\t";
		cout<<*ii;
	}
	cout<<"\n\t||----\n";

	cout<<"\t||------------------------------------------------------||\n";
}
int main()
{
	Init();
	//Cluster();
	//print("Cluster");
	Heuristic_Cluster();
	print("Heuristic_Cluster");
	system("pause");
}

