//
// Created by liuyu on 7/6/2025.
//

#ifndef BUFFER_H
#define BUFFER_H

#include <memory>
#include <string>
#include <cstdint>

class Buffer {
public:
    using ptr = std::shared_ptr<Buffer>;
    Buffer();
    void Append(const char *str, uint32_t len);
    void SetContent(const std::string &content);
    uint32_t Size() const;
    void Clear();
    const char *c_str() const;
private:
    std::string m_buffer;
};

#endif //BUFFER_H
