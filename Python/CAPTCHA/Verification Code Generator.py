""""
Author: Mlx
Time:	2011-12-31
"""
import random
import string
import StringIO
from PIL import Image, ImageDraw, ImageFont, ImageFilter
 
flag=1                                     #标记
charset=""                                 #符号源
img_size=(250,40)                          #图片大小
width,height=img_size                      #长，宽
font_size=22                               #字体大小
font_type='D:/calibri.ttf'                 #字体类型
bg_color=(255,255,255)                     #背景颜色
fg_color=(0,0,255)                         #字体颜色
d_color=(0,0,0)                            #干扰颜色
dir='D:/'                                  #保存路径
length=5                                   #验证码的字符数
lnum=5                                     #干扰线条数
pnum=100                                   #干扰点数
proba=2                                    #随机概率因子
pic=Image.new('RGB',img_size,bg_color)     #图片
draw=ImageDraw.Draw(pic)                   #画笔
 
def init():
    """
    初始化
    """
    global charset
    lower="".join(map(chr,range(97,122)))
    upper=lower.upper()
    num="".join(map(str,range(0,10)))
    charset= "".join(random.sample(lower+upper+num,length))
 
def MlxTeachuDraw():
    """
    画画
    """
    global charset,flag,pic,draw
    charset='2011Memory'
    charset = charset[0]+"".join([' '+charset[x] for x in range(1,len(charset))])
    hold=ImageFont.truetype(font_type,font_size)
    draw.text((10,2),charset,font=hold,fill=fg_color)
    save()
 
def chaos():
    def transform():
        """
        变换,还不太明白这个变换
        """
        global pic,draw
        params = [1 - float(random.randint(1, 2)) / 100,0,0,0,1 - float(random.randint(1, 10)) / 100,float(random.randint(1, 2)) / 500,0.001,float(random.randint(1, 2)) / 500]
        pic = pic.transform(img_size, Image.PERSPECTIVE, params)
        draw=ImageDraw.Draw(pic)
        save()
    def addpoint():
        """
        增加干扰点
        """
        global proba,draw
        for i in range(width):
            for j in range(height):
                tmp=random.randint(0,100)
                if(tmp<=proba):
                    draw.point((i,j),d_color)
        save()
    def addline():
        """
        增加干扰线
        """
        global lnum,draw
        for i in range(lnum):
            x1 = random.randint(0,width)
            x2 = random.randint(0,width)
            y1 = random.randint(0,height)
            y2 = random.randint(0,height)
            draw.line(((x1,y1),(x2,y2)),d_color)
        save()
 
    transform()
    addpoint()
    addline()
    flush()
 
def flush():
    """
    打磨润色
    """
    global pic
    pic=pic.filter(ImageFilter.EDGE_ENHANCE_MORE)
    pic=pic.filter(ImageFilter.SMOOTH)
    save()
 
def save():
    """
    保存中间结果
    """
    global flag,pic
    pic.save(dir+'%d.png' % flag )
    flag+=1
 
def store():
    """
    存储图片
    """
    global pic
    mem=StringIO.StringIO()
    pic.save(mem,"GIF")
 
if __name__=='__main__':
    init()
    MlxTeachuDraw()
    chaos()
    store()
    pic.show()
    input()