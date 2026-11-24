给定长度为 $21$ 的序列 $A_0,A_1$，记 $x$ 的二进制表示为 $b$，$b_i$ 为 $b$ 从低位起的第 $i$ 位，定义 

$$
f(x)=\prod_{i=0}^{20}A_{b_i}(i),s_x=\sum_{i=1}^nf(i+dep_{\text{LCA}(i,x)})
$$

对于所有 $x\in[1,n]$ 求 $s_x$ 的值。

---
<!-- 因为 $f(x)$ 只与每一位的值的乘积有关，所以可以用类似光速幂的思想，预处理出低 $8$ 位和高 $13$ 位的值， -->

以下推导中 $\text{anc}(a)$ 为 $a$ 所有的祖先，$\text{subtree}(a)$ 为 $a$ 的子树。**这两个均是集合**。

定义基础贡献：

$$
\begin{aligned}
W_u&=\sum_{v\in\text{subtree}(u)}f(v+dep_u)\\
E_u&=\sum_{v\in\text{subtree}(u)}f(v+dep_{fa_u})\\
&=\sum_{v\in\text{subtree}(u)}f(v+dep_u-1)
\end{aligned}
$$

并认为 $E_{rt}=0$，则可以分类考虑对 $s_u$ 的贡献：

1. $\text{LCA}=u$，贡献为 $W_u$；
2. $\text{LCA}=fa_u$，贡献为 $\text{subtree}(fa_u)$ 的贡献减去 $\text{subtree}(u)$ 的贡献，即为 $W_{fa_u}-E_u$；
3. $\cdots$

所以 

$$
s_x=\sum_{v\in\text{anc}(x)}W_v-\sum_{v\in\text{anc}(x)}E_{v}
$$

令 $P$ 为 $W$ 的树上前缀和，$Q$ 为 $E$ 的树上前缀和，则有 $s_x=P_x-Q_x$。

---
$P,Q$ 可以在已知 $W,E$ 后 $\mathcal O(n)$ dfs 一遍求出，所以考虑求出 $W,E$。

令 $st[x]=\text{dfn}[x],en[x]=\text{dfn}[x]+\text{siz}[x]-1$ 为 $\text{subtree}(x)$ 在 dfs 序中的区间，则有

$$
\begin{aligned}
W_u&=\sum_{pos=st[u]}^{en[u]}f(\text{dfn}[pos]+dep_u)\\
E_u&=\sum_{pos=st[u]}^{en[u]}f(\text{dfn}[pos]+dep_u-1)\\
\end{aligned}
$$

令 $R(x,d)=\sum_{i=1}^xf(\text{dfn}[i]+d)$，则 

$$
\begin{aligned}
W_u&=R(en[u],dep_u)-R(st[u]-1,dep_u)\\
E_u&=R(en[u],dep_u-1)-R(st[u]-1,dep_u-1)\\
\end{aligned}
$$

故离线扫描线，把询问挂在节点上，按顺序扫一遍便可得出答案。

容易发现朴素插入是 $\mathcal O(1)$ 的，查询却是 $\mathcal O(n)$ 的，且两种操作发生的次数均是 $\mathcal O(n)$ 这非常的不平衡。考虑平衡复杂度,在插入时用 $\mathcal O(n/B)$ 预处理答案，查询时用 $\mathcal O(B)$ 求答案，就可以把复杂度降下来，容易发现在 $B=\sqrt n$ 时取到最优复杂度 $\mathcal O(n\sqrt n)$。

**以下均认为 $x_1/x_2$ 是 $x$ 高 $13$ 位/低 $8$ 位的值或贡献。**

具体实现上，我取 $B=2^8$，先把 $f$ 拆成 $f_1$ 与 $f_2$ 的乘积，维护一个桶 `ans[][]`，`ans[i][j]` 意为在已插入的这些数中低 $8$ 位为 $j$ 的数 $k$ 的 $f_1(i+k_1)$ 的和。在插入 `v` 时暴力遍历一遍 `i` 更新 `ans[i][v_2]`，在查询时对于参数 `h` 暴力遍历 `ans[h][low]` 求解即可。