
# 题意

多测，给定长度为 $n$ 的排列 $a,b$，$q$ 次询问，每次给定 $[l,r]$，求 

$$
\sum_{l \le x \le y \le r}\left(\max_{i=x}^y a_i\right)\left(\max_{i=x}^y b_i\right)
$$

数据范围：$n,q\leq 2.5\times 10^5$。 

# 题解

## 扫描线转化

考虑扫描右端点 $y$（将其视为时间轴）。对于固定的 $y$，设 $L_a$ 是 $y$ 左边第一个大于 $a_y$ 的位置，则对于 $x \in (L_a, y]$，区间 $[x, y]$ 的 $\max a$ 就是 $a_y$。同理可定义 $b$ 的 $L_b$。

当 $y$ 向右移动时，单调栈的弹栈操作等价于对 $x$ 的某个前缀区间进行**区间覆盖**（将 $\max a$ 覆盖为新的 $a_y$，$\max b$ 覆盖为新的 $b_y$）。

建立平面直角坐标系，点 $(x,y)$ 代表子区间 $[x,y]$。则区间 $[l,r]$ 的答案等价于求右下角区域 $\{(x,y) \mid x \ge l, y \le r\}$ 内所有点的权值乘积之和：

$$
\text{Ans}(l,r)=\sum_{x\geq l}\sum_{y=1}^r (\max a) \cdot (\max b)
$$

把 $y$ 看做时间轴，单调栈的修改转化为**区间覆盖**，查询答案转化为查询**历史和**。

## 线段树维护

考虑线段树需要维护什么。为了回答历史和查询，我们肯定要维护历史和 $\text{his}$。
为了在时间推进（tick）时更新 $\text{his} \gets \text{his} + \text{AB}$，我们需要维护当前时刻的乘积和 $\text{AB}=\sum (\max a)(\max b)$。
而为了在区间覆盖时更新 $\text{AB}$，我们还需要分别维护 $\text{A}=\sum \max a$ 和 $\text{B}=\sum \max b$。

因此，每个节点维护一个状态向量（加入常数 $1$ 以处理长度 $len$ 带来的常数项）：

$$
\text{vec}=
\begin{bmatrix}
\text{his}\\
\text{AB}\\
\text{A}\\
\text{B}\\
1
\end{bmatrix}
$$

设当前节点的长度为 $len$，我们将所有操作对向量的影响用矩阵进行刻画。

### 1. 区间覆盖 $a$ 为 $v$
这会使得 $\text{A} \gets v \cdot len$，同时 $\text{AB} \gets v \cdot \text{B}$。转移矩阵为：
$$
M_{\text{cov}A}(v) =
\begin{bmatrix}
1 & 0 & 0 & 0 & 0 \\
0 & 1 & 0 & v & 0 \\
0 & 0 & 0 & 0 & v \cdot len \\
0 & 0 & 0 & 1 & 0 \\
0 & 0 & 0 & 0 & 1 \\
\end{bmatrix}
$$

### 2. 区间覆盖 $b$ 为 $v$
这会使得 $\text{B} \gets v \cdot len$，同时 $\text{AB} \gets v \cdot \text{A}$。转移矩阵为：
$$
M_{\text{cov}B}(v) =
\begin{bmatrix}
1 & 0 & 0 & 0 & 0 \\
0 & 1 & v & 0 & 0 \\
0 & 0 & 1 & 0 & 0 \\
0 & 0 & 0 & 0 & v \cdot len \\
0 & 0 & 0 & 0 & 1 \\
\end{bmatrix}
$$

### 3. 更新历史和
时间推进一格，$\text{his} \gets \text{his} + \text{AB}$。转移矩阵为：
$$
M_{\text{his}} =
\begin{bmatrix}
1 & 1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 & 0 \\
0 & 0 & 1 & 0 & 0 \\
0 & 0 & 0 & 1 & 0 \\
0 & 0 & 0 & 0 & 1 \\
\end{bmatrix}
$$


## 矩阵的封闭性与复合公式

观察上述三种矩阵，它们都是**主对角线全为 $1$ 的上三角矩阵**。
为了保证线段树懒标记的正确性，这类矩阵在乘法下必须是**封闭**的。

