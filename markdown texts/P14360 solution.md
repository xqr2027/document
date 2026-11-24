# 题意

给定一个长度数列 $a$ 与一个权值序列 $b$，你可以进行若干次操作：每次选定一个区间 $[l,r]$，花费 $m$ 的代价使 $[l,r]$ 中的数 $-1$。定义这个序列的权值为 $\sum_{i=1}^n[a_i\leq 0]b_i-km$，$k$ 为操作次数。求经过若干次操作后序列最大的权值。

数据范围： $n\leq 5\times10^5$。

# $\mathcal O(nV)$ 做法

把一次操作看作把一块地方的高度加一，则最终的序列就会是一个类似直方图的形式。令 $i$ 处的高度为 $h_i$，则计算权值可理解为在图上从左往右的行动。设当前高度为 $H$，分别有以下两种运动方式：
 
 - $h_i>H$（上山），花费 $(h_i-H)\times m$；
 - $h_i\geq H$（下山或不动），花费 $0$。

于是我们就把比较困难的序列操作转化为了好维护一点的线性决策问题了。

设 $dp[i][j]$ 为已经做完了前 $i$ 个位置，当前高度为 $j$ 的最大权值，显然有转移式

$$
dp[i][j]=\max_{p=0}^V\left(dp[i-1][p]-\text{cost}(p,j)\right)
$$

其中

$$
\text{cost}(i,j)=
\begin{cases}
&0,&i\geq j\\
&m(j-i)& \text{otherwise}
\end{cases}
$$

初始状态为 $dp[0][0]=0$，其余均为 $-\infty$，模拟即可。

加上前后缀优化后复杂度 $\mathcal O(nV)$，期望得分 15 分。

代码：

```cpp
constexpr int INF=0x3f3f3f3f3f3f3f3f;
void solve()
{
    static constexpr int V=5000;
    int n,m;cin>>n>>m;
    vector<int>a(n+1);
    vector<int>b(n+1);
    for(int i=1;i<=n;++i)cin>>a[i]>>b[i];
    vector<int>dp(V+1,-INF);
    dp[0]=0;
    for(int i=1;i<=n;++i)
    {
        vector<int>ndp(V+1,-INF);
        vector<int>pre(V+1,-INF);
        vector<int>suf(V+1,-INF);
        for(int j=0;j<=V;++j)
        {
            pre[j]=dp[j]+m*j;
            if(j!=0)pre[j]=max(pre[j],pre[j-1]);
        }
        for(int j=V;j>=0;--j)
        {
            suf[j]=dp[j];
            if(j!=V)suf[j]=max(suf[j],suf[j+1]);
        }
        for(int j=0;j<=V;++j)
        {
            if(j!=0)ndp[j]=max(ndp[j],pre[j-1]-m*j);
            ndp[j]=max(ndp[j],suf[j]);
            if(j>=a[i])ndp[j]+=b[i];
        }
        swap(dp,ndp);
    }
    cout<<*ranges::max_element(dp)<<'\n';
}

```
# $\mathcal O(n\log n)$ 做法

假设当前已经做完了 $1$ 至 $i$ 的状态，现在要求 $i+1$，设 $i+1$分界线 $a_{i+1}$ 为 $H$，对于 $i+1$ 的高度 $v$ 分类讨论：

 - $v>H$，本来就比他高，白赚 $b_i$；
 - $v=H$，从上面降下来（代价 $0$）或从下面升上来（代价 $m(H-h_{old})$），收益 $b_i$；
 - $v=H-1$，从上面降下来（代价 $0$）或从下面升上来（代价 $m(H-h_{old})$），收益 $0$；
 - $v<H-1$，升上去没用，降下去不优 $\to$ 不动。

发现这可以用线段树进行维护，具体的：

 - $v>H$：整段加上 $b_i$；
 - $v=H$：后缀求 $x=\displaystyle\max_{i=H}^Vdp_i$，前缀求 $y=\displaystyle\max_{i=0}^{H-1}(dp_i-m(H-i))$，最后令 $dp_v\to \max(x,y)+b_i$。
 - $v=H-1$，前缀求 $x=\displaystyle\max_{i=H}^Vdp_i$，后缀求 $y=\displaystyle\max_{i=0}^{H-1}(dp_i-m(H-1-i))$，最后令 $dp_v\to\max(x,y)$。
 - $v<H-1$，不动。
  
拍一棵线段树上去即可。

代码：

