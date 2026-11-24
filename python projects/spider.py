import requests
import json
import time
import random

# 你的目标 URL
BASE_URL = "https://www.luogu.com.cn/problem/list"
PARAMS = {
    "type": "luogu",
    "tag": "41,42,47,50,53,65,100,112,215,232,233,234,289,290,298,316,371,379,465,531",
    "page": 1
}

HEADERS = {
    "User-Agent": (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/120.0.0.0 Safari/537.36"
    ),
    "x-lentille-request": "content-only",
    "x-luogu-type": "content-only",
    "Referer": "https://www.luogu.com.cn/problem/list",
    "X-Requested-With": "XMLHttpRequest",
    "Accept": "application/json; charset=UTF-8",
}

def fetch_page(page: int) -> dict:
    """获取一页题目列表的 JSON 数据"""
    params = PARAMS.copy()
    params["page"] = page
    resp = requests.get(BASE_URL, params=params, headers=HEADERS, timeout=10)
    resp.raise_for_status()
    return resp.json()

def get_total_pages(data: dict) -> int:
    """从第一页的 JSON 里算出总页数"""
    cnt = data["data"]["problems"]["count"]
    per = data["data"]["problems"]["perPage"]
    return (cnt + per - 1) // per

def crawl_all() -> list:
    """爬取所有页，返回题目列表"""
    first = fetch_page(1)
    total = get_total_pages(first)
    print(f"共 {total} 页")

    all_problems = []
    all_problems.extend(first["data"]["problems"]["result"])

    for page in range(2, total + 1):
        time.sleep(random.uniform(2, 4))   # 控制频率，避免被封
        data = fetch_page(page)
        all_problems.extend(data["data"]["problems"]["result"])
        print(f"已抓取第 {page}/{total} 页")

    return all_problems

if __name__ == "__main__":
    problems = crawl_all()

    # 提取字段
    result = []
    for p in problems:
        # 修复 1: tags 现在直接是标签 ID 的整数列表，直接保存即可
        # 修复 2: 题目名称的字段是 "name" 而不是 "title"
        result.append({
            "pid": p.get("pid"),
            "title": p.get("name"),          
            "difficulty": p.get("difficulty"),
            "tags": p.get("tags", []),       # 直接保存标签 ID 列表，例如: [41, 42, 45, ...]
        })

    with open("luogu_problems.json", "w", encoding="utf-8") as f:
        json.dump(result, f, ensure_ascii=False, indent=2)

    print(f"完成，共 {len(result)} 道题，已保存到 luogu_problems.json")