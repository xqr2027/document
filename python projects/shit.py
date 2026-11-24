import re
import random
import string
import sys

# ========== 0. 保留字白名单 ==========
RESERVED = set('''
alignas alignof and and_eq asm auto bitand bitor bool break unsigned size_t
case catch char char8_t char16_t char32_t class compl concept const
consteval constexpr constinit const_cast continue co_await co_return
co_yield decltype default delete do double dynamic_cast else enum
explicit export extern false float for friend goto if inline int long
mutable namespace new noexcept not not_eq nullptr operator or or_eq
private protected public register reinterpret_cast requires return
short signed sizeof static static_assert static_cast struct switch
template this thread_local throw true try typedef typeid typename
union unsigned using virtual void volatile wchar_t while xor xor_eq
main std
std cout cin cerr clog endl string vector map set multimap multiset
unordered_map unordered_set pair make_pair queue deque stack priority_queue
list forward_list array tuple bitset iostream fstream ifstream ofstream
istringstream ostringstream stringstream ios sync_with_stdio tie
size begin end rbegin rend push_back pop_back push pop front back empty
clear insert erase find count at first second data c_str length substr
append replace reserve resize assign swap sort stable_sort reverse unique
lower_bound upper_bound binary_search fill fill_n memset memcpy memmove
strlen strcpy strcmp strcat printf scanf puts gets putchar getchar
sprintf sscanf fopen fclose fread fwrite freopen sqrt pow exp log log2
log10 sin cos tan asin acos atan atan2 floor ceil round fabs fmod abs
to_string stoi stol stoll stoul stoull cbegin cend
mt19937 mt19937_64 random_device chrono high_resolution_clock system_clock
steady_clock now time_since_epoch duration_cast milliseconds microseconds
nanoseconds seconds
INT_MAX INT_MIN LLONG_MAX LLONG_MIN UINT_MAX ULLONG_MAX SIZE_MAX
UINT8_MAX UINT16_MAX UINT32_MAX UINT64_MAX INT8_MAX INT16_MAX INT32_MAX INT64_MAX
emplace_back max min top __lg 
'''.split())

RESERVED |= set('''
stdin stdout stderr EOF NULL FILE BUFSIZ FILENAME_MAX L_tmpnam
SEEK_CUR SEEK_END SEEK_SET TMP_MAX _IOFBF _IOLBF _IONBF
errno EILSEQ ERANGE EDOM
assert EXIT_FAILURE EXIT_SUCCESS
RAND_MAX MB_CUR_MAX CLOCKS_PER_SEC
INFINITY NAN HUGE_VAL HUGE_VALF HUGE_VALL
FP_INFINITE FP_NAN FP_NORMAL FP_SUBNORMAL FP_ZERO
FLT_MAX FLT_MIN FLT_EPSILON FLT_DIG FLT_MANT_DIG FLT_RADIX
DBL_MAX DBL_MIN DBL_EPSILON DBL_DIG DBL_MANT_DIG DBL_RADIX
LDBL_MAX LDBL_MIN LDBL_EPSILON LDBL_DIG LDBL_MANT_DIG LDBL_RADIX
M_PI M_E M_SQRT2 M_LOG2E M_LOG10E M_LN2 M_LN10
INT8_C INT16_C INT32_C INT64_C UINT8_C UINT16_C UINT32_C UINT64_C
INTMAX_C UINTMAX_C
INT_LEAST8_MAX INT_LEAST16_MAX INT_LEAST32_MAX INT_LEAST64_MAX
INT_FAST8_MAX INT_FAST16_MAX INT_FAST32_MAX INT_FAST64_MAX
INTMAX_MAX INTMAX_MIN UINTMAX_MAX
PTRDIFF_MAX PTRDIFF_MIN
SIG_ATOMIC_MAX SIG_ATOMIC_MIN
WCHAR_MAX WCHAR_MIN WINT_MAX WINT_MIN
size_t ptrdiff_t int8_t int16_t int32_t int64_t
uint8_t uint16_t uint32_t uint64_t
intptr_t uintptr_t intmax_t uintmax_t
getchar_unlocked putchar_unlocked fread_unlocked fwrite_unlocked
'''.split())
RESERVED |= {'LL', 'ULL', 'll', 'ull', 'LU', 'UL', 'lu', 'ul'}

# 字典序模式固定字符集
LEX_FIRST = string.ascii_lowercase + string.ascii_uppercase                          # a-z + A-Z（52 个首字符，数字不能开头）
LEX_REST  = string.ascii_lowercase + string.ascii_uppercase + string.digits + '_'    # a-z + A-Z + 0-9 + _

# ========== 1. 交互输入：模式 ==========
if len(sys.argv) > 1:
    choice = sys.argv[1].strip()
else:
    choice = input("模式 [1: 随机字符串, 2: 下划线, 3: 字典序]: ").strip()
while choice not in ('1', '2', '3'):
    choice = input("输入无效，请重新选择 [1/2/3]: ").strip()

RAND_LEN = 8
if choice == '1':
    if len(sys.argv) > 2:
        RAND_LEN = int(sys.argv[2])
    else:
        s = input("随机字符串长度（回车默认 8，建议 6~12）: ").strip()
        if s:
            RAND_LEN = int(s)
    if RAND_LEN < 2:
        print(f"长度 {RAND_LEN} < 2，强制设为 2")
        RAND_LEN = 2

