//
// Created by Edge51 on 2025/10/19.
//

#ifndef CPPWEBSERVER_FIXEDBUFFER_H
#define CPPWEBSERVER_FIXEDBUFFER_H

#include <cstdint>
#include <string>

#include "DisableCopy.h"

namespace ELogger {

constexpr uint32_t BUFFER_SIZE = 1024;

class FixedBuffer : public DisableCopy {
public:
    FixedBuffer();
    ~FixedBuffer();
    uint32_t Append(const char* data, uint32_t length);
    uint32_t Append(const std::string& data);
    uint32_t AvailSize() const;
    uint32_t UsedSize() const;
    void Flush();
    void Reset();
    const char *const Data() const;

private:
    const char* End() const;

private:
    char data_[BUFFER_SIZE] { 0 };
    char *curr_{ nullptr };
};

} // namespace ELogger

#endif // CPPWEBSERVER_FIXEDBUFFER_H