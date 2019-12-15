#include <cstdio>
#include <cstring>
#include <cmath>

const int MAXN = 100;                           //地图规模
const double COSA = 0.5;

int s, p, d, h;                                 //扫描线数为s,海拔测试点数为p、等边三角形的边长为d,
                                                        //两条相邻的等高线之间的海拔增量为h
int a[MAXN][MAXN];                              //a[i][j]为第主条扫描线上第j个测试点的海拔测量数据
double ans;                                     //地图上所有等高线的总长度
void init()                                     //输入海拔测量信息
{
    scanf("%d %d %d", &p, &d, &h);              //输入海拔测试点的数目、等边三角形的边长和两条相邻的等高线之间的海拔量
    int i, j;
    for (i = 1; i <= s; ++i) //输入每条扫描线上各测试点的海拔测量数据
    {
        for (j = 1; j <= p + 1 - (i & 1); ++j)
            scanf("%d", &a[i][j]);
    }
}

void sortVertex(int &a, int &b)                   //按照递增顺序排列a和b
{
    if (a > b)
    {
        int t = a;
        a = b;
        b = t;
    }
}

inline double getThird(double a, double b)        //已知三角形的边长a和边长b,求第三条边长
{
    return sqrt(a * a + b * b - 2 * a * b * COSA);
}

inline double getByArithmetic(int a, int b, int c, int l, int r)
{
    //计算三角形一部分的等高线
    if (l > r)
        return 0;
    double L = getThird(double(l - a) / double(b - a) * d, double(l - a) / double(c - a) * d);
    double R = getThird(double(r - a) / double(b - a) * d, double(r - a) / double(c - a) * d);

    return (L + R) * ((r - l) / h + 1) / 2; //等差数列求和
}

double getLength(int a, int b, int c) //计算一个三角形内部的等高线
{
    //排序3个顶点的高
    sortVertex(a, b);
    sortVertex(b, c);
    sortVertex(a, b);
    int l, m1, m2, r;
    double ret;
    if (a == c) //如果3个顶点的高相等,直接返回边长d*1.5
    {
        if (a % h == 0)
            return d * 1.5;
    }

    if (a == b) //低的两角相等
    {
        l = (a / h) * h;
        if (l < a)
            l += h;
        r = (c / h) * h;
        ret = getByArithmetic(-c, -b, -a, -r, -l);
    }
    else if (b == c)
    //高的两角相等
    {
        l = (a / h) * h + h;
        r = (c / h) * h;
        ret = getByArithmetic(a, b, c, l, r);
    }
    else //3个角均不相等
    {
        l = (a / h) * h + h;
        m2 = (b / h) * h + h;
        m1 = m2 - h;
        r = (c / h) * h;
        if (r == c)
            r -= h;
        ret = getByArithmetic(a, b, c, l, m1) + getByArithmetic(-c, -b, -a, -r, -m2);
    }
    if ((b % h == 0) && ((a == b) || (b == c)))
        ret -= d * 0.5;//如果有边上的等高线,减去一半
    
    return ret;
}

bool equal(int a, int b, int c, int d) //返回a、b,c、d四数相等的标志
{
    return (a == b) && (b == c) && (c == d);
}

void solve() //计算和输出地图上所有等高线的总长度
{
    ans = 0;
    int i, j;
    for (i = 1; i < s; i += 2)
    { //累加奇数行的
        for (j = 1; j < p; ++j)
        {
            ans += getLength(a[i][j], a[i + 1][j], a[i + 1][j + 1]);
            ans += getLength(a[i][j], a[i][j + 1], a[i + 1][j + 1]);
        }
        ans += getLength(a[i][p], a[i + 1][p], a[i + 1][p + 1]);
        if ((a[i][1] == a[i + 1][1]) && (a[i][1] % h == 0))
            ans += d * 0.5; //加上边界上的
        if ((a[i][p] == a[i + 1][p + 1]) && (a[i][p] % h == 0))
            ans += d * 0.5;
    }
    for (i = 2; i < s; i += 2)
    { //累加偶数行的
        for (j = 1; j < p; ++j)
        {
            ans += getLength(a[i][j],a[i+1][j],a[i][j+1]);
            ans += getLength(a[i + 1][j + 1], a[i][j + 1], a[i + 1][j]);
        }
        ans+= getLength(a[i][p],a[i+1][p],a[i][p+1]);
        if ((a[i][1] == a[i + 1][1]) && (a[i][1] % h == 0))
            ans += d * 0.5; //加上边界上的
        if ((a[i][p + 1] == a[i + 1][p]) && (a[i][p + 1] % h == 0))
            ans += d * 0.5;
    }

    for (i = 1; i < p; ++i) //加上上边界上的
        if ((a[1][i] == a[1][i + 1]) && (a[1][i] % h == 0))
            ans += d * 0.5;
    for (i = 1; i < p + 1 - (s & 1); ++i) //加下上边界上的
        if ((a[s][i] == a[s][i + 1]) && (a[s][i] % h == 0))
            ans += d * 0.5;
    for (i = 1; i < s; i += 2) //检查奇数行上平地之间的边
    {
        for (j = 1; j < p; ++j)
            if (a[i][j] % h == 0)
            {
                if (equal(a[i][j], a[i + 1][j], a[i][j + 1], a[i + 1][j + 1]))
                    ans -= d;
                if (equal(a[i][j], a[i][j + 1], a[i + 1][j + 1], a[i + 1][j + 2]))
                    ans -= d;
            }
        if (i < s - 1)
            for (j = 1; j <= p; ++j)
                if (a[i][j] % h == 0)
                    if (equal(a[i][j], a[i + 1][j], a[i + 2][j], a[i + 1][j + 1]))
                        ans -= d;
    }
    for (i = 2; i < s; i += 2) //检查偶数行上平地之间的边
    {
        for (j = 1; j < p; ++j)
            if (a[i][j + 1] % h == 0)
            {
                if (equal(a[i][j], a[i + 1][j], a[i][j + 1], a[i + 1][j + 1]))
                    ans -= d;
                if (equal(a[i + 1][j], a[i][j + 1], a[i + 1][j + 1], a[i][j + 2]))
                    ans -= d;
            }
        if (i < s - 1)
            for (j = 1; j < p; ++j)
                if (a[i][j + 1] % h == 0)
                    if (equal(a[i][j + 1], a[i + 1][j + 1], a[i + 2][j + 1], a[i + 1][j]))
                        ans -= d;
    }
    printf("%.0f\n", ans); //输出地图上所有等高线的总长度
}
int main()
{
    int CASE = 0;                         //测试用例编号初始化
    while ((scanf("%d", &s) != EOF) && s) //反复输入扫描线数,直至输入0为止
    {
        init();                    //输入海拔测量信息
        printf("Case %d: ", ++CASE); //输出测试用例编号
        solve();                   //计算和输出地图上所有等高线的总长度
    }
    return 0;
}