完整的封闭矩阵必须包含 **9 个自由参数**：
$$
M =
\begin{bmatrix}
1 & a & g & h & k \\
0 & 1 & b & c & f \\
0 & 0 & 1 & 0 & d \\
0 & 0 & 0 & 1 & e \\
0 & 0 & 0 & 0 & 1 \\
\end{bmatrix}
$$

### 标记复合 ( $M_{\text{new}} \times M_{\text{old}}$ )
设旧标记为下标 $1$，新标记为下标 $2$，复合后的矩阵 $M_{\text{res}} = M_2 \times M_1$。通过矩阵乘法展开，可得极其规整的复合公式：

$$
\begin{aligned}
a_{\text{res}} &= a_1 + a_2 \\
b_{\text{res}} &= b_1 + b_2 \\
c_{\text{res}} &= c_1 + c_2 \\
d_{\text{res}} &= d_1 + d_2 \\
e_{\text{res}} &= e_1 + e_2 \\
f_{\text{res}} &= f_1 + f_2 + b_2 d_1 + c_2 e_1  \\
g_{\text{res}} &= g_1 + g_2 + a_2 b_1  \\
h_{\text{res}} &= h_1 + h_2 + a_2 c_1  \\
k_{\text{res}} &= k_1 + k_2 + a_2 f_1 + g_2 d_1 + h_2 e_1
\end{aligned}
$$

### 标记作用于节点
设节点当前状态为 $(\text{his}, \text{AB}, \text{A}, \text{B})$，长度为 $len$，应用标记 $M$ 后：
$$
\begin{aligned}
\text{his}' &= \text{his} + a \cdot \text{AB} + g \cdot \text{A} + h \cdot \text{B} + k \cdot len \\
\text{AB}'  &= \text{AB} + b \cdot \text{A} + c \cdot \text{B} + f \cdot len \\
\text{A}'   &= \text{A} + d \\
\text{B}'   &= \text{B} + e
\end{aligned}
$$

