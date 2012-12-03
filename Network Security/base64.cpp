#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <conio.h>
#include <string>
#include <cstring>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
using namespace std;
char hold[10];
char c[100001];
char ori[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";  
map<char,int> tr;
int getVal( string &s,int bit) 
{
	int sum=0;
	reverse(s.begin(),s.end());
	for(int i=0;i<bit;i++) sum+=((s[i]-48)*(1<<i));
	return  sum;
}
void Code(char *c)
{
	int flag=0;
	string temp,to="",res="",str="";
	for(int i=0;i<strlen(c);i++)
	{
		itoa((int)c[i],hold,2);
		temp=hold;
		while(temp.size()<8) temp.insert(temp.begin(),'0');
		to+=temp;
	}
	flag=to.size()%3;
	for(int i=1;i<=2*flag;i++) to+="0";
	for(int i=0;i<=to.size()-6;i+=6)
	{
		str=to.substr(i,6);
		res.push_back(ori[getVal(str,6)]);
	}
	while(flag--) res+="=";
	printf("%s\n",res.c_str());
}
void Decode(char *c)
{
	string temp,to="",res="",str="";
	for(int i=0;i<strlen(c);i++)
	{
		if(c[i]!='=') 
		{
			itoa(tr[c[i]],hold,2);
			temp=hold;
			while(temp.size()<6) temp.insert(temp.begin(),'0');
			to+=temp;
		}
		else
		{
			if(c[i+1]=='=') 
			{
				i++;
				to=to.substr(0,to.size()-4);
			}
			else to=to.substr(0,to.size()-2);
		}
	}
	for(int i=0;i<=to.size()-8;i+=8)
	{
		str=to.substr(i,8);
		res.push_back(getVal(str,8));
	}
	printf("%s\n",res.c_str());
}
int main()
{
	int i,j;
	for(i=0;i<strlen(ori);i++)  tr[ori[i]]=i;
	while(scanf("%s",c)!=EOF)
	{	
		/*
		switch(getch())
		{
		case '1':Code(c);
		break;
		case '2':Decode(c);
		}
		*/
		//Code(c);
		//Decode(c);
	}
	return 0;
}