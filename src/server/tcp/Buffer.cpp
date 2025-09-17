//
// Created by liuyu on 7/6/2025.
//

#include "Buffer.h"


Buffer::Buffer() {
}

void Buffer::Append(const char *str, uint32_t len) {
    m_buffer.append(str, len);
}

void Buffer::SetContent(const std::string &content) {
    m_buffer = content;
}

uint32_t Buffer::Size() const {
    return m_buffer.length();
}

void Buffer::Clear() {
    m_buffer.clear();
}

const char *Buffer::c_str() const {
    return m_buffer.c_str();
}