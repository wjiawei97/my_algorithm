#encoding:utf-8
import math

global p,d,h,s

def init(matrix):
    for i in range(s):
        line = input().split(' ')
        for j in range(len(line)):
            matrix[i].append(int(line[j]))

def sortVertex(t1,t2):
    if t1>t2:
        t=t1
        t1=t2
        t2=t

def getThird(a,b):
    return math.sqrt(a*a+b*b-2*a*b*0.5)

def getArithmetic(a,b,c,l,r):#计算三角形一部分的等高线
    if (l > r):
        return 0
    L = getThird((l - a) / (b - a) * d, (l - a) / (c - a) * d)
    R = getThird((r - a) / (b - a) * d, (r - a) / (c - a) * d)
    return (L + R) * ((r - l) / h + 1) / 2 #等差数列求和

def getLength(a,b,c):
    sum=a+b+c
    c=max(a,b,c)
    a=min(a,b,c)
    b=sum-a-c
    if a == c: #如果3个顶点的高相等,直接返回边长d*1.5
        if a % h == 0:
            return d * 1.5
    if a==b: #低的两角相等
        l = (a // h) * h
        if (l < a):
            l += h
        r = (c // h) * h
        ret = getArithmetic(-c, -b, -a, -r, -l)
    elif b==c:  #高的两角相等
        l = (a // h) * h + h
        r = (c // h) * h
        ret = getArithmetic(a, b, c, l, r)
    else:  #3个角均不相等
        l = (a // h) * h + h
        m2 = (b // h) * h + h
        m1 = m2 - h
        r = (c // h) * h
        if (r == c):
            r -= h
        ret = getArithmetic(a, b, c, l, m1) + getArithmetic(-c, -b, -a, -r, -m2)
    if ((b % h == 0) and ((a == b) or (b == c))):
        ret -= d * 0.5 #如果有边上的等高线, 减去一半
    return ret

def equal(a,b,c,d):
    return (a == b) and (b == c) and (c == d)

def solve(grid):
    ans=0
    for i in range(0,s-1,2):  #累加奇数行  ##
        for j in range(0,p-1):
            ans+=getLength(grid[i][j], grid[i + 1][j], grid[i + 1][j + 1])
            ans += getLength(grid[i][j], grid[i][j + 1], grid[i + 1][j + 1])
        ans+=getLength(grid[i][p-1],grid[i+1][p-1],grid[i+1][p])
        if ((grid[i][0] == grid[i + 1][0]) and (grid[i][0] % h == 0)):  ##
            ans += d * 0.5 #加上边界上的
        if ((grid[i][p-1] == grid[i + 1][p]) and (grid[i][p-1] % h == 0)):
            ans += d * 0.5
    for i in range(1,s-1,2):  #累加偶数行 ##
        for j in range(0,p-1):  ##
            ans+=getLength(grid[i][j], grid[i + 1][j], grid[i][j + 1])
            ans+=getLength(grid[i+1][j+1], grid[i][j+1], grid[i + 1][j])
        ans += getLength(grid[i][p-1], grid[i + 1][p-1], grid[i][p])
        if ((grid[i][0] == grid[i + 1][0]) and (grid[i][0] % h == 0)):  ##
            ans += d * 0.5 #加上边界上的
        if ((grid[i][p] == grid[i + 1][p-1]) and (grid[i][p] % h == 0)):
            ans += d * 0.5

        for i in range(0,p-1): #加上上边界上的  ##
            if ((grid[0][i] == grid[0][i + 1]) and (grid[0][i] % h == 0)):  ##
                ans += d * 0.5
        for i in range (0,p + 1 - (s & 1)-1): # 加上上边界上的  ##
            if ((grid[s-1][i] == grid[s-1][i + 1]) and (grid[s-1][i] % h == 0)):
                ans += d * 0.5

        for i in range(0,s-1,2): #检查奇数行上平地之间的边
            for j in range(0,p-1):
                if (grid[i][j] % h == 0):
                    if (equal(grid[i][j], grid[i + 1][j], grid[i][j + 1], grid[i + 1][j + 1])):
                        ans -= d
                    if (equal(grid[i][j], grid[i][j + 1], grid[i + 1][j + 1], grid[i + 1][j + 2])):
                        ans -= d
            if (i < s - 2):
                for j in range(0,p):
                    if (grid[i][j] % h == 0):
                        if (equal(grid[i][j], grid[i + 1][j], grid[i + 2][j], grid[i + 1][j + 1])):
                            ans -= d
        for i in range(1,s-1,2): #检查偶数行上平地之间的边
            for j in range(0,p-1):
                if (grid[i][j+1] % h == 0):
                    if (equal(grid[i][j], grid[i + 1][j], grid[i][j + 1], grid[i + 1][j + 1])):
                        ans -= d
                    if (equal(grid[i+1][j], grid[i][j + 1], grid[i + 1][j + 1], grid[i][j + 2])):
                        ans -= d
            if (i < s - 2):
                for j in range(0,p-1):
                    if (grid[i][j+1] % h == 0):
                        if (equal(grid[i][j+1], grid[i + 1][j+1], grid[i + 2][j+1], grid[i + 1][j])):
                            ans -= d
        print("%.0f"%ans)


if __name__ == '__main__':
    CASE=0
    while True:
        try:
            vars = input().split(' ')
            s = int(vars[0])
        except:
            pass
        if s == 0:
            break
        else:
            CASE+=1
            p = int(vars[1])
            d = int(vars[2])
            h = int(vars[3])
            grid = [[] for i in range(s)]
            init(grid)
            print("Case ",CASE," : ",end="") #输出测试用例编号
            solve(grid) #计算和输出地图上所有等高线的总长度