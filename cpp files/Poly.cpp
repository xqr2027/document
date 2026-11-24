#include<bits/stdc++.h>
using namespace std;
constexpr int mod=998244353;
constexpr int G=3;
constexpr int Gi=332748118;
constexpr int inv2=(mod+1)/2;
static int qpow(int a,int b)
{
    int res=1;
    while(b)
    {
        if(b&1)res=1ll*res*a%mod;
        a=1ll*a*a%mod;
        b>>=1;
    }
    return res;
}
namespace Cipolla
{
    long long w;
    struct Extension
    {
        long long u,v;
        Extension(long long u=0,long long v=0):u(u),v(v){}
        Extension operator*(const Extension& b)const
        {
            Extension res;
            res.u=(u*b.u%mod+v*b.v%mod*w%mod)%mod;
            res.v=(b.u*v%mod+b.v*u%mod)%mod;
            return res;
        }
        Extension operator^(long long a)const
        {
            Extension res(1,0);
            Extension base=*this;
            while(a)
            {
                if(a&1)res=res*base;
                base=base*base;
                a>>=1;
            }
            return res;
        }
    };
    static int solve(int n)
    {
        if(n==0)return 0;
        if(qpow(n,(mod-1)/2)!=1)return-1;
        mt19937 rng(random_device{}());
        long long r,base;
        while(true)
        {
            r=rng()%mod;
            base=(r*r-n+mod)%mod;
            if(qpow(base,(mod-1)/2)==mod-1)
                break;
        }
        w=base;
        Extension res=Extension(r,1)^ ((mod+1)/2);
        int ans=res.u;
        return min(ans,mod-ans);
    }
}
static int sqrt_mod(int a){return Cipolla::solve(a);}
class Poly
{
public:
    vector<int>val;
    Poly()=default;
    explicit Poly(int siz,int v=0):val(siz,v){}
    Poly(const vector<int>& v):val(v){trim();}
    size_t size()const{return val.size();}
    int operator()(const int x)const
    {
        int ans=0;
        for(int i=(int)val.size()-1;i>=0;--i)ans=(1ll*ans*x+val[i])%mod;
        return ans;
    }
    void resize(int n){val.resize(n,0);}
    void trim()
    {
        while(!val.empty()&& val.back()==0)val.pop_back();
    }
    Poly trunc(int n)const
    {
        Poly res;
        for(int i=0;i<min(n,(int)val.size());++i)res.val.emplace_back(val[i]);
        return res;
    }
    int  operator[](int idx)const
    {
        if(idx<0||idx>=(int)val.size())return 0;
        return val[idx];
    }
    int& operator[](int idx)
    {
        if(idx>=(int)val.size())val.resize(idx+1,0);
        return val[idx];
    }
    static void ntt(vector<int>& f,bool inv)
    {
        int n=f.size();
        for(int i=0,j=0;i<n;++i)
        {
            if(i<j)swap(f[i],f[j]);
            int k=n>>1;
            for(;j&k;k>>=1)j^=k;
            j^=k;
        }
        for(int mid=1;mid<n;mid<<=1)
        {
            int wn=qpow(inv?Gi:G,(mod-1)/(mid<<1));
            for(int i=0;i<n;i+=mid<<1)
            {
                int w=1;
                for(int j=0;j<mid;++j,w=1ll*w*wn%mod)
                {
                    int x=f[i+j];
                    int y=1ll*f[i+j+mid]*w%mod;
                    f[i+j]=(x+y)%mod;
                    f[i+j+mid]=(x-y+mod)%mod;
                }
            }
        }
        if(inv)
        {
            int invn=qpow(n,mod-2);
            for(int& v:f)v=1ll*v*invn%mod;
        }
    }
    static void fwt_or (vector<int>& a,bool inv)
    {
        int n=a.size();
        for(int len=2;len<=n;len<<=1)
        {
            int h=len>>1;
            for(int i=0;i<n;i+=len)
            {
                for(int j=0;j<h;j++)
                {
                    int x=a[i+j],y=a[i+j+h];
                    if(!inv)a[i+j+h]=(x+y)%mod;
                    else a[i+j+h]=(y-x+mod)%mod;
                }
            }
        }
    }
    static void fwt_and(vector<int>& a,bool inv)
    {
        int n=a.size();
        for(int len=2;len<=n;len<<=1)
        {
            int h=len>>1;
            for(int i=0;i<n;i+=len)
            {
                for(int j=0;j<h;j++)
                {
                    int x=a[i+j],y=a[i+j+h];
                    if(!inv)a[i+j]=(x+y)%mod;
                    else a[i+j]=(x-y+mod)%mod;
                }
            }
        }
    }
    static void fwt_xor(vector<int>& a,bool inv)
    {
        int n=a.size();
        for(int len=2;len<=n;len<<=1)
        {
            int h=len>>1;
            for(int i=0;i<n;i+=len)
            {
                for(int j=0;j<h;j++)
                {
                    int x=a[i+j],y=a[i+j+h];
                    a[i+j]=(x+y)%mod;
                    a[i+j+h]=(x-y+mod)%mod;
                }
            }
        }
        if(inv)
        {
            int invn=qpow(n,mod-2);
            for(auto& v:a)v=1ll*v*invn%mod;
        }
    }
    Poly rev()const
    {
        Poly res=*this;
        reverse(res.val.begin(),res.val.end());
        return res;
    }
    Poly deriv()const
    {
        int n=(int)size();
        if(n<=1)return Poly(1);
        Poly res(n-1);
        for(int i=1;i<n;++i)res[i-1]=1ll*val[i]*i%mod;
        res.trim();
        return res;
    }
    Poly integ()const
    {
        int n=(int)size();
        Poly res(n+1);
        vector<int>inv(n+2);
        inv[1]=1;
        for(int i=2;i<=n+1;++i)inv[i]=mod-1ll*(mod/i)*inv[mod%i]%mod;
        for(int i=0;i<n;++i)res[i+1]=1ll*val[i]*inv[i+1]%mod;
        res[0]=0;
        res.trim();
        return res;
    }
    Poly inv (int n)const
    {
        Poly b(1);
        b[0]=qpow((*this)[0],mod-2);
        int len=1;
        while(len<n)
        {
            len<<=1;
            Poly cur;
            cur.resize(len);
            for(int i=0;i<len;++i)cur[i]=(*this)[i];
            Poly tmp=b*b*cur;
            b.resize(len);
            for(int i=0;i<len;++i)b[i]=(2LL*b[i]-tmp[i]+mod)%mod;
        }
        b.resize(n);
        b.trim();
        return b;
    }
    Poly sqrt(int n)const
    {
        Poly b(1);
        b[0]=sqrt_mod((*this)[0]);
        int len=1;
        while(len<n)
        {
            len<<=1;
            Poly cur;
            cur.resize(len);
            for(int i=0;i<len;++i)cur[i]=(*this)[i];
            Poly invB=b.inv(len);
            Poly tmp=cur*invB;
            b.resize(len);
            for(int i=0;i<len;++i)b[i]=1ll*(b[i]+tmp[i])* inv2%mod;
        }
        b.resize(n);
        b.trim();
        return b;
    }
    Poly ln  (int n)const
    {
        Poly dA=deriv();
        Poly iA=inv(n);
        Poly res=dA*iA;
        res.resize(n-1);
        res=res.integ();
        res.resize(n);
        res.trim();
        return res;
    }
    Poly exp (int n)const
    {
        Poly b(1);
        b[0]=1;
        int len=1;
        while(len<n)
        {
            len<<=1;
            Poly lnB=b.ln(len);
            for(int i=0;i<len;++i)lnB[i]=((*this)[i]-lnB[i]+mod)%mod;
            lnB[0]=(lnB[0]+1)%mod;
            b=b*lnB;
            b.resize(len);
        }
        b.resize(n);
        b.trim();
        return b;
    }
    Poly pow(long long k,int n)const
    {
        Poly a=*this;
        a.trim();
        int pos=0;
        int m=(int)a.size();
        while(pos<m&&a[pos]==0)pos++;
        if(pos==m||1ll*pos*k>=n)
        {
            Poly zero(n);
            return zero;
        }
        int c=a[pos];
        int invc=qpow(c,mod-2);
        Poly b;
        for(int i=pos;i<m;++i)b[i-pos]=1ll*a[i]*invc%mod;
        b.resize(n);
        Poly lnB=b.ln(n);
        for(auto& v:lnB.val)v=1ll*v*(k%mod)%mod;
        Poly e=lnB.exp(n);
        Poly res(n);
        long long shift=k*pos;
        int ck=qpow(c,(int)(k%(mod-1)));
        for(int i=0;i<n;++i)
        {
            long long tar=i+shift;
            if(tar>=n)continue;
            res[(int)tar]=1ll*e[i]*ck%mod;
        }
        res.trim();
        return res;
    }
    friend Poly operator+(const Poly& a,const Poly& b)
    {
        int n=max((int)a.size(),(int)b.size());
        Poly res(n);
        for(int i=0;i<n;++i)res[i]=(a[i]+b[i])%mod;
        res.trim();
        return res;
    }
    friend Poly operator-(const Poly& a,const Poly& b)
    {
        int n=max((int)a.size(),(int)b.size());
        Poly res(n);
        for(int i=0;i<n;++i)res[i]=(a[i]-b[i]+mod)%mod;
        res.trim();
        return res;
    }
    friend Poly operator*(const Poly& a,const Poly& b)
    {
        auto fa=a.val,fb=b.val;
        int n=1;
        while(n<(int)(a.size()+b.size()-1))n<<=1;
        fa.resize(n);
        fb.resize(n);
        Poly::ntt(fa,false);
        Poly::ntt(fb,false);
        for(int i=0;i<n;++i)fa[i]=1ll*fa[i]*fb[i]%mod;
        Poly::ntt(fa,true);
        Poly res(fa);
        res.trim();
        return res;
    }
    friend Poly operator/(const Poly& a,const Poly& b)
    {
        if(a.size()<b.size())return Poly(1);
        Poly f=a.rev(),g=b.rev();
        int n=(int)a.size(),m=(int)b.size();
        int k=n-m+1;
        g.resize(k);
        Poly invg=g.inv(k);
        Poly Qrev=f*invg;
        Qrev.resize(k);
        Poly Q=Qrev.rev();
        Q.trim();
        return Q;
    }
    friend Poly operator%(const Poly& a,const Poly& b)
    {
        Poly Q=a/b;
        Poly R=a-Q*b;
        R.resize((int)b.size()-1);
        R.trim();
        return R;
    }
    friend Poly operator|(const Poly& a,const Poly& b)
    {
        vector<int>fa=a.val,fb=b.val;
        int n=1;
        while(n<(int)(a.size()+b.size()-1))n<<=1;
        fa.resize(n),fb.resize(n);
        Poly::fwt_or(fa,false);
        Poly::fwt_or(fb,false);
        for(int i=0;i<n;++i)fa[i]=1ll*fa[i]*fb[i]%mod;
        Poly::fwt_or(fa,true);
        Poly res(fa);
        res.trim();
        return res;
    }
    friend Poly operator&(const Poly& a,const Poly& b)
    {
        vector<int>fa=a.val,fb=b.val;
        int n=1;
        while(n<(int)(a.size()+b.size()-1))n<<=1;
        fa.resize(n),fb.resize(n);
        Poly::fwt_and(fa,false);
        Poly::fwt_and(fb,false);
        for(int i=0;i<n;++i)fa[i]=1ll*fa[i]*fb[i]%mod;
        Poly::fwt_and(fa,true);
        Poly res(fa);
        res.trim();
        return res;
    }
    friend Poly operator^(const Poly& a,const Poly& b)
    {
        vector<int>fa=a.val,fb=b.val;
        int n=1;
        while(n<(int)(a.size()+b.size()-1))n<<=1;
        fa.resize(n),fb.resize(n);
        Poly::fwt_xor(fa,false);
        Poly::fwt_xor(fb,false);
        for(int i=0;i<n;++i)fa[i]=1ll*fa[i]*fb[i]%mod;
        Poly::fwt_xor(fa,true);
        Poly res(fa);
        res.trim();
        return res;
    }
    friend Poly operator<<(const Poly& a,size_t k)
    {
        Poly res;
        res.resize(k);
        for(auto x:a.val)res.val.emplace_back(x);
        return res;
    }
    friend Poly operator>>(const Poly& a,size_t k)
    {
        Poly res;
        for(int i=k;i<(int)a.size();++i)res.val.emplace_back(a[i]);
        res.trim();
        return res;
    }
    friend istream& operator>>(istream& in,Poly& p)
    {
        int len=p.val.size();
        for(int i=0;i<len;++i)
        {
            in>>p.val[i];
            p.val[i]%=mod;
            if(p.val[i]<0)p.val[i]+=mod;
        }
        p.trim();
        return in;
    }
    friend ostream& operator<<(ostream& out,const Poly& p)
    {
        for(int v:p.val)out<<v<<' ';
        return out;
    }
    auto operator +=(const Poly& b){*this=*this+b;return *this;}
    auto operator -=(const Poly& b){*this=*this-b;return *this;}
    auto operator *=(const Poly& b){*this=*this*b;return *this;}
    auto operator /=(const Poly& b){*this=*this/b;return *this;}
    auto operator %=(const Poly& b){*this=*this%b;return *this;}
    auto operator |=(const Poly& b){*this=*this|b;return *this;}
    auto operator &=(const Poly& b){*this=*this&b;return *this;}
    auto operator ^=(const Poly& b){*this=*this^b;return *this;}
    auto operator<<=(const size_t& b){*this=*this<<b;return *this;}
    auto operator>>=(const size_t& b){*this=*this>>b;return *this;}
};
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin>>T;
    map<pair<int,int>,int>mp;
    while(T--)
    {
        int n,k;
        cin>>n>>k;
        if(mp.count({n,k}))
        {
            cout<<mp[{n,k}]<<'\n';
            continue;
        }
        Poly a(10,1);
        Poly b=a.pow(n,k+1);
        int ans=0;
        for(auto p:b.val)ans+=p;
        mp[{n,k}]=ans-1;
        cout<<ans-1<<'\n';
    }
}