# ========== 2. 读取 + 收集用户 #define 宏名 ==========
with open('input.cpp', 'r', encoding='utf-8') as f:
    raw = f.read()

user_macros = set(re.findall(r'^\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)', raw, re.M))
RESERVED |= user_macros

# ========== 3. 保护 # 开头的行 ==========
lines = raw.split('\n')
protected = {}
for i, line in enumerate(lines):
    if line.lstrip().startswith('#'):
        protected[i] = line
        lines[i] = ''
content = '\n'.join(lines)

# ========== 4. 删除注释（保持行数） ==========
content = re.sub(r'//[^\n]*', '', content)
content = re.sub(r'/\*.*?\*/',
                 lambda m: '\n' * m.group(0).count('\n'),
                 content, flags=re.S)

# ========== 5. Token 匹配 ==========
pattern = (
    r'("(?:\\.|[^"\\])*")'
    r"|'(?:\\.|[^'\\])*'"
    r'|\b\d+(?:\.\d*)?(?:[eE][+-]?\d+)?(?:[uU]?(?:ll|LL|[lL])?)?\b'
    r'|[A-Za-z_][A-Za-z0-9_]*'
)

tokens = []
seen = set()
for m in re.finditer(pattern, content):
    t = m.group(0)
    if t.startswith('"') or t.startswith("'"): continue
    if t[0].isdigit(): continue
    if t in seen: continue
    seen.add(t)
    tokens.append(t)

# ========== 6. 生成替换映射 ==========
def gen_lex_names(count, first_chars, rest_chars):
    """按字典序生成：a, b, ..., Z, aa, ab, ..., ZZ, aaa, ..."""
    result = []
    for c in first_chars:
        if len(result) >= count: return result
        result.append(c)
    nf = len(first_chars)
    nr = len(rest_chars)
    length = 2
    while len(result) < count:
        total = nf * (nr ** (length - 1))
        for k in range(total):
            if len(result) >= count: return result
            name = [first_chars[k // (nr ** (length - 1))]]
            rem = k % (nr ** (length - 1))
            for pos in range(length - 2, -1, -1):
                name.append(rest_chars[rem // (nr ** pos)])
                rem %= (nr ** pos)
            result.append(''.join(name))
        length += 1
    return result

if choice == '1':
    first_chars = string.ascii_letters
    all_chars   = string.ascii_letters + string.digits + '_'
    used = set(tokens) | RESERVED
    replacement_map = {}
    for t in tokens:
        for _ in range(1000):
            s = random.choice(first_chars) + \
                ''.join(random.choice(all_chars) for _ in range(RAND_LEN - 1))
            if s not in used:
                used.add(s)
                replacement_map[t] = s
                break
        else:
            print(f"长度 {RAND_LEN} 无法为 {len(tokens)} 个 token 生成唯一名字，请增大长度")
            sys.exit(1)
    mode_name = f'随机字符串（长度 {RAND_LEN}）'

elif choice == '2':
    shuffled = tokens.copy()
    random.shuffle(shuffled)
    replacement_map = {}
    assigned = set()
    counter = 1
    for t in shuffled:
        while True:
            us = '_' * counter
            counter += 1
            if us not in assigned and us != t:
                break
        assigned.add(us)
        replacement_map[t] = us
    mode_name = '下划线'

else:  # choice == '3'
    budget = 2 * (len(tokens) + len(RESERVED)) + 100
    lexnames = gen_lex_names(budget, LEX_FIRST, LEX_REST)
    used = set(tokens) | RESERVED
    replacement_map = {}
    j = 0
    for t in tokens:
        while j < len(lexnames) and lexnames[j] in used:
            j += 1
        if j >= len(lexnames):
            print("字典序名字预算不够")
            sys.exit(1)
        replacement_map[t] = lexnames[j]
        used.add(lexnames[j])
        j += 1
    mode_name = '字典序（首字符 a-z+A-Z，后续 a-z+A-Z+0-9+_）'

# ========== 7. RESERVED 的 token → #define ==========
kw_defines = []
for t in tokens:
    if t in RESERVED:
        kw_defines.append(f'#define {replacement_map[t]} {t}\n')

# ========== 8. 替换主体 ==========
def replace_token(m):
    t = m.group(0)
    if t.startswith('"') or t.startswith("'"): return t
    if t[0].isdigit(): return t
    return replacement_map.get(t, t)

new_content = re.sub(pattern, replace_token, content)

# ========== 9. 还原 # 行 + 在第一个主体代码行之前插入 #define 块 ==========
new_lines = new_content.split('\n')
for i, line in protected.items():
    new_lines[i] = line

insert_pos = 0
while insert_pos < len(new_lines):
    if insert_pos in protected or new_lines[insert_pos].strip() == '':
        insert_pos += 1
    else:
        break

new_lines.insert(insert_pos, ''.join(kw_defines))
new_content = '\n'.join(new_lines)

# ========== 10. 输出 ==========
with open('output.cpp', 'w', encoding='utf-8') as f:
    f.write(new_content)

n_kw = sum(1 for t in tokens if t in RESERVED)
n_user = len(tokens) - n_kw
print(f"已生成 output.cpp（模式：{mode_name}）")
print(f"共 {len(tokens)} 个 token：")
print(f"  - {n_kw} 个 RESERVED → 走 #define")
print(f"  - {n_user} 个用户标识符 → 直接替换")
print(f"#define 块插在第 {insert_pos + 1} 行之前（第一个主体代码行）")