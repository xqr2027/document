#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <cmath>
#include <utility>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <cstring>
using namespace std;
using namespace std::chrono;

/* ====================1. 冒泡排序 ====================*/
void bubbleSort(vector<int>& arr)
{
    int n=(int)arr.size();
    for(int i=0;i<n-1;++i)
    {
        bool swapped=false;
        for(int j=0;j<n-1-i;++j)
        {
            if(arr[j]>arr[j+1]){swap(arr[j],arr[j+1]);swapped=true;}
        }
        if(!swapped)break;
    }
}
/* ====================2. 选择排序 ====================*/
void selectionSort(vector<int>& arr)
{
    int n=(int)arr.size();
    for(int i=0;i<n-1;++i)
    {
        int minIdx=i;
        for(int j=i+1;j<n;++j)
            if(arr[j]<arr[minIdx])minIdx=j;
        if(minIdx != i)swap(arr[i],arr[minIdx]);
    }
}
/* ====================3. 插入排序 ====================*/
void insertionSort(vector<int>& arr)
{
    int n=(int)arr.size();
    for(int i=1;i<n;++i)
    {
        int key=arr[i];
        int j=i-1;
        while(j>=0&&arr[j]>key){arr[j+1]=arr[j];--j;}
        arr[j+1]=key;
    }
}
/* ====================4. 希尔排序 ====================*/
void shellSort(vector<int>& arr)
{
    int n=(int)arr.size();
    for(int gap=n/2;gap>0;gap/=2)
    {
        for(int i=gap;i<n;++i)
        {
            int key=arr[i];
            int j=i-gap;
            while(j>=0&&arr[j]>key){arr[j+gap]=arr[j];j-=gap;}
            arr[j+gap]=key;
        }
    }
}
/* ====================5. 归并排序 ====================*/
namespace
{
void mergeSortHelper(vector<int>& arr,vector<int>& tmp,int left,int right)
{
    if(left>=right)return;
    int mid=left +(right-left)/2;
    mergeSortHelper(arr,tmp,left,mid);
    mergeSortHelper(arr,tmp,mid+1,right);
    int i=left,j=mid+1,k=left;
    while(i<=mid&&j<=right)
        tmp[k++]=(arr[i]<=arr[j])?arr[i++]:arr[j++];
    while(i<=mid)  tmp[k++]=arr[i++];
    while(j<=right)tmp[k++]=arr[j++];
    for(int t=left;t<=right;++t)arr[t]=tmp[t];
}
}
void mergeSort(vector<int>& arr)
{
    if(arr.size()<2)return;
    vector<int> tmp(arr.size());
    mergeSortHelper(arr,tmp,0,(int)arr.size()-1);
}
/* ====================6. 快速排序 ====================*/
namespace
{
int partition(vector<int>& arr,int low,int high)
{
    int randIdx=low+rand()%(high-low+1);
    swap(arr[randIdx],arr[high]);
    int pivot=arr[high];
    int i=low-1;
    for(int j=low;j<high;++j)
        if(arr[j]<pivot)swap(arr[++i],arr[j]);
    swap(arr[i+1],arr[high]);
    return i+1;
}
void quickSortHelper(vector<int>& arr,int low,int high)
{
    if(low>=high)return;
    int p=partition(arr,low,high);
    quickSortHelper(arr,low,p-1);
    quickSortHelper(arr,p+1,high);
}
}
void quickSort(vector<int>& arr)
{
    if(arr.size()<2)return;
    quickSortHelper(arr,0,(int)arr.size()-1);
}
/* ====================7. 堆排序 ====================*/
namespace
{
void siftDown(vector<int>& arr,int n,int i)
{
    while(true)
    {
        int largest=i,l=2*i+1,r=2*i+2;
        if(l<n&&arr[l]>arr[largest])largest=l;
        if(r<n&&arr[r]>arr[largest])largest=r;
        if(largest==i)break;
        swap(arr[i],arr[largest]);
        i=largest;
    }
}
}
void heapSort(vector<int>& arr)
{
    int n=(int)arr.size();
    for(int i=n/2-1;i>=0;--i)siftDown(arr,n,i);
    for(int i=n-1;i>0;--i){swap(arr[0],arr[i]);siftDown(arr,i,0);}
}
/* ====================8. 计数排序 ====================*/
void countingSort(vector<int>& arr)
{
    if(arr.empty())return;
    int mn=*min_element(arr.begin(),arr.end());
    int mx=*max_element(arr.begin(),arr.end());
    long long range=(long long)mx-mn+1;
    if(range>100000000LL)return; // 值域>1e8 放弃
    vector<int> cnt((size_t)range,0);
    for(int x:arr)++cnt[x-mn];
    int idx=0;
    for(int v=0;v<(int)range;++v)
        while(cnt[v]-->0)arr[idx++]=v+mn;
}
/* ====================9. 桶排序 ====================*/
void bucketSort(vector<int>& arr)
{
    if(arr.size()<2)return;
    int n=(int)arr.size();
    int mn=*min_element(arr.begin(),arr.end());
    int mx=*max_element(arr.begin(),arr.end());
    if(mn==mx)return;
    long long range=(long long)mx-mn+1;
    int bucketCount=(int)min<long long>({10000LL,range,(long long)n});
    if(bucketCount<1)bucketCount=1;
    double width=(double)range/bucketCount;
    vector<vector<int>> buckets(bucketCount);
    for(int x:arr)
    {
        int idx=(int)((x-mn)/width);
        if(idx>=bucketCount)idx=bucketCount-1;
        buckets[idx].emplace_back(x);
    }
    int k=0;
    for(auto& b:buckets)
    {
        if(b.size()>32)sort(b.begin(),b.end());
        else           insertionSort(b);
        for(int x:b)arr[k++]=x;
    }
}
/* ====================10. 基数排序 ====================*/
void radixSort(vector<int>& arr)
{
    if(arr.size()<2)return;
    int mn=*min_element(arr.begin(),arr.end());
    for(int& x:arr)x-=mn;
    int mx=*max_element(arr.begin(),arr.end());
    constexpr int BITS=8;
    constexpr int RADIX=1<<BITS;      // 256
    constexpr int MASK=RADIX-1;       // 255
    vector<int> output(arr.size());
    int cnt[RADIX];
    for(int shift=0;(mx>>shift)>0;shift+=BITS)
    {
        memset(cnt,0,sizeof(cnt));
        for(int x:arr) ++cnt[(x>>shift)&MASK];
        for(int i=1;i<RADIX;++i) cnt[i]+=cnt[i-1];
        for(int i=(int)arr.size()-1;i>=0;--i)
        {
            int d=(arr[i]>>shift)&MASK;
            output[--cnt[d]]=arr[i];
        }
        arr=output;
    }
    for(int& x:arr)x+=mn;
}
/* ====================6b. 三路快速排序 ====================*/
namespace
{
auto partition3Way(vector<int>& arr,int low,int high)->pair<int,int>
{
    int randIdx=low+rand()%(high-low+1);
    swap(arr[low],arr[randIdx]);
    int pivot=arr[low];
    int lt=low,gt=high,i=low+1;
    while(i<=gt)
    {
        if(arr[i]<pivot)     swap(arr[lt++],arr[i++]);
        else if(arr[i]>pivot)swap(arr[i],arr[gt--]);
        else                 ++i;
    }
    return {lt,gt};
}
void quickSort3WayHelper(vector<int>& arr,int low,int high)
{
    if(low>=high)return;
    auto [lt,gt]=partition3Way(arr,low,high);
    quickSort3WayHelper(arr,low,lt-1);
    quickSort3WayHelper(arr,gt+1,high);
}
}
void quickSort3Way(vector<int>& arr)
{
    if(arr.size()<2)return;
    quickSort3WayHelper(arr,0,(int)arr.size()-1);
}
/* ====================生成模式 ====================*/
enum class Pattern
{
    NearlyAscending,
    NearlyDescending,
    Random,
    FewDistinct 
};
/* ====================数据生成器 ====================*/
class DataGenerator
{
public:
    explicit DataGenerator(unsigned seed=random_device{}()): rnd(seed){}
    vector<int> generate(int n,Pattern pattern,
                         int lo=0,int hi=1000,
                         double noise=0.02,int distinct=0)
    {
        if(n<=0)return {};
        if(lo>hi)swap(lo,hi);
        vector<int> arr(n);
        switch(pattern)
        {
            case Pattern::NearlyAscending:
                buildAscending(arr,lo,hi);disturb(arr,noise);break;
            case Pattern::NearlyDescending:
                buildAscending(arr,lo,hi);reverse(arr.begin(),arr.end());disturb(arr,noise);break;
            case Pattern::Random:
                buildAscending(arr,lo,hi);shuffle(arr.begin(),arr.end(),rnd);break;
            case Pattern::FewDistinct:
            {
                int k=(distinct>0)?distinct:max(1,(int)sqrt((double)n));
                long long span=(long long)hi-lo+1;
                if((long long)k>span)k=(int)span;
                uniform_int_distribution<int> d(lo,hi);
                vector<int> pool;
                pool.reserve(k);
                while((int)pool.size()<k)
                {
                    int v=d(rnd);
                    if(find(pool.begin(),pool.end(),v)==pool.end())pool.emplace_back(v);
                }
                uniform_int_distribution<int> pick(0,k-1);
                for(int& x:arr)x=pool[pick(rnd)];
                break;
            }
        }
        return arr;
    }
private:
    mt19937 rnd;
    void buildAscending(vector<int>& arr,int lo,int hi)
    {
        int n=(int)arr.size();
        if(n==1){arr[0]=lo;return;}
        long long span=(long long)hi-lo;
        for(int i=0;i<n;++i)
            arr[i]=lo+(int)(span*i/(n-1));
    }
    void disturb(vector<int>& arr,double noise)
    {
        int n=(int)arr.size();
        if(n<2||noise<=0)return;
        int swaps=max(1,(int)(noise*n));
        uniform_int_distribution<int> d(0,n-2);
        for(int i=0;i<swaps;++i)
        {
            int j=d(rnd);
            swap(arr[j],arr[j+1]);
        }
    }
};
/* ==================== 显示宽度工具 ====================*/
int displayWidth(const string& s)
{
    int w=0;
    for(size_t i=0;i<s.size();)
    {
        unsigned char c=(unsigned char)s[i];
        if(c<0x80)           {w+=1;i+=1;}
        else if((c>>5)==0x6 ){w+=2;i+=2;}
        else if((c>>4)==0xE ){w+=2;i+=3;}
        else if((c>>3)==0x1E){w+=2;i+=4;}
        else                 {w+=1;i+=1;}
    }
    return w;
}
string padRight(const string& s,int width)
{
    int w=displayWidth(s);
    return s+string(max(0,width-w),' ');
}
string padLeft(const string& s,int width)
{
    int w=displayWidth(s);
    return string(max(0,width-w),' ')+s;
}

