import json
import random
import os
import re

PROBLEMS_FILE = "luogu_problems.json"
TAGS_FILE = "luogu_tags.json"
DONE_FILE = "done.txt"

# 全局变量
TAG_ID_TO_NAME = {}
TAG_NAME_TO_ID = {}
ALGO_TAG_IDS = set()

# 洛谷官方最新难度映射表 (0-8)
DIFFICULTY_MAP = {
    0: "暂无评定",
    1: "入门",
    2: "普及-",
    3: "普及",
    4: "普及+/提高-",
    5: "提高",
    6: "提高+/省选-",
    7: "省选/NOI-",
    8: "NOI/NOI+/CTS"
}

def get_difficulty_text(diff):
    """将数字难度直接转换为文字描述，例如 '提高'"""
    diff_int = int(diff) if str(diff).isdigit() else 0
    return DIFFICULTY_MAP.get(diff_int, "未知")

# ================= 数据加载与保存 =================

def load_tags():
    global TAG_ID_TO_NAME, TAG_NAME_TO_ID, ALGO_TAG_IDS
    if not os.path.exists(TAGS_FILE): return
    try:
        with open(TAGS_FILE, "r", encoding="utf-8") as f:
            raw_data = json.load(f)
        tag_list = []
        if isinstance(raw_data, dict) and "tags" in raw_data: tag_list = raw_data["tags"]
        elif isinstance(raw_data, list): tag_list = raw_data
        elif isinstance(raw_data, dict):
            for k, v in raw_data.items():
                TAG_ID_TO_NAME[str(k)] = v; TAG_ID_TO_NAME[int(k)] = v; TAG_NAME_TO_ID[v] = str(k)
            return
        for item in tag_list:
            tid = item.get("id"); tname = item.get("name"); ttype = item.get("type")
            if tid is not None and tname is not None:
                TAG_ID_TO_NAME[str(tid)] = tname; TAG_ID_TO_NAME[int(tid)] = tname; TAG_NAME_TO_ID[tname] = str(tid)
                if ttype == 2:
                    ALGO_TAG_IDS.add(int(tid)); ALGO_TAG_IDS.add(str(tid))
    except Exception: pass

def load_problems():
    if not os.path.exists(PROBLEMS_FILE): return []
    try:
        with open(PROBLEMS_FILE, "r", encoding="utf-8") as f: return json.load(f)
    except json.JSONDecodeError: return []

def save_problems(data):
    with open(PROBLEMS_FILE, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=2)

def load_done():
    if not os.path.exists(DONE_FILE): return set()
    try:
        with open(DONE_FILE, "r", encoding="utf-8") as f:
            return set(line.strip().upper() for line in f if line.strip())
    except Exception: return set()

def save_done(done_set):
    try:
        with open(DONE_FILE, "w", encoding="utf-8") as f:
            for pid in sorted(list(done_set)): f.write(pid + "\n")
    except Exception as e: print(f"⚠️ 保存 {DONE_FILE} 失败: {e}")

def format_tags_display(tags_list, max_tags=300):
    if not tags_list: return "无"
    algo_tags = [t for t in tags_list if t in ALGO_TAG_IDS]
    if not algo_tags: return "无算法标签"
    names = [TAG_ID_TO_NAME.get(t, str(t)) for t in algo_tags]
    if len(names) > max_tags:
        return ", ".join(names[:max_tags]) + f" 等{len(names)}个算法标签"
    return ", ".join(names)

# ================= 核心功能 =================

def show_help():
    print("\n" + "="*40)
    print(" 🏆 洛谷刷题控制系统 - 帮助 🏆 ")
    print("="*40)
    print(" 1. 🎲 随机跳题 (支持过滤)")
    print(" 2. ➕ 添加题目")
    print(" 3. ➖ 删除题目 (支持批量)")
    print(" 4. 📚 查看所有题目")
    print(" 5. ✅ 标记已完成 (支持批量)")
    print(" 6. ❌ 取消已完成 (支持批量)")
    print(" 7. 📊 查看刷题进度")
    print(" 8. 🔍 查询题目")
    print(" 9. 🚪 退出系统")
    print("="*40 + "\n")