```cpp
#include<bits/stdc++.h>
using namespace std;
#define int long long
constexpr int INF=0x3f3f3f3f3f3f3f3f;
constexpr int V=1e9;
struct SegTree
{
    private:
    #define mid ((l+r)>>1)
    int m;
    struct info
    {
        int ls,rs;
        int mx1,mx2;// mx1:dp_i,mx2:dp_i+mi
        int add;
        info():ls(0),rs(0),mx1(-INF),mx2(-INF),add(0){}
    };
    vector<info>node;
    int root;
    int newnode()
    {
        node.emplace_back();
        return node.size()-1;
    }
    void pushup(int p)
    {
        int ls=node[p].ls;
        int rs=node[p].rs;
        node[p].mx1=max(node[ls].mx1,node[rs].mx1);
        node[p].mx2=max(node[ls].mx2,node[rs].mx2);
    }
    void apply_add(int p,int add)
    {
        if(!p)return;
        node[p].add+=add;
        node[p].mx1+=add;
        node[p].mx2+=add;
    }
    void pushdown(int p)
    {
        if(!p||node[p].add==0)return;
        if(!node[p].ls)node[p].ls=newnode();
        if(!node[p].rs)node[p].rs=newnode();
        apply_add(node[p].ls,node[p].add);
        apply_add(node[p].rs,node[p].add);            
        node[p].add=0;
    }
    int update_set(int p,int l,int r,int i,int dpi)
    {
        if(!p)p=newnode();
        if(l==r)
        {
            node[p].add=0;
            node[p].mx1=dpi;
            node[p].mx2=dpi+m*i;
            return p;
        }
        pushdown(p);
        if(i<=mid)node[p].ls=update_set(node[p].ls,l,mid,i,dpi);
        else node[p].rs=update_set(node[p].rs,mid+1,r,i,dpi);
        pushup(p);
        return p;
    }
    int update_add(int p,int l,int r,int s,int t,int k)
    {
        if(!p)p=newnode();
        if(s<=l&&r<=t)return apply_add(p,k),p;
        pushdown(p);
        if(s<=mid)node[p].ls=update_add(node[p].ls,l,mid,s,t,k);
        if(t> mid)node[p].rs=update_add(node[p].rs,mid+1,r,s,t,k);
        pushup(p);
        return p;
    }
    int query_mx1(int p,int l,int r,int s,int t)
    {
        if(!p||r<s||t<l)return -INF;
        if(s<=l&&r<=t)return node[p].mx1;
        int res=-INF;
        pushdown(p);
        if(s<=mid)res=max(res,query_mx1(node[p].ls,l,mid,s,t));
        if(t> mid)res=max(res,query_mx1(node[p].rs,mid+1,r,s,t));
        return res;
    }
    int query_mx2(int p,int l,int r,int s,int t)
    {
        if(!p||r<s||t<l)return -INF;
        if(s<=l&&r<=t)return node[p].mx2;
        int res=-INF;
        pushdown(p);
        if(s<=mid)res=max(res,query_mx2(node[p].ls,l,mid,s,t));
        if(t> mid)res=max(res,query_mx2(node[p].rs,mid+1,r,s,t));
        return res;
    }
    public:
    void set_m(int m_){m=m_;}
    void clear(){node.clear();root=newnode();}
    void update_set(int i,int dpi){root=update_set(root,0,V,i,dpi);}
    void update_add(int s,int t,int k){root=update_add(root,0,V,s,t,k);}
    int query_mx1(int s,int t){return query_mx1(root,0,V,s,t);}
    int query_mx2(int s,int t){return query_mx2(root,0,V,s,t);}
    #undef mid
};
SegTree tr;
void solve()
{
    tr.clear();
    int n,m;cin>>n>>m;
    tr.set_m(m);
    tr.update_set(0,0);
    vector<int>a(n+1);
    vector<int>b(n+1);
    for(int i=1;i<=n;++i)cin>>a[i]>>b[i];
    for(int i=1;i<=n;++i)
    {
        int H=a[i];
        int suf1=tr.query_mx1(H,V);
        int pre1=tr.query_mx2(0,H-1)-m*H;
        int suf2=tr.query_mx1(H,V);
        int pre2=tr.query_mx2(0,H-1)-m*H+m;
        tr.update_add(H+1,V,b[i]);
        tr.update_set(H,max(pre1,suf1)+b[i]);
        tr.update_set(H-1,max(pre2,suf2));
    }
    cout<<tr.query_mx1(0,V)<<'\n';
}
signed main()
{
    #ifdef _WIN32
    freopen("qbu.in","r",stdin);
    #endif
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;cin>>T;
    while(T--)solve();
}

```