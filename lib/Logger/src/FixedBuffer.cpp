//
// Created by Edge51 on 2025/10/19.
//

#include "FixedBuffer.h"

#include <cstring>
#include <iostream>
#include <ostream>

namespace ELogger {

FixedBuffer::FixedBuffer()
{
    curr_ = data_;
}

FixedBuffer::~FixedBuffer()
{
    curr_ = nullptr;
}

uint32_t FixedBuffer::Append(const char *data, uint32_t length)
{
    uint32_t availSize = AvailSize();
    if (length > availSize) {
        std::memcpy(curr_, data, availSize);
        curr_ += availSize;
        return availSize;
    }
    std::memcpy(curr_, data, length);
    curr_ += length;
    return length;
}

uint32_t FixedBuffer::Append(const std::string &data)
{
    return Append(data.data(), data.length());
}

uint32_t FixedBuffer::AvailSize() const
{
    if (curr_ == nullptr) {
        return 0;
    }
    return static_cast<uint32_t>(End() - curr_);
}

uint32_t FixedBuffer::UsedSize() const
{
    return static_cast<uint32_t>(curr_ - data_);
}

bool FixedBuffer::Empty() const
{
    return curr_ == data_;
}


void FixedBuffer::Reset()
{
    curr_ = data_;
    data_[0] = '\0';
}

const char * const FixedBuffer::Data() const
{
    return data_;
}

const char * FixedBuffer::End() const
{
    return data_ + BUFFER_SIZE;
}

} // namespace ELogger
