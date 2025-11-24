//
// Created by Edge51 on 2025/11/21.
//

#ifndef CPPWEBSERVER_DOUBLEBUFFER_H
#define CPPWEBSERVER_DOUBLEBUFFER_H
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include "FixedBuffer.h"

namespace ELogger {

class DoubleBuffer {
public:
    DoubleBuffer(std::uint32_t bufferThreshold = BUFFER_SIZE * 0.8);
    ~DoubleBuffer();
    void SetFlushCallback(std::function<void (const char *, uint32_t)> callback);
    uint32_t Append(const char* data, uint32_t length);
    uint32_t Append(const std::string& data);
    void SwapBuffer();
    void Flush();
    void Stop();
private:
    void FlushThreadFunc();
private:
    std::atomic<bool> isRunning_{ false };
    std::mutex mutex_;
    std::condition_variable cv_;
    std::unique_ptr<FixedBuffer> currBuffer_ = std::make_unique<FixedBuffer>();
    std::vector<std::unique_ptr<FixedBuffer>> fullBuffers_;
    std::uint32_t bufferThreshold_ { 1000 };
    std::function<void (const char *, uint32_t)> flushCallback_;
    std::thread flushThread_;
};

} // namespace Elogger

#endif //CPPWEBSERVER_DOUBLEBUFFER_H