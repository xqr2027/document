#include <bits/stdc++.h>
using namespace std;
#define int long long

signed main()
{
// #ifdef _WIN32
//     system("chcp 65001 > nul");
// #endif
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);

    int n,tab;
    char name[256];   
    char filename[256];
    scanf(" %s%lld%lld%s",name,&n,&tab,filename);
    freopen(filename,"a",stdout);
    printf("\n");

    vector<char> buf[256];
    vector<char*>s(n);
    for(int i = 0; i < n; i++)
    {
        buf[i].resize(256);
        s[i] = buf[i].data();
        scanf(" %s",s[i]);
    }

    auto print=[&]()->void
    {
        for(int i=1;i<=tab;++i)printf("\t");
    };
    print();
    printf("struct %s\n",name);
    print();
    printf("{\n");
    print();
    printf("\tint ");
    for(int i=0;i<n;++i)
    {
        if(i) printf(",");
        printf("%s",s[i]);
    }
    printf(";\n");
    print();
    printf("\t%s()=default;\n",name);
    print();
    printf("\t%s(",name);
    for(int i=0;i<n;++i)
    {
        if(i) printf(",");
        printf("int %s",s[i]);
    }
    printf("):");
    for(int i=0;i<n;++i)
    {
        if(i) printf(",");
        printf("%s(%s)",s[i],s[i]);
    }
    printf("{}\n");
    print();
    printf("};\n");

    fflush(stdout);
    fclose(stdout);
    return 0;
}