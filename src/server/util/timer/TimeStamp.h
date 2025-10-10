//
// Created by Edge51 on 2025/9/21.
//

#ifndef CPPWEBSERVER_TIMESTAMP_H
#define CPPWEBSERVER_TIMESTAMP_H
#include <string>


class TimeStamp {
public:
    explicit TimeStamp(int64_t microSeconds);
    bool operator==(const TimeStamp &other) const;
    bool operator<(const TimeStamp &other) const;

    [[nodiscard]] std::string ToFormatedString() const;
    int64_t MicroSeconds() const;

    static TimeStamp Now();
    static TimeStamp AddMicroSeconds(const TimeStamp &old, int64_t microSeconds);

private:
    int64_t m_microSeconds;
};


#endif //CPPWEBSERVER_TIMESTAMP_H