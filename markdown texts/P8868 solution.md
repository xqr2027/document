# 题意

多测，给定长度为 $n$ 的排列 $a,b$，$q$ 次询问，每次给定 $[l,r]$,求 

$$

\sum_{[x,y]\in[l,r]}\left(\max_{i=x}^ya_i\right)\left(\max_{i=x}^yb_i\right)

$$

数据范围：$n,q\leq2.5\times 10^5$。 

# 题解

## 扫描线转化

先考虑排列 $a$。设 $L$ 是 $i$ 左边第一个大于 $a_i$ 的位置，$R$ 是 $i$ 右边第一个大于 $a_i$ 的位置。则区间 $[s,t]$ 满足 $\max(a_s,\cdots a_t)=a_i$ 的充要条件为 $s\in(L,i]\land t\in[i,R)$。

建立平面直角坐标系，点 $(l,r)$ 代表子区间 $[l,r]$。令点 $(a,b)$ 关于 $a$ 的点权为 $w_a$，关于 $b$ 的点权为 $w_b$，则区间 $[l,r]$ 的答案为

$$

\text{Ans}(l,r)=\sum_{x\geq l}\sum_{y=1}^rw_a\cdot w_b

$$

把 $y$ 看做时间轴，则矩形加可转化为区间加，查询答案转化可为查询历史和。现在只需要维护历史和的变化即可。

## 线段树维护

考虑有什么操作。

1. 区间加/减法（原矩形加）；
2. 更新历史和；
3. 查询历史和。

不难发现肯定要维护历史和 $\text{his}$，为了更新历史和还要维护 $a\times b$ 的和 $\text{AB}=\displaystyle\sum ab$，为了更新 $a\times b$ 的和还要再维护一个普通的和 $\displaystyle A=\sum a,B=\sum b$。即每个节点维护一个向量

$$
\text{vec}=
\begin{bmatrix}
\text{his}\\
\text{AB}\\
\text{A}\\
\text{B}\\
1
\end{bmatrix}
$$

把这些操作对向量的影响用矩阵进行刻画。

### 区间加减

一次区间加减会让 $A\gets A+\text{add}$，把转移矩阵写出来：

$$
M_{\text{add}}
=
\begin{bmatrix}
1 & 0 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 & 0 \\
0 & 0 & 1 & 0 & 0 \\
0 & 0 & 0 & 1 & 0 \\
0 & 0 & 0 & 0 & 1 \\
\end{bmatrix}
$$