#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int maxn=1e2+10;
constexpr int maxm=7e3+10;
constexpr int maxk=60;
constexpr int mod=998244353;
constexpr int INF=1e18;
constexpr int inv2=(mod+1)>>1;
constexpr int inv6=166374059;
constexpr double eps=1e-8;
int n,m,T,k,ans,res,cnt[maxk],q,dp[16][9][5][3][2][maxm];
int sum(int p)
{
    int q=p/2,r=p&1;
    q%=mod;
    int s1=q*((q+1)%mod)%mod*inv2%mod;
    int s2=q*((q+1)%mod)%mod*((2*q%mod+1)%mod)%mod*inv6%mod;
    int ans=((n+1)%mod*s1%mod-s2+mod)%mod*2%mod;
    if(r)
    {
        ans=(ans+(q+1)%mod*((n%mod-q+mod)%mod)%mod)%mod;
    }
    return ans;
}
int dfs(int _1,int _2,int _3,int _4,int _5,int s)
{
    if(_1<0||_2<0||_3<0||_4<0||_5<0||s<0)
    {
        return 0;
    }
    if(!s)
    {
        return !(_1||_2||_3||_4||_5);
    }
    int &ans=dp[_1][_2][_3][_4][_5][s];
    if(~ans)
    {
        return ans;
    }
    ans=0;
    int p=_1+_2+_3+_4+_5;
    p*=(n-p+1);
    ans+=dfs(_1-1,_2,_3,_4,_5,s-p);
    ans+=dfs(_1,_2-1,_3,_4,_5,s-2*p);
    ans+=dfs(_1,_2,_3-1,_4,_5,s-3*p);
    ans+=dfs(_1,_2,_3,_4-1,_5,s-4*p);
    ans+=dfs(_1,_2,_3,_4,_5-1,s-5*p);
    return ans;
}
int fac[maxn];
signed main()
{
    // freopen("npc.in","r",stdin);
    // freopen("npc.out","w",stdout);
    scanf("%lld",&q);
    fac[0]=1;
    for(int i=1;i<16;++i)
    {
        fac[i]=fac[i-1]*i;
    }
    while(q--)
    {
        scanf("%lld%lld",&n,&k);
        int lg=__lg(n)+1;
        memset(cnt,0,sizeof(cnt));
        for(int i=1;i<=lg;++i)
        {
            cnt[i]=n/(1ll<<i-1)-n/(1ll<<i);
        }
        ans=0;
        for(int i=lg,l=1,r;i>=1;--i,l=r+1)
        {
            r=l+cnt[i]-1;
            ans=(ans+(sum(r)-sum(l-1)+mod)%mod*i%mod)%mod;
        }
        if(n>29)
        {
            printf("%lld\n",ans);
            continue;
        }
        for(int i=1;i<=5;++i)
        {
            k=(k+fac[cnt[i]]-1)/fac[cnt[i]];
        }
        memset(dp,-1,sizeof(dp));
        for(int i=ans;;++i)
        {
            int rnk=dfs(cnt[1],cnt[2],cnt[3],cnt[4],cnt[5],i);
            if(rnk>=k)
            {
                printf("%lld\n",i%mod);
                break;
            }
            k-=rnk;
        }
    }
    return 0;
}
