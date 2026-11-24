## 题意

给定一棵树，每条边有边权，可以用 $c$ 的代价把一条边的边权增加/减少 $c$，边权始终非负，求使根节点与所有叶子节点的距离相等的最小花费。

## 题解

### $\mathcal O(nV^2)$ 解法

一眼 dp，设 `dp[u][x]` 为让 $u$ 的子树中让所有叶子节点至 $u$ 的距离均为 $x$ 的最小代价。则显然有 dp 转移

$$
dp[u][x]=\sum_{v\in\text{gra}[u]}\min_{y\leq x}(dp[v][y]+|x-y-w|)
$$

初始值为

$$
dp[u][x]=
\begin{cases}
\infty & \text{if}\ u\ \text{is a leaf node and }x\not=0\\
0 & \text{otherwise.}
\end{cases}
$$

期望得分 $19$ 分。


<details>
<summary>代码</summary>


```cpp
#include<bits/stdc++.h>
using namespace std;
#define int long long
constexpr int INF=0x3f3f3f3f3f3f3f3f;
signed main()
{
    #ifdef _WIN32
    freopen("qbu.in","r",stdin);
    #endif
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m;
    cin>>n>>m;
    vector<vector<pair<int,int>>>gra(n+m+1);
    for(int i=2,p,c;i<=n+m;++i)
    {
        cin>>p>>c;
        gra[i].emplace_back(p,c);
        gra[p].emplace_back(i,c);
    }
    n+=m;
    vector<array<int,600>>dp(n+1);
    auto dfs=[&](auto&& self,int u,int f)
    {
        if(gra[u].size()==1&&f)
        {
            dp[u].fill(INF);
            dp[u][0]=0;
            return;
        }
        dp[u].fill(0);
        for(auto [v,w]:gra[u])
        {
            if(v==f)continue;
            self(self,v,u);
            for(int i=0;i<600;++i)
            {
                int add=INF;
                for(int j=0;j<=i;++j)
                    add=min(add,dp[v][j]+abs(i-j-w));
                if(dp[u][i]<=INF)dp[u][i]+=add;
                else dp[u][i]=INF;
            }
        }
    };
    dfs(dfs,1,0);
    cout<<*ranges::min_element(dp[1]);
}
```

</details>

### $\mathcal O(n\log n)$ 解法

观察这个 dp 的形式，它形如 $\displaystyle\sum_{a\in p}|x-a|$，图像大概长这个样：

![图挂了](https://cdn.luogu.com.cn/upload/image_hosting/cwz5iyke.webp)

可以发现，虽然它的值域很大，但斜率变化的点只有 $\mathcal O(n)$ 个，较少，且斜率单调递增，所以我们只需要记录斜率变化点的坐标（相邻两个点之间斜率差 $1$）与最右端的直线的斜率，就可以还原出整个函数。

考虑这个 dp 在做什么。

 - $\min$：它取得是整个函数在这个点左端的最小值，它会把谷底右侧的分界点全部删掉，即把这个集合中最大的 $k-1$ 个点删掉（$k$ 是这个点的子节点数）

例：
![图挂了](https://cdn.luogu.com.cn/upload/image_hosting/cwz5iyke.webp)
会变为
![图挂了](https://cdn.luogu.com.cn/upload/image_hosting/a91ng5az.webp)

 - $-$： 把整个函数往右移 $w$，$w$ 是它与它的父节点的距离；
 - $\sum$：把当前节点合并到父结点上。

最后把根节点的函数的谷底拿出来即可。考虑答案较初始值 $\displaystyle\sum w$ 下降了多少。设使斜率由 $-1\to 0$ 的分界点为 $v_m$，$0$ 与 $v_m$ 中的分界点分别为 $v_{1\cdots m-1}$。易得 $0$ 处的斜率为 $-m$，则由变化量为 

$$
\begin{aligned}
\Delta _1&=-m(v_1-0)=-mv_1\\
\Delta _2&=-(m-1)(v_2-v_1)=-(m-1)(v_2-v_1)\\
\Delta _3&=-(m-2)(v_3-v_2)=-(m-2)(v_3-v_2)\\
\cdots&=\cdots\\
\Delta _m&=-1(v_m-v_{m-1})=-v_m-v_{m-1}\\
\end{aligned}
$$

对两边求和，可以发现 $\displaystyle\sum\Delta$ 就等于 $-\displaystyle\sum v_i$，所以只需在答案中减去这些点即可。

<details>
<summary>比暴力短的正解</summary>

```cpp
#include<bits/stdc++.h>
#include<ext/pb_ds/priority_queue.hpp>
using namespace std;
#define int long long
using heap=__gnu_pbds::priority_queue<int>;
constexpr int INF=0x3f3f3f3f3f3f3f3f;
signed main()
{
    #ifdef _WIN32
    freopen("qbu.in","r",stdin);
    #endif
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m;
    cin>>n>>m;
    vector<int>fa(n+m+1);
    vector<int>len(n+m+1); 
    vector<int>cnt(n+m+1);
    vector<heap>h(n+m+1);
    int ans=0;
    for(int i=2;i<=n+m;++i)cin>>fa[i]>>len[i],ans+=len[i],cnt[fa[i]]++;
    for(int i=n+m;i>=2;--i)
    {
        int L=0,R=0;
        if(i<=n)
        {
            while(--cnt[i])h[i].pop();
            R=h[i].top();h[i].pop();
            L=h[i].top();h[i].pop();
        }
        h[i].push(L+len[i]);
        h[i].push(R+len[i]);
        h[fa[i]].join(h[i]);
    }
    while(cnt[1]--)h[1].pop();
    while(!h[1].empty())ans-=h[1].top(),h[1].pop();
    cout<<ans;
}
```

</details>