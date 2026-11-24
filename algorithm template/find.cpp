#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

// 将字符串转为小写，用于不区分大小写的匹配
std::string to_lower(const std::string& s) {
    std::string res = s;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return res;
}

int main() {
    system("chcp 65001");
    std::string query;
    std::cout << "请输入要查找的文件名(支持模糊匹配): ";
    std::cin >> query;

    std::string lower_query = to_lower(query);
    bool found = false;

    try {
        // 从当前目录 "." 开始，递归遍历所有子文件夹
        for (const auto& entry : fs::recursive_directory_iterator(".")) {
            
            // 忽略文件夹，只处理普通文件
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                std::string lower_filename = to_lower(filename);

                // 如果文件名包含你输入的关键字（例如输入 NTT 会匹配 NTT.hpp）
                if (lower_filename.find(lower_query) != std::string::npos) {
                    found = true;
                    std::cout << "\n========== 找到文件: " << entry.path().string() << " ==========\n";
                    
                    // 读取并输出文件内容
                    std::ifstream file(entry.path());
                    if (file.is_open()) {
                        // 使用 rdbuf() 直接输出整个文件流，效率高
                        std::cout << file.rdbuf(); 
                        std::cout << "\n";
                    } else {
                        std::cout << "❌ 发现文件但无法读取内容。\n";
                    }
                    std::cout << "========================================================\n";
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "文件系统遍历出错: " << e.what() << '\n';
    }

    if (!found) {
        std::cout << "❌ 未在目录及其子文件夹中找到包含 \"" << query << "\" 的文件。\n";
    }

    return 0;
}