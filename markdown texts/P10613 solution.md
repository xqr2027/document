题意：求把 $n$ 划分成若干个无序正整数之和的方案数。

$n\leq 2\times 10^5$。

$O(n^2)$ 做法显然且显然过不了，考虑优化。注意到答案即为 $p(n)$，由 $p(n)$ 的 OGF 有

$$
P(x)=\prod_{k\geq 1}\dfrac 1{1-x^k}
$$

即

$$
P(x)\left(\prod_{k\geq 1}\left(1-x^k\right)\right)=1
$$

将欧拉五边形数定理给出的等式代入，有

$$
P(x)\left[1+\sum_{k=1}^\infty(-1)^k\left(x^\frac {k(3k-1)}2+x^\frac {k(3k+1)}2\right)\right]=1
$$

提取 $x^n$ 的系数，有（规定 $n<0$ 时 $p(n)=0$）

$$
p(n)+\sum_{k=1}^\infty(-1)^k\left(p\left(n-\dfrac{k(3k-1)}2\right)+p\left(n-\dfrac{k(3k+1)}2\right)\right)=0
$$

恒成立，移项即得

$$
p(n)=-\sum_{k\geq 1}(-1)^k\left(p\left(n-\dfrac{k(3k-1)}2\right)+p\left(n-\dfrac{k(3k+1)}2\right)\right)
$$

模拟即可。复杂度显然是 $O(n\times\text{card}\{p|p\leq n\})$，因为 $p$ 是 $O(k^2)$ 的，所以总复杂度是 $O(n\sqrt n)$ 的。

定理证明：先证 

$$
G(x)=\prod_{k\geq 1}(1-x^k)=1+\sum_{k=1}^\infty(-1)^k\left(x^\frac {k(3k-1)}2+x^\frac {k(3k+1)}2\right)
$$

展开左边，不难发现为在 $1$ 与 $-x^k$ 中选一个，对于 $x^k$ 这一项相当于构成了 $k$ 的任意一个部分互异划分，设这个集合为 $\mathcal D$，则这个划分的贡献为 $(-1)^{|\mathcal D|}x^k$。设 $k$ 的所有**互异**划分的集合为 $\mathcal S(k)$，则 $x^k$ 的系数为 

$$
[x^k]G(x)=\sum_{\mathcal D\in \mathcal S(k)}(-1)^{|\mathcal D|}.
$$

即划分出的数的个数为偶数的方案数减去个数为奇数的方案数。现在只需证 

$$
[x^k]G(x) \neq 0 \iff k \in \left\{ a \;\middle|\; a=\frac{t(3t-1)}{2} \lor a=\frac{t(3t+1)}{2},\, t \in \mathbb{N} \right\}
$$

对于 $k$ 的任意一个划分 $\{a,b,c,\dots,p\}$，画图：

```plain text
■■■...■■ (长度 a)
■■...■■  (长度 b)
■■...■■  (长度 c)
...
■■...■■  (长度 p)

```

并在图中定义 $B$ 为最下面一行的长度（图中的 $p$），$R$ 为从第一行开始连续满足 $len_i-1=len_{i-1}$ 的行数（默认第一行合法）。分类讨论：

1. $B>R$，则选取最上面的 $R$ 行末尾的一个元素，移至最下方，单开一行；
2. 否则，把最下方一行拆成 $B$ 个元素补在最上面的 $B$ 行的末尾。

不难发现不卡住情况 $12$ 是互逆的，且只有 $B=R=|\mathcal D|$ 时或 $B-1=R=|\mathcal D|$ 会卡住，令 $t=|\mathcal D|$，此时有 $k=\dfrac{t(3t-1)}2$ 或 $k=\dfrac{t(3t+1)}2$ 得证。