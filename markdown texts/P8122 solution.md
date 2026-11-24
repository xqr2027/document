~~大便交互题~~
# 题意
给定一个长度为 $N$ 的数列 $x$，你要在 $S$ 次询问内选出 $K$ 个数，使这些数的和 $\in[A,2A]$。

数据范围：$S\leq 40$，$3\leq N\leq 10^5$，$1\leq A,x_i\leq 10^{17}$，$3\leq K\leq 10$。

# 题解

因为 $S$ 的范围，所以觉得大概是一个 $\mathcal O(\log N)$ 的算法。

分类讨论，按是否选了大于 $A$ 的数分类。

如果你选了大于 $A$ 的数，那么你的和一定是大于 $A$ 的，只需要考虑上界即可。不难发现此时让和小一点一定不劣，所以只需要 `check` 最小的 $K-1$ 个数加上最小的大于 $A$ 的数，查询它们的和是否小于等于 $2A$ 即可。这显然可以二分优化，复杂度 $\mathcal O(K+\log N)\leq28$。

第二种情况，你没有选大于 $A$ 的数。一个观察是，你把已选的数中的一个换成最大的未选的 $\leq A$ 的数，总和的增加量一定小于 $A$，于是可以最开始选最小的 $K$ 个，再从小到大把初始的 $K$ 个数换成最大的未选的 $\leq A$ 的数，总和第一次 $\geq A$ 时的数列就是答案。不难发现只有前 $K$ 次操作总和会增加，$K$ 次操作后选出的数已经为最大的 $\leq A$ 的 $K$ 个数了，再操作只会使总和变小，因此只需操作 $K$ 次即可，情况二中最多询问 $K\leq 10$ 次。

总共询问次数 $\leq 38$，可以通过。

# 完整代码

~~都写这么 shit 了还抄？~~

```cpp
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
ll skim(int i);
void answer(vector<int> v);
void impossible();
void solve(int N, int K, ll A, int S)
{
    int n=N;
    ll a=A;
    vector<ll>memory(N+1,-1);
    auto ask=[&](int i)
    {
        if(memory[i]!=-1)return memory[i];
        return memory[i]=skim(i);
    };
    deque<int>q;
    ll sum=0;
    for(int i=1;i<=K;++i)
    {
        sum+=ask(i);
        q.emplace_back(i);
    }
    if(sum>a<<1)
    {
        impossible();
        return;
    }
    if(sum>=a)
    {
        vector<int>out;
        for(int i:q)out.emplace_back(i);
        answer(out);
        return;
    }
    auto get=[&]()
    {
        int l=K+1,r=n;
        int ans=K;
        while(l<=r)
        {
            int mid=(l+r)>>1;
            if(ask(mid)<=A)ans=mid,l=mid+1;
            else r=mid-1;
        }
        return ans;
    };
    int rptr=get();
    int lptr=K;
    if(rptr!=n)
    {
        ll nsum=sum-ask(lptr)+ask(rptr+1);
        if(a<=nsum&&nsum<=a<<1)
        {
            q.back()=rptr+1;
            vector<int>out;
            for(int i:q)out.emplace_back(i);
            answer(out);
            return;
        }
    }
    int cnt=0;
    while(sum<a&&cnt++<K&&K<rptr)
    {
        auto idx=q.front();
        q.pop_front();
        sum-=ask(idx);
        sum+=ask(rptr);
        q.emplace_back(rptr);
        if(sum>=a)
        {
            vector<int>out;
            for(int i:q)out.emplace_back(i);
            answer(out);
            return;
        }
        --rptr;
    }
    impossible();
}
```