```cpp
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
using ull=unsigned long long;
struct SegTree
{
    struct Tag
    {
        ull a_1,a_x,a_y,a_xy;
        ull b_1,b_x,b_y;
        ull x_1,y_1;
        Tag(ull a_1=0,ull a_x=0,ull a_y=0,ull a_xy=0,ull b_1=0,ull b_x=0,ull b_y=0,ull x_1=0,ull y_1=0):a_1(a_1),a_x(a_x),a_y(a_y),a_xy(a_xy),b_1(b_1),b_x(b_x),b_y(b_y),x_1(x_1),y_1(y_1) {}
        bool empty()const
        {
            return !(a_1|a_x|a_y|a_xy|b_1|b_x|b_y|x_1|y_1);
        }
        Tag operator+(const Tag&t)const
        {
            Tag r;
            r.x_1=x_1+t.x_1;
            r.y_1=y_1+t.y_1;
            r.b_x=b_x+t.b_x;
            r.b_y=b_y+t.b_y;
            r.b_1=b_1+t.b_1+t.b_x*x_1+t.b_y*y_1;
            r.a_xy=a_xy+t.a_xy;
            r.a_x=a_x+t.a_x+t.a_xy*b_x;
            r.a_y=a_y+t.a_y+t.a_xy*b_y;
            r.a_1=a_1+t.a_1+t.a_xy*b_1+t.a_x*x_1+t.a_y*y_1;
            return r;
        }
    };
    struct node_t
    {
        ull sum_x,sum_y,sum_xy,hist;
        int len;
        Tag tag;
        node_t(ull sum_x=0,ull sum_y=0,ull sum_xy=0,ull hist=0,int len=0,Tag tag= {}):sum_x(sum_x),sum_y(sum_y),sum_xy(sum_xy),hist(hist),len(len),tag(tag) {}
        node_t operator+(const node_t&b)const
        {
            return {sum_x+b.sum_x,sum_y+b.sum_y,sum_xy+b.sum_xy,hist+b.hist,len+b.len};
        }
        node_t operator+(const Tag&t)const
        {
            if(t.empty())return *this;
            node_t r=*this;
            ull L=1ull*len;
            r.hist+=t.a_xy*sum_xy+t.a_x*sum_x+t.a_y*sum_y+t.a_1*L;
            r.sum_xy+=t.b_x*sum_x+t.b_y*sum_y+t.b_1*L;
            r.sum_x+=t.x_1*L;
            r.sum_y+=t.y_1*L;
            r.tag=tag+t;
            return r;
        }
        node_t& operator+=(const Tag&t)
        {
            *this=*this+t;
            return *this;
        }
    };
    #define ls (p<<1)
    #define rs (p<<1|1)
    #define mid ((l+r)>>1)
    vector<node_t>node;
    void assign(int n)
    {
        node.assign(n<<2, {});
    }
    void pushup(int p)
    {
        node[p]=node[ls]+node[rs];
    }
    void pushdown(int p)
    {
        if(node[p].tag.empty())return;
        node[ls]+=node[p].tag;
        node[rs]+=node[p].tag;
        node[p].tag= {};
    }
    void build(int p,int l,int r)
    {
        if(l==r)
        {
            node[p]=node_t(0,0,0,0,1, {});
            return;
        }
        build(ls,l,mid);
        build(rs,mid+1,r);
        pushup(p);
    }
    void update(int p,int l,int r,int s,int t,const Tag&k)
    {
        if(s<=l&&r<=t)
        {
            node[p]+=k;
            return;
        }
        pushdown(p);
        if(s<=mid)update(ls,l,mid,s,t,k);
        if(t>mid)update(rs,mid+1,r,s,t,k);
        pushup(p);
    }
    ull query_hist(int p,int l,int r,int s,int t)
    {
        if(s<=l&&r<=t)return node[p].hist;
        pushdown(p);
        ull ans=0;
        if(s<=mid)ans+=query_hist(ls,l,mid,s,t);
        if(t>mid)ans+=query_hist(rs,mid+1,r,s,t);
        return ans;
    }
    static Tag add_x(ull d)
    {
        Tag t;
        t.x_1=d;
        t.b_y=d;
        return t;
    }
    static Tag add_y(ull d)
    {
        Tag t;
        t.y_1=d;
        t.b_x=d;
        return t;
    }
    static Tag tick()
    {
        Tag t;
        t.a_xy=1;
        return t;
    }
    #undef ls
    #undef rs
    #undef mid
};
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T,n;
    cin>>T>>n;
    vector<int>a(n+1),b(n+1);
    for(int i=1;i<=n;++i)cin>>a[i];
    for(int i=1;i<=n;++i)cin>>b[i];
    int q;
    cin>>q;
    vector<vector<pair<int,int>>>qry(n+1);
    for(int i=1,l,r;i<=q;++i)
    {
        cin>>l>>r;
        qry[r].emplace_back(l,i-1);
    }
    SegTree tr;
    tr.assign(n);
    tr.build(1,1,n);
    vector<int>stk1,stk2;
    stk1.reserve(n);
    stk2.reserve(n);
    vector<ull>ans(q);
    for(int r=1;r<=n;++r)
    {
        while(!stk1.empty()&&a[stk1.back()]<a[r])
        {
            int j=stk1.back();
            stk1.pop_back();
            int L=stk1.empty()?1:stk1.back()+1;
            tr.update(1,1,n,L,j,SegTree::add_x(1ull*(a[r]-a[j])));
        }
        tr.update(1,1,n,r,r,SegTree::add_x(1ull*a[r]));
        stk1.emplace_back(r);
        while(!stk2.empty()&&b[stk2.back()]<b[r])
        {
            int j=stk2.back();
            stk2.pop_back();
            int L=stk2.empty()?1:stk2.back()+1;
            tr.update(1,1,n,L,j,SegTree::add_y(1ull*(b[r]-b[j])));
        }
        tr.update(1,1,n,r,r,SegTree::add_y(1ull*b[r]));
        stk2.emplace_back(r);
        tr.update(1,1,n,1,n,SegTree::tick());
        for(auto&[l,id]:qry[r])ans[id]=tr.query_hist(1,1,n,l,r);
    }
    for(int i=0;i<q;++i)cout<<ans[i]<<'\n';
}
```