def random_jump(data, done_set):
    print("\n--- 🎲 随机跳题 ---")
    if not data: print("❌ 题库为空！"); return
    print("模式: 1.完全随机  2.只抽未做(推荐)  3.只抽已做(复习)")
    mode = input("👉 选择模式 (1-3, 默认2): ").strip() or "2"
    target_data = data
    if mode == "2":
        target_data = [p for p in data if p.get("pid") not in done_set]
        if not target_data: print("🎉 题库里的题目你都已经做过了！"); return
    elif mode == "3":
        target_data = [p for p in data if p.get("pid") in done_set]
        if not target_data: print("❌ 你还没做过任何题目！"); return
    prob = random.choice(target_data)
    pid = prob.get("pid", "Unknown")
    status = "✅ 已做" if pid in done_set else "⭕ 未做"
    diff_text = get_difficulty_text(prob.get('difficulty', 0))
    
    print("=" * 60)
    print(f"🎯 推荐题目 : [{pid}] {prob.get('title', '未知')}")
    print(f"📊 难度/状态: {diff_text} | {status}")
    print(f"🏷️  相关标签 : {format_tags_display(prob.get('tags', []), max_tags=3)}")
    print(f"🔗 直达链接 : https://www.luogu.com.cn/problem/{pid}")
    print("=" * 60)

def add_problem(data):
    print("\n--- ➕ 添加新题目 ---")
    pid = input("PID (如 P1001): ").strip().upper()
    if not pid or any(p.get("pid") == pid for p in data): print("❌ PID 为空或已存在！"); return
    title = input("名称: ").strip()
    diff = input("难度 (0-8, 留空0): ").strip() or "0"
    tags_in = input("标签 (逗号分隔): ").strip()
    tags = []
    if tags_in:
        for t in tags_in.split(","):
            t = t.strip()
            if not t: continue
            if t in TAG_NAME_TO_ID: tags.append(int(TAG_NAME_TO_ID[t]))
            elif t.isdigit() or (t.startswith('-') and t[1:].isdigit()): tags.append(int(t))
            else: tags.append(t)
    data.append({"pid": pid, "title": title, "difficulty": int(diff) if diff.isdigit() else 0, "tags": tags})
    save_problems(data)
    print(f"🎉 成功添加: [{pid}] {title}")

def delete_problem(data):
    print("\n--- ➖ 删除题目 ---")
    pids_input = input("请输入要删除的 PID (多个请用逗号或空格分隔): ").strip()
    if not pids_input: print("❌ 不能为空！"); return
    pids = [p.strip().upper() for p in re.split(r'[,\s]+', pids_input) if p.strip()]
    original_len = len(data)
    new_data = [p for p in data if p.get("pid") not in pids]
    deleted = original_len - len(new_data)
    if deleted > 0:
        data.clear(); data.extend(new_data); save_problems(data)
        print(f"🗑️ 成功删除 {deleted} 道题目！")
    else:
        print(f"❌ 未找到任何匹配的 PID！")

def mark_done(data, done_set):
    print("\n--- ✅ 标记已完成 ---")
    pids_input = input("请输入 PID (多个请用逗号或空格分隔): ").strip()
    if not pids_input: print("❌ 不能为空！"); return
    pids = [p.strip().upper() for p in re.split(r'[,\s]+', pids_input) if p.strip()]
    problem_pids = {p.get("pid") for p in data}
    added = 0; skipped = 0; not_in_db = 0
    for pid in pids:
        if pid in done_set: skipped += 1
        elif pid not in problem_pids:
            not_in_db += 1
            print(f"⚠️  警告: {pid} 不在题库中，已跳过。")
        else:
            done_set.add(pid); added += 1
    save_done(done_set)
    print(f"🎉 成功标记 {added} 题为已完成！")
    if not_in_db > 0: print(f"⚠️  {not_in_db} 个 PID 不在题库中，已跳过。")
    if skipped > 0: print(f"⏭️  {skipped} 题之前已标记过。")

def unmark_done(done_set):
    print("\n--- ❌ 取消已完成 ---")
    pids_input = input("请输入要取消的 PID (多个请用逗号或空格分隔): ").strip()
    if not pids_input: print("❌ 不能为空！"); return
    pids = [p.strip().upper() for p in re.split(r'[,\s]+', pids_input) if p.strip()]
    removed = 0; skipped = 0
    for pid in pids:
        if pid in done_set: done_set.remove(pid); removed += 1
        else: skipped += 1
    save_done(done_set)
    print(f"🗑️ 成功取消 {removed} 题的完成标记！(跳过未标记 {skipped} 题)")

