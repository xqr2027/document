import random
import os
import json

TODO_FILE = "todolist1.txt"
DONE_FILE = "done.txt"
META_FILE = "luogu_problems.json"
TAG_FILE = "luogu_tags.json"          # 标签映射文件

# ===== 要排除的标签（含有任意一个的题目都会被删去） =====
DELDTE_LIST = [
    # 100,   # cdq 分治
    # 232,   # 树套树
    # 229,   # 动态树 LCT
    # 382,   # 动态树分治
    # 318,   # 构造 
    # 365,   # Ad-hoc
    # 3,     # 动态规划 DP
    # 146,   # 动态规划优化
    # 7,     # 贪心
]
DELDTE_SET = set(DELDTE_LIST)

# ===== 洛谷难度中文映射 =====
DIFF_CN = {
    0: "暂无评定",
    1: "入门",
    2: "普及−",
    3: "普及",
    4: "普及+/提高−",
    5: "提高",
    6: "提高+/省选−",
    7: "省选/NOI−",
    8: "NOI/NOI+/CTSC",
}

def diff_to_cn(d):
    if d is None or d == "":
        return "未知"
    try:
        return DIFF_CN.get(int(d), f"未知({d})")
    except (ValueError, TypeError):
        return str(d)

# ===== 读取标签映射，只保留算法标签（type == 2） =====
def load_tag_mapping():
    """返回 {tag_id: tag_name} 字典，仅包含算法标签"""
    if not os.path.exists(TAG_FILE):
        print(f"提示：未找到 {TAG_FILE}，将只显示标签 ID。")
        return {}

    with open(TAG_FILE, "r", encoding="utf-8") as f:
        data = json.load(f)

    # 情况1：原始 API 格式，包含 "tags" 数组
    if isinstance(data, dict) and "tags" in data and isinstance(data["tags"], list):
        mapping = {}
        for tag in data["tags"]:
            # 只保留 type == 2 的算法标签
            if tag.get("type") == 2 and "id" in tag and "name" in tag:
                mapping[int(tag["id"])] = tag["name"]
        return mapping

    # 情况2：精简映射格式 { "1": "模拟", ... }
    # 无法判断类型，直接全部返回（建议使用原始格式重新生成）
    if isinstance(data, dict):
        mapping = {}
        for k, v in data.items():
            try:
                mapping[int(k)] = v
            except ValueError:
                pass
        return mapping

    return {}

# ===== 读取待做列表 =====
if not os.path.exists(TODO_FILE):
    print(f"错误：找不到 {TODO_FILE}")
    exit(1)

with open(TODO_FILE, "r", encoding="utf-8") as f:
    todo_pids = [line.strip() for line in f if line.strip()]

if not todo_pids:
    print("待做列表为空。")
    exit(1)

# ===== 读取题目元数据 =====
if not os.path.exists(META_FILE):
    print(f"错误：找不到 {META_FILE}，无法按标签筛选。")
    exit(1)

with open(META_FILE, "r", encoding="utf-8") as f:
    meta = json.load(f)

pid_to_tags = {}
pid_to_name = {}
pid_to_diff = {}
for p in meta:
    pid_to_tags[p["pid"]] = set(p.get("tags", []))
    pid_to_name[p["pid"]] = p.get("name", "")
    pid_to_diff[p["pid"]] = p.get("difficulty", "")

# ===== 加载标签 ID -> 名称（仅算法标签） =====
tag_id_to_name = load_tag_mapping()

# ===== 按 DELDTE_LIST 排除 =====
before = len(todo_pids)
if DELDTE_SET:
    todo_pids = [
        pid for pid in todo_pids
        if not (DELDTE_SET & pid_to_tags.get(pid, set()))
    ]
    print(f"已排除标签 {sorted(DELDTE_SET)}，剩余 {len(todo_pids)} / {before} 题")

if not todo_pids:
    print("排除后没有可抽的题目。")
    exit(1)

# ===== 读取已做列表 =====
if os.path.exists(DONE_FILE):
    with open(DONE_FILE, "r", encoding="utf-8") as f:
        done_pids = [line.strip() for line in f if line.strip()]
else:
    done_pids = []

done_set = set(done_pids)
remaining = [pid for pid in todo_pids if pid not in done_set]

if not remaining:
    print("恭喜！所有可做的题目都已完成。")
    exit(0)

# ===== 随机抽一个 =====
chosen = random.choice(remaining)

print("=" * 40)
print(f"随机抽到的题号：{chosen}")
if pid_to_name.get(chosen):
    print(f"题目名称：{pid_to_name[chosen]}")
if pid_to_tags.get(chosen):
    # 只显示算法标签
    algo_tag_names = []
    for tid in sorted(pid_to_tags[chosen]):
        if tid in tag_id_to_name:          # 只显示在算法标签映射中的标签
            algo_tag_names.append(tag_id_to_name[tid])
    if algo_tag_names:
        print(f"算法标签：{', '.join(algo_tag_names)}")
    else:
        print("算法标签：无")
if pid_to_diff.get(chosen) not in (None, ""):
    print(f"难度：{diff_to_cn(pid_to_diff[chosen])}")

print("=" * 40)

# ===== 追加到已做列表 =====
with open(DONE_FILE, "a", encoding="utf-8") as f:
    f.write(chosen + "\n")

# ===== 重新读取已做列表用于展示 =====
with open(DONE_FILE, "r", encoding="utf-8") as f:
    done_pids = [line.strip() for line in f if line.strip()]

print(f"\n当前已做 {len(done_pids)} 题，前 5 道：")
for pid in done_pids[:5]:
    print(f"  {pid}")

print(f"\n剩余未做：{len(remaining) - 1} 题")