# 题意

给定一个 $1$ 到 $n$ 的排列 $p_1, p_2, \dots, p_n$。对于 $1 \le i < j \le n$，定义 $\text{sec}(i, j)$ 为 $p_i, p_{i+1}, \dots, p_j$ 中**第二小**的数。

有 $q$ 次询问，每次给出两个整数 $l, r$。你需要求出：

$$ 
\sum_{l \le i < j \le r} \text{sec}(i, j) 
$$

即完全包含于区间 $[l, r]$、长度至少为 $2$ 的所有连续子区间的次小值之和。

当 $l = r$ 时，答案为 $0$。

数据范围：$n,q\leq 2\times 10^5$。

# 题解

## 扫描线建模

硬做不好做，考虑贡献法。

不难发现 $p_i$ 为一个区间的次小值等价于这个区间有且仅有一个数小于它。设 $i$ 左边第 $a$ 个小于 $p_i$ 的数的位置为 $L_a$（若无则为 $0$）， $i$ 右边第 $a$ 个小于 $i$ 的数的位置为 $R_a$（若无则为 $n+1$），则可以发现以下两种区间的次小值均为 $p_i$：

 - $l\in(L_2,L_1]\land r\in[i,R_1)$
 - $l\in(L_1,i]\land r\in[R_1,R_2)$

建立平面直角坐标系，$(x,y)$ 代表子区间 $[x,y]$，则满足 $\sec(x,y)=p_i$ 的点构成两个矩形，相当于给这个矩形内每个数均 $+p_i$，最后查询 $\{(x,y)|x\geq l,y\leq r\}$ 范围内的和即可。

故有答案为 $\text{Ans}(l,r)=\displaystyle\sum_{x\geq l}\sum_{y=1}^r w(x,y)$（$w(x,y)$ 代表点 $(x,y)$ 的点权）。

把 $y$ 看作时间轴把每个操作按 $y$ 排序，则 $\text{Ans}(l,r)$ 就可以视为查询 $r$ 时刻 $l$ 的历史版本和。

## 线段树维护

操作分为两类。设当前区间和为 $\text{sum}$，历史和为 $\text{his}$：

1. 在矩形左边界的区间加，在矩形右边界的区间减，即 $\text{sum}\gets\text{sum}+\text{add}$；
2. 更新历史和，即 $\text{his}\gets\text{his}+\text{sum}$。

故每个节点维护一个向量，$s=\text{sum}$ 为区间和，$h=\text{his}$ 为历史和，则有

$$
\text{vec}=\begin{bmatrix} s \\ h \\ 1 \end{bmatrix}
$$

把操作用矩阵刻画：
1. 区间加/减：对于操作 $s\gets s+v$，可以发现即为乘上矩阵 $M_{\text{add}}=\begin{bmatrix} 1 & 0 & v\\0 & 1 & 0\\ 0 & 0 & 1\end{bmatrix}$

2. 更新历史和：对于操作 $h\gets h+s$，可以发现即为乘上矩阵 $M_{\text{his}}=\begin{bmatrix} 1 & 0 & 0\\1 & 1 & 0\\ 0 & 0 & 1\end{bmatrix}$

发现这些矩阵都有相同的形式 $\begin{bmatrix} 1 & 0 & X\\ Y & 1 & Z\\ 0 & 0 & 1\end{bmatrix}$，这里令为 $A$ 型矩阵，发现 $A$ 型矩阵只有三个变量，且矩阵乘法明显满足结合律，故只需证明 $A$ 型矩阵对于矩阵乘法成含幺半群即可用线段树维护。

证明：

$$

\begin{bmatrix} 
1 & 0 & X_1\\
Y_1 & 1 & Z_1\\ 
0 & 0 & 1
\end{bmatrix}
\times
\begin{bmatrix} 
1 & 0 & X_2\\
Y_2 & 1 & Z_2\\ 
0 & 0 & 1
\end{bmatrix}
=
\begin{bmatrix} 
1 & 0 & X_1+X_2\\
Y_1+Y_2 & 1 & X_2Y_1+Z_1+Z_2\\ 
0 & 0 & 1
\end{bmatrix}
$$

所以 $A$ 对于矩阵乘法封闭，线段树维护即可。

<details>
<summary>代码</summary>

