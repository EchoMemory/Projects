#coding:utf-8
import urllib2
from sgmllib import SGMLParser

class mlx:
    url=''
    step=0
    def __init(self):
        pass
    def __init__(self, __url='',__step=0):
        self.url=__url
        self.step=__step

class urllister(SGMLParser):
    def __init__(self):
        SGMLParser.reset(self)
        self.res = []
        
    def start_a(self,attrs):
        gp = [val for para, val in attrs if 'href' == para]
        for value in gp:
            if ((value not in self.res) and (value.find('script')==-1) and (value.find('mailto')==-1) and (value.find('#')==-1) and len(value)>2):
                if(value[0]=='/' and value !="/"): 
                    value=value[1:]
                value=value.rstrip(' ')
                if (value.find('http')==-1) or (value.find(':')==-1):
                    value = base+value
                if( value not in all_url and value.startswith(base)):
                    all_url.append(value)
                    self.res.append(value)
                
def check(now_url):
    now_url=now_url[:len(now_url)-1]
    if(now_url.endswith('aspx') or now_url.endswith('asp') or now_url.endswith('php') or now_url.endswith('jsp')):
        return True
    else:
        return False
    

def spider(st_url,depth):
    now=mlx(st_url,depth)
    q.append(now)
    while len(q) >0:
        
        #print 'kkkkkkkkkk'
        
        #for i in q: print i.url
        
        
        now=mlx(q[0].url,q[0].step)
        del q[0]
        #print '*********' +now.url
        print now.step,now.url
        if(now.step>dep): return
        try:
            ans = urllib2.urlopen(now.url)
            #print 'aaaaaaaaak'
        except:
            #print 'fkkkkkk'
            continue
        
        data = ans.read()
        lp = urllister()
        lp.feed(data)
        for next_url in lp.res:
            #print "next_url:aaaaaaaaaaaaaaaaaaaaaa"
            hold=mlx(next_url,now.step+1)
            q.append(hold)
            if(next_url.find('?')!=-1 and next_url.find('=')!=-1 ):
                    npos=next_url.index('?')
                    prefix=next_url[:npos+1].lower()
                    suffix=''
                    flag=0
                    for i in range(npos+1,len(next_url)):
                        if(next_url[i]=='='):
                            flag=1
                            continue
                        if(next_url[i]=='&'):
                            flag=0
                            suffix+='#'
                            continue
                        if(flag==0):
                            suffix+=next_url[i].lower()
                   # print 'debug------'
                   # print prefix,suffix
                   # print 'debug------'
                      
                    suffix=prefix+suffix
                    if( (suffix not in str_set) and check(prefix)):
                        ans_url.append(next_url)
                        str_set.append(suffix)
                        #print 'done'

if __name__ == "__main__":     
    q=[]
    str_set=[]
    all_url = []
    ans_url=[]
    file1 = open("ans_url.txt","w")
    file2 = open("all_url.txt","w")
    st_url = "http://www.soft.zjut.edu.cn/index.asp"
    base = "http://www.soft.zjut.edu.cn/"
    dep=4
    spider(st_url,0)
    for one in ans_url:
        #print one+"\n"
        file1.writelines(one)
        file1.write('\n')
    for all in all_url:
        #print all+"\n"
        file2.writelines(all)
        file2.write('\n')
    file1.flush();file1.close()
    file2.flush();file2.close()
    """
    print "-------------left urls queue:\n"
    for i in q: print i.url
    print "-------------"
    print "-------------now str_set queue:\n"
    for i in str_set: print i
    print "-------------"
    """
    input("done_spider")