def show_progress(data, done_set):
    print("\n--- 📊 刷题进度统计 ---")
    total = len(data)
    done_count = len([p for p in data if p.get("pid") in done_set])
    extra_done = [pid for pid in done_set if not any(p.get("pid") == pid for p in data)]
    print(f"📚 题库总题数 : {total}")
    print(f"✅ 题库内已做 : {done_count}")
    print(f"⭕ 题库内未做 : {total - done_count}")
    if total > 0: print(f"📈 题库完成率 : {(done_count / total) * 100:.1f}%")
    if extra_done: print(f"📌 题库外已做 : {len(extra_done)} 题")

def show_all(data, done_set):
    print(f"\n--- 📚 当前题库 (共 {len(data)} 道题) ---")
    if not data: print("题库为空。"); return
    for i, p in enumerate(sorted(data, key=lambda x: x.get("pid", "")), 1):
        pid = p.get("pid", "Unknown")
        status = "✅" if pid in done_set else "⭕"
        diff_text = get_difficulty_text(p.get('difficulty', 0))
        print(f"{i}. {status} [{pid}] {p.get('title')} | 难度: {diff_text} | 标签: {format_tags_display(p.get('tags', []), max_tags=2)}")

def search_problem(data, done_set):
    print("\n--- 🔍 查询题目 ---")
    keyword = input("请输入关键字 (支持 PID / 题目名称 / 标签名或ID): ").strip()
    if not keyword: print("❌ 关键字不能为空！"); return
    keyword_lower = keyword.lower(); keyword_upper = keyword.upper()
    keyword_tag_id = None
    if keyword.isdigit() or (keyword.startswith('-') and keyword[1:].isdigit()): keyword_tag_id = int(keyword)
    elif keyword in TAG_NAME_TO_ID: keyword_tag_id = int(TAG_NAME_TO_ID[keyword])
    results = []
    for p in data:
        pid = p.get("pid", ""); title = p.get("title", ""); tags = p.get("tags", [])
        match = False
        if keyword_upper in pid.upper(): match = True
        elif keyword_lower in title.lower(): match = True
        elif keyword_tag_id is not None and keyword_tag_id in tags: match = True
        else:
            for t in tags:
                t_name = TAG_ID_TO_NAME.get(t, str(t)).lower()
                if keyword_lower in t_name: match = True; break
        if match: results.append(p)
    if not results: print(f"❌ 未找到与 '{keyword}' 相关的题目。"); return
    print(f"✅ 找到 {len(results)} 道相关题目：\n" + "-" * 65)
    for i, p in enumerate(results, 1):
        pid = p.get("pid", "Unknown"); status = "✅" if pid in done_set else "⭕"
        diff_text = get_difficulty_text(p.get('difficulty', 0))
        print(f"{i}. {status} [{pid}] {p.get('title')}")
        print(f"   难度: {diff_text} | 标签: {format_tags_display(p.get('tags', []), max_tags=300)}")
    print("-" * 65)

# ================= 主程序 =================

def main():
    print("⏳ 正在初始化系统...")
    load_tags(); data = load_problems(); done_set = load_done()
    print(f"✅ 初始化完成！(题库: {len(data)} 题, 已完成: {len(done_set)} 题)")
    print("💡 提示：输入 'help' 查看操作面板，直接输入数字执行命令。\n")
    while True:
        try:
            choice = input("👉 洛谷 > ").strip()
            if not choice: continue
            if choice.lower() in ['help', 'h', '?']: show_help()
            elif choice == "1": random_jump(data, done_set)
            elif choice == "2": add_problem(data)
            elif choice == "3": delete_problem(data)
            elif choice == "4": show_all(data, done_set)
            elif choice == "5": mark_done(data, done_set)
            elif choice == "6": unmark_done(done_set)
            elif choice == "7": show_progress(data, done_set)
            elif choice == "8": search_problem(data, done_set)
            elif choice in ["9", "exit", "quit", "q"]:
                print("👋 感谢使用，祝你 AK IOI！再见！"); break
            else: print(f"❌ 未知指令 '{choice}'。输入 'help' 查看帮助。")
        except (EOFError, KeyboardInterrupt):
            print("\n👋 检测到退出信号，再见！"); break

if __name__ == "__main__":
    main()