```cpp
#include<bits/stdc++.h>
using namespace std;
#define int long long
using ull=unsigned long long;
struct SegmentTree
{
private:
    #define ls (p<<1)
    #define rs (p<<1|1)
    #define mid ((l+r)>>1)
    struct modify
    {
        ull X,Y,Z;
        modify():X(0),Y(0),Z(0){}
        modify(ull X,ull Y,ull Z):X(X),Y(Y),Z(Z){}
        modify operator*(const modify& b)const
        {
            return modify(
                X+b.X,Y+b.Y,b.X*Y+Z+b.Z
            );
        }
        modify& operator*=(const modify& b){return *this=*this*b;}
        bool empty()const{return X==0&&Y==0&&Z==0;}
    };
    struct info
    {
        int len;
        ull sum,his;
        info():len(0),sum(0),his(0){}
        info(int len,ull sum,ull his):len(len),sum(sum),his(his){}
        info operator*(const modify& b)const
        {
            return info(
                len,sum+b.X*(ull)len,sum*b.Y+his+b.Z*(ull)len
            );
        }
        info operator+(const info& b)const
        {
            return info(
                len+b.len,sum+b.sum,his+b.his
            );
        }
        info& operator*=(const modify& b){return *this=*this*b;}
    };
    modify M_add(int v){return move(modify(v,0,0));}
    modify M_his(){return move(modify(0,1,0));}
    vector<info>val;
    vector<modify>tag;
    void pushup(int p)
    {
        val[p]=val[ls]+val[rs];
    }
    void apply_tag(int p,const modify& tmp)
    {   
        tag[p]=tmp*tag[p];
        val[p]*=tmp;
    }
    void pushdown(int p)
    {
        if(tag[p].empty())return;
        apply_tag(ls,tag[p]);
        apply_tag(rs,tag[p]);
        tag[p]=modify();
    }
public:
    void build(int p,int l,int r)
    {
        val[p].len=r-l+1;
        if(l==r)return;
        build(ls,l,mid),build(rs,mid+1,r);
    }
    void assign(int n){val.assign(n<<2,{}),tag.assign(n<<2,{});}
    void update_add(int p,int l,int r,int s,int t,int k)
    {
        if(s<=l&&r<=t)return apply_tag(p,M_add(k)),void();
        pushdown(p);
        if(s<=mid)update_add(ls,l,mid,s,t,k);
        if(t> mid)update_add(rs,mid+1,r,s,t,k);
        pushup(p);
    }
    void update_his(){apply_tag(1,M_his());}
    ull query(int p,int l,int r,int s,int t)
    {
        if(s<=l&&r<=t)return val[p].his;
        pushdown(p);
        ull res=0;
        if(s<=mid)res+=query(ls,l,mid,s,t);
        if(t> mid)res+=query(rs,mid+1,r,s,t);
        return res;
    }
    #undef ls
    #undef rs
    #undef mid
};
struct event
{
    int l,r;
    int val;
    event():l(0),r(0),val(0){}
    event(int l,int r,int val):l(l),r(r),val(val){}
};
signed main()
{
    freopen("second.in","r",stdin);
    freopen("second.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int c,T;
    cin>>c>>T;
    while(T--)
    {
        int n,q;
        cin>>n>>q;
        SegmentTree tr;
        tr.assign(n);
        tr.build(1,1,n);
        vector<int>p(n+1);
        for(int i=1;i<=n;++i)cin>>p[i];
        vector<int>L1(n+2,0);
        vector<int>R1(n+1,n+1);
        vector<int>L2(n+2,0);
        vector<int>R2(n+1,n+1);
        {
            vector<int>pre(n+2);
            vector<int>nxt(n+2);
            vector<int>pos(n+2);
            for(int i=1;i<=n;++i)pos[p[i]]=i,pre[i]=i-1,nxt[i]=i+1;
            pre[0]=0,nxt[n+1]=n+1;
            for(int i=n;i>=1;--i)
            {
                int k=pos[i];
                L1[k]=pre[k],L2[k]=pre[pre[k]];
                R1[k]=nxt[k],R2[k]=nxt[nxt[k]];
                nxt[pre[k]]=nxt[k],pre[nxt[k]]=pre[k];
            }
        }
        vector<vector<event>>gra(n+2);
        vector<vector<pair<int,int>>>qry(n+1);
        vector<int>ans(q);
        auto add_event=[&](int tim,int l,int r,int val)
        {
            if(l>r)return;
            l=max(l,0ll);
            r=min(r,n);
            gra[tim].emplace_back(l,r,val);
        };
        for(int i=1;i<=n;++i)
        {
            add_event(i,L2[i]+1,L1[i],p[i]);
            add_event(R1[i],L2[i]+1,L1[i],-p[i]);
            add_event(R1[i],L1[i]+1,i,p[i]);
            add_event(R2[i],L1[i]+1,i,-p[i]);
        }
        for(int i=0,l,r;i<q;++i)
        {
            cin>>l>>r;
            qry[r].emplace_back(l,i);
        }
        for(int i=1;i<=n;++i)
        {
            for(auto& [l,r,val]:gra[i])tr.update_add(1,1,n,l,r,val);
            tr.update_his();
            for(auto& [l,idx]:qry[i])ans[idx]=tr.query(1,1,n,l,n);
        }
        for(int i=0;i<q;++i)cout<<ans[i]<<'\n';
    }
}   
```

</details>