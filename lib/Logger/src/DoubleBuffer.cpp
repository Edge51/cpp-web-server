//
// Created by Edge51 on 2025/11/21.
//

#include "DoubleBuffer.h"

#include <iostream>
#include <utility>

ELogger::DoubleBuffer::DoubleBuffer(std::uint32_t bufferThreshold)
    :bufferThreshold_(bufferThreshold), isRunning_(true)
{
    flushThread_ = std::thread(&ELogger::DoubleBuffer::FlushThreadFunc, this);
}

ELogger::DoubleBuffer::~DoubleBuffer()
{
    Stop();
}

void ELogger::DoubleBuffer::SetFlushCallback(std::function<void(const char *, uint32_t)> callback)
{
    flushCallback_ = std::move(callback);
}

uint32_t ELogger::DoubleBuffer::Append(const char *data, uint32_t length)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (length > currBuffer_->AvailSize()) {
        SwapBuffer();
    }
    uint32_t written = currBuffer_->Append(data, length);
    if (written > 0) {
        if (currBuffer_->UsedSize() >= bufferThreshold_) {
            cv_.notify_one();
        }
    }
    return written;
}

uint32_t ELogger::DoubleBuffer::Append(const std::string &data)
{
    return Append(data.data(), data.length());
}

void ELogger::DoubleBuffer::SwapBuffer()
{
    if (currBuffer_->Empty()) {
        return ;
    }
    fullBuffers_.emplace_back(std::move(currBuffer_));
    currBuffer_ = std::make_unique<FixedBuffer>();
}

void ELogger::DoubleBuffer::Flush()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        SwapBuffer();
    }
    cv_.notify_one();
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]() {
        return fullBuffers_.empty();
    });
}

void ELogger::DoubleBuffer::Stop()
{
    if (!isRunning_) {
        return ;
    }
    isRunning_ = false;
    cv_.notify_all();
    if (flushThread_.joinable()) {
        flushThread_.join();
    }
}

void ELogger::DoubleBuffer::FlushThreadFunc()
{
    while (isRunning_) {
        std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [&]() {
                return !fullBuffers_.empty() || !isRunning_;
            });

            std::vector<std::unique_ptr<FixedBuffer>> buffers;
            buffers.swap(fullBuffers_);
        lock.unlock();

        for (auto& buffer : buffers) {
            flushCallback_(buffer->Data(), buffer->UsedSize());
            cv_.notify_one();
        }
    }
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& buffer : fullBuffers_) {
        if (flushCallback_ && !buffer->Empty()) {
            flushCallback_(buffer->Data(), buffer->UsedSize());
        }
    }
    fullBuffers_.clear();
    if (!currBuffer_->Empty() && flushCallback_) {
        flushCallback_(currBuffer_->Data(), currBuffer_->UsedSize());
    }
    currBuffer_->Reset();
    cv_.notify_one();
}
