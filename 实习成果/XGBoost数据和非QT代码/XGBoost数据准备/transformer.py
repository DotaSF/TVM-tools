import os

path = ".\conv2d"
f1 = open('train.txt','w')              #记录训练数据
files = os.listdir(path)             #目录下所有1,2,3,4,5的文件
for i in range(1,33):                   #遍历所有文件，文件1
    s = []
    feature_path = path + '\\' + str(i)+'\\' + "feature.txt"   #构造路径
    label_path = path + '\\' + str(i) +'\\' + "cycle.bin"
    f = open(feature_path,'r')
    for lines in f:                   #遍历所有行
        ls = lines.strip('\n').split(':')[1]  #删除换行，分隔
        s.append(ls)
    f.close()

    ceng=0
    binfile = open(label_path, 'rb') #打开二进制文件
    for line in binfile:
        if(ceng==1):
            l = str(line, encoding="utf-8")
            ms=l[9:17]
            break
        else:
            ceng=ceng+1
    res=str(int(ms[6:8]+ms[4:6]+ms[2:4]+ms[0:2],16))
    s.append(res)
    for j in range(len(s)):
        f1.write(s[j] + ' ')
    f1.write('\n')
f1.close()
