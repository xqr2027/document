#include<bits/stdc++.h>
using namespace std;
#define int long long
#define Accepted {now=nxt,nowdis=nxtdis;}
constexpr int lim=1001;
constexpr int ordT=1e5;
constexpr double maxt=0.90;
constexpr double delta=0.999;
constexpr double eps=1e-8;
vector<double>a[lim];
double res[lim];
int maxn;
double Rand(){return (double)rand()/RAND_MAX;}
double calc(vector<double>& b)
{
    double sum=0;
    for(int i=0;i<=maxn;++i)
    {
        double cur=0;
        for(int j=0;j<maxn;++j)cur+=100*(a[i][j]-b[j])*(a[i][j]-b[j]);
        res[i]=cur;sum+=cur;
    }
    double ans=0;
    for(int i=0;i<=maxn;++i)ans+=fabs(res[i]*maxn-sum);
    return ans;
}
vector<double>best;
double bestdis;
void simulateAnneal(vector<double> b)
{
    double T=ordT;
    auto now=b;
    double nowdis=calc(now);
    while(T>eps)
    {
        auto nxt=now;
        nxt[rand()%maxn]+=T*(Rand()*2-1);
        double nxtdis=calc(nxt);
        double dec=nxtdis-nowdis;
        if(dec<0)
        {
            Accepted;
            if(bestdis>nowdis)best=now,bestdis=nowdis;
        }
        else if(exp(-dec/T)>Rand())Accepted;
        T*=delta;
    }
}
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout<<fixed<<setprecision(3);
    // srand(time(0));
    // cin>>n;
    // best.assign(n,0);
    // for(int i=0;i<=n;++i)
    // {
    //     a[i].assign(n,0);
    //     for(int j=0;j<n;++j)
    //     {
    //         cin>>a[i][j];
    //         best[j]+=a[i][j];
    //     }
    // }
    // for(int i=0;i<n;++i)best[i]/=(n+1);
    // bestdis=calc(best);
    // do simulateAnneal(best);
    // while((double)clock()/CLOCKS_PER_SEC<maxt);
    // for(auto i:best)cout<<i<<' ';
    cout<<'\a';
}