/* ====================主流程 ====================*/
int main()
{
    srand((unsigned)time(0));
    system("chcp 65001>nul");
    cout << "请输入 n，n 大于等于 100000 时将不测试 O(n^2)的排序（可省略，默认 10000）：\n> ";
    int n=10000;
    {
        string line;
        getline(cin,line);
        size_t s=line.find_first_not_of(" \t");
        if(s != string::npos)
            try{n=stoi(line.substr(s));} catch(...){}
    }
    if(n <= 0){cerr << "n 必须为正整数\n";return 1;}
    DataGenerator gen(random_device{}());
    struct Case
    {
        string  name;
        Pattern pat;
        double  noise;
        int     distinct;
        int     lo=0;
        int     hi=1000;
    };
    vector<Case> cases =
    {
        {"几乎正序",  Pattern::NearlyAscending, 0.02,0},
        {"几乎逆序",  Pattern::NearlyDescending,0.02,0},
        {"乱序",      Pattern::Random,          0.0, 0},
        {"重复元素多",Pattern::FewDistinct,     0.0, 4},
        {"大值域乱序",Pattern::Random,          0.0, 0,0,10000000},
    };
    vector<pair<string,void(*)(vector<int>&)>> algos;
    if(n<100000)
    {
        algos =
        {
            {"冒泡排序",bubbleSort},
            {"选择排序",selectionSort},
            {"插入排序",insertionSort},
            {"希尔排序",shellSort},
            {"归并排序",mergeSort},
            {"快速排序",quickSort},
            {"三路快排",quickSort3Way},
            {"堆排序",  heapSort},
            {"计数排序",countingSort},
            {"桶排序",  bucketSort},
            {"基数排序",radixSort},
            {"std::sort",[](vector<int>& v){ sort(v.begin(),v.end());}},
        };
    }
    else if(n<1000000)
    {
        algos =
        {
            {"希尔排序",shellSort},
            {"归并排序",mergeSort},
            {"快速排序",quickSort},
            {"三路快排",quickSort3Way},
            {"堆排序",  heapSort},
            {"计数排序",countingSort},
            {"桶排序",  bucketSort},
            {"基数排序",radixSort},
            {"std::sort",[](vector<int>& v){ sort(v.begin(),v.end());}},
        };
    }
    else
    {
        algos =
        {
            {"希尔排序",shellSort},
            {"归并排序",mergeSort},
            {"三路快排",quickSort3Way},
            {"堆排序",  heapSort},
            {"计数排序",countingSort},
            {"桶排序",  bucketSort},
            {"基数排序",radixSort},
            {"std::sort",[](vector<int>& v){ sort(v.begin(),v.end());}},
        };
    }
    const int REPEAT=(n <= 2000)? 20 :(n <= 20000 ? 10 : 3);

    cout << "n=" << n << "，每种算法运行 " << REPEAT << " 次取平均\n\n";

    int siz=(int)cases.size();

   /* ---------- 1)生成数据并写文件 ---------- */
    vector<vector<int>> datas(siz);
    for(int ci=0;ci<siz;++ci)
    {
        datas[ci]=gen.generate(n,cases[ci].pat,
                                 cases[ci].lo,cases[ci].hi,
                                 cases[ci].noise,cases[ci].distinct);
        string fname="data"+to_string(ci+1)+".in";
        ofstream fout(fname);
        if(fout)
        {
            fout << datas[ci].size()<< '\n';
            for(size_t i=0;i<datas[ci].size();++i)
            {
                if(i)fout << ' ';
                fout << datas[ci][i];
            }
            fout << '\n';
        }
    }
    cout << siz << " 种数据已写入 data1.in ~ data" << siz << ".in\n\n";

   /* ---------- 2)单线程顺序计时 ---------- */
    struct Result
    {
        double ms=0;
        bool   ok=true;
    };
    vector<vector<Result>> results(algos.size(),vector<Result>(siz));

    for(int ci=0;ci<siz;++ci)
    {
        vector<int> expected=datas[ci];
        sort(expected.begin(),expected.end());

        for(int ai=0;ai<(int)algos.size();++ai)
        {
            auto& [name,fn]=algos[ai];

           // ---------- 进度条 ----------
            {
                int done =ci*(int)algos.size()+ai;
                int total=siz*(int)algos.size();
                int barW =25;
                int filled=(int)((long long)barW*done/ total);
                ostringstream prog;
                prog << '[';
                for(int k=0;k<barW;++k)prog <<(k<filled ? '#' : '.');
                prog << "] " << setw(3)<<(done*100/ total)<< "% "
                     << '[' <<(ci+1)<< '/' << siz << "] "
                     << padRight(cases[ci].name,12)<< " -> "
                     << '[' <<(ai+1)<< '/' << algos.size()<< "] "
                     << padRight(name,12);
                cerr << '\r' << prog.str()<< "    " << flush;
            }
           // --------------------------------

           // 预热一次，排除首次缓存/内存分配干扰
            {
                vector<int> warm=datas[ci];
                fn(warm);
            }

            double totalMs=0;
            bool   ok     =true;

            for(int r=0;r<REPEAT;++r)
            {
                vector<int> b=datas[ci];
                auto t0=high_resolution_clock::now();
                fn(b);
                auto t1=high_resolution_clock::now();
                totalMs+=duration_cast<duration<double,milli>>(t1-t0).count();
                if(b != expected)ok=false;
            }
            results[ai][ci]={totalMs/ REPEAT,ok};
        }
    }

   // 清进度行
    cerr << "\r" << string(100,' ')<< "\r" << flush;

   /* ---------- 3)列宽 ---------- */
    const int NAME_W=10;
    vector<int> COL_W(siz,NAME_W);
    for(int ci=0;ci<siz;++ci)
    {
        int maxIntLen=1;
        for(int ai=0;ai<(int)algos.size();++ai)
        {
            ostringstream oss;
            oss << fixed << setprecision(3)<< results[ai][ci].ms;
            string s=oss.str();
            size_t dot=s.find('.');
            int len=(dot==string::npos)?(int)s.size():(int)dot;
            if(len>maxIntLen)maxIntLen=len;
        }
        COL_W[ci]=max(maxIntLen+7,NAME_W);
    }

   /* ---------- 4)表头 ---------- */
    cout << padRight("算法",NAME_W);
    for(int ci=0;ci<siz;++ci)
        cout << " | " << padLeft(cases[ci].name,COL_W[ci]);
    cout << '\n';

    cout << string(NAME_W,'-');
    for(int ci=0;ci<siz;++ci)
        cout << "-+-" << string(COL_W[ci],'-');
    cout << '\n';

   /* ---------- 5)数据行 ---------- */
    cout << fixed << setprecision(3);
    for(int ai=0;ai<(int)algos.size();++ai)
    {
        cout << padRight(algos[ai].first,NAME_W);
        for(int ci=0;ci<siz;++ci)
        {
            ostringstream oss;
            oss << fixed << setprecision(3)<< results[ai][ci].ms << " ms";
            string cell=oss.str();
            if(!results[ai][ci].ok)cell="ERR "+cell;
            cout << " | " << padLeft(cell,COL_W[ci]);
        }
        cout << '\n';
    }

    return 0;
}