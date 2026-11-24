#include <bits/stdc++.h>
#define a using
#define b namespace
#define c std
#define d constexpr
#define e int
#define v double
#define L if
#define M return
#define W signed
#define X main
#define Y scanf
#define Z for
#define ab while
#define ad __lg
#define ae memset
#define af sizeof
#define ah printf
#define ai continue
#define ak break

a b c;
#define int long long
d e f=1e2+10;
d e g=7e3+10;
d e h=60;
d e j=998244353;
d e o=1e18;
d e t=(j+1)>>1;
d e u=166374059;
d v w=1e-8;
e x,y,z,A,B,C,D[h],E,F[16][9][5][3][2][g];
e G(e H)
{
    e E=H/2,I=H&1;
    E%=j;
    e J=E*((E+1)%j)%j*t%j;
    e K=E*((E+1)%j)%j*((2*E%j+1)%j)%j*u%j;
    e B=((x+1)%j*J%j-K+j)%j*2%j;
    L(I)
    {
        B=(B+(E+1)%j*((x%j-E+j)%j)%j)%j;
    }
    M B;
}
e N(e O,e P,e Q,e R,e S,e U)
{
    L(O<0||P<0||Q<0||R<0||S<0||U<0)
    {
        M 0;
    }
    L(!U)
    {
        M !(O||P||Q||R||S);
    }
    e &B=F[O][P][Q][R][S][U];
    L(~B)
    {
        M B;
    }
    B=0;
    e H=O+P+Q+R+S;
    H*=(x-H+1);
    B+=N(O-1,P,Q,R,S,U-H);
    B+=N(O,P-1,Q,R,S,U-2*H);
    B+=N(O,P,Q-1,R,S,U-3*H);
    B+=N(O,P,Q,R-1,S,U-4*H);
    B+=N(O,P,Q,R,S-1,U-5*H);
    M B;
}
e V[f];
W X()
{
    
    
    Y("%lld",&E);
    V[0]=1;
    Z(e aa=1;aa<16;++aa)
    {
        V[aa]=V[aa-1]*aa;
    }
    ab(E--)
    {
        Y("%lld%lld",&x,&A);
        e ac=ad(x)+1;
        ae(D,0,af(D));
        Z(e aa=1;aa<=ac;++aa)
        {
            D[aa]=x/(1ll<<aa-1)-x/(1ll<<aa);
        }
        B=0;
        Z(e aa=ac,ag=1,I;aa>=1;--aa,ag=I+1)
        {
            I=ag+D[aa]-1;
            B=(B+(G(I)-G(ag-1)+j)%j*aa%j)%j;
        }
        L(x>29)
        {
            ah("%lld\n",B);
            ai;
        }
        Z(e aa=1;aa<=5;++aa)
        {
            A=(A+V[D[aa]]-1)/V[D[aa]];
        }
        ae(F,-1,af(F));
        Z(e aa=B;;++aa)
        {
            e aj=N(D[1],D[2],D[3],D[4],D[5],aa);
            L(aj>=A)
            {
                ah("%lld\n",aa%j);
                ak;
            }
            A-=aj;
        }
    }
    M 0;
}
