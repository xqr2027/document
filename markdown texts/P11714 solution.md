# 题意

给定一张 $n$ 个点，$m$ 条边的有向图，求有多少种删边的方案，使最后的图是一个强连通图。

数据范围：$n\leq 15,m\leq \dfrac{n(n-1)}{2}$

# 题解

看到数据范围，想到状压 DP。

因为强连通图缩点后只有一个点，不好计数，考虑单步容斥，从总方案数中减去不强连通的方案数。

设 $f(S)$ 为当前选的点集为 $S$，且当前图是强连通图的方案数，$E(S,T)$ 为从点集 $S$ 到点集 $T$ 的边数。因为一个非强连通图缩点后会有若干个入度为 $0$ 的点，设为 $C_1,C_2,\ldots,C_k$，并记 $T=\displaystyle\bigcup_{i}C_i$，则一个合法的 $T$ 需满足一下性质：

 - $E(S\setminus T,T)=0$
 - $T=V$ 时 $i\geq 1$。

此时 $T\to S\setminus T$ 的边与 $S\setminus T\to S\setminus T$ 的边任意，则由容斥原理可得：

$$
\begin{aligned}
f(S)&=2^{E(S,S)}-\sum_{\substack{T\neq\varnothing\\ T\subsetneq S}}\sum_{i\geq 1}(-1)^{i-1}\sum_{T=\bigcup_{j=1}\limits^i C_j}\times \left(\prod_{j=1}^if(C_j)\right)\times 2^{E(T,S\setminus T)+E(S\setminus T,S\setminus T)}\\
&=2^{E(S,S)}-\sum_{\substack{T\neq\varnothing\\ T\subsetneq S}}2^{E(T,S\setminus T)+E(S\setminus T,S\setminus T)}\left(\sum_{i\geq 1}(-1)^{i-1}\sum_{T=\bigcup_{j=1}\limits^i C_j}\times\prod_{j=1}^if(C_j)\right)\\
\end{aligned}
$$
发现用括号括起来的部分与 $S$ 无关，记为 $g(T)$，则有

$$

f(S)=2^{E(S,S)}-\sum_{\substack{T\neq\varnothing\\T\subseteq S}}2^{E(T,S\setminus T)+E(S\setminus T,S\setminus T)}g(T)

$$

发现第二条性质不好刻画却好特判，故先忽略。再令 $g'(T,i)=\displaystyle\sum_{T=\bigcup C_j}\prod_{j=1}^if(C_i)$，则有 $g(S)=\displaystyle\sum_{i\geq 1}(-1)^{i-1}g'(S,i)$。

现在要通过 dp 求 $g'$，不妨设 $T\subseteq S$ 且 $\min(T)=\min(S)$，则有

$$

g'(S,i)=\sum_{\substack{T\subseteq S\\\min(S)=\min(T)}}g'(S\setminus T,i-1)\times f(T)

$$

初始值为 $g'(S,1)=f(S)$ 与 $g'(S,0)=[s=\varnothing]$。直接实现复杂度 $\mathcal O(n3^n)$，因为我是大常选手所以 T 了，考虑优化。

直接把 $g'$ 的递推式代回原式，有

$$
\begin{aligned}
g(S)&=\sum_{i\geq 1}(-1)^{i-1}g'(S,i)\\
&=f(S)+\sum_{i\geq 2}(-1)^{i-1}g'(S,i)\\
&=f(S)+\sum_{i\geq 2}(-1)^{i-1}\sum_{\substack{T\subseteq S\\\min(S)=\min(T)}}g'(S\setminus T,i-1)\times f(T)\\
&=f(S)+\sum_{\substack{T\subseteq S\\\min(S)=\min(T)}}f(T)\sum_{i\geq 2}(-1)^{i-1}g'(S\setminus T,i-1)\\
&=f(S)-\sum_{\substack{T\subseteq S\\\min(S)=\min(T)}}f(T)\sum_{i\geq 1}(-1)^{i-1}g'(S\setminus T,i)\\
&=f(S)-\sum_{\substack{T\subseteq S\\\min(S)=\min(T)}}f(T)g(S\setminus T)\\
\end{aligned}
$$

现在 $g'$ 没用了，复杂度 $\mathcal O(3^n)$。

但有一个问题，整理出递推式：

$$
f(S)=2^{E(S,S)}-\sum_{\substack{T\neq\varnothing\\ T\subsetneq S}}2^{E(T,S\setminus T)+E(S\setminus T,S\setminus T)}g(T)\\

g(S)=f(S)-\sum_{\substack{T\subseteq S\\\min(S)=\min(T)}}f(T)g(S\setminus T)

$$

当 $S=T$ 时循环依赖了。此时用一下性质 2，$S=T$ 时 $j\neq 1$，故可以先计算出 $g_S$ 不包含 $f_S$ 的部分，在用 $f_S$ 更新 $g_S$ 即可。