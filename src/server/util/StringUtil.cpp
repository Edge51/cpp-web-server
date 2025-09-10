//
// Created by Edge51 on 2025/9/11.
//

#include "StringUtil.h"

std::string StringUtil::Trim(const std::string& str)
{
    // 定义需要去除的空白字符集，包括空格、制表符、换行符等
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace); // 找到第一个非空白字符的位置
    if (start == std::string::npos) { // 如果字符串全为空白字符
        return "";
    }
    size_t end = str.find_last_not_of(whitespace); // 找到最后一个非空白字符的位置
    return str.substr(start, end - start + 1); // 截取子串
}