//
// Created by Edge51 on 2025/6/8.
//

#ifndef LOGGER_H
#define LOGGER_H

#define LOG(fmt, ...) \
    printf("[%s:%d][%s]" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

class Logger {

};



#endif //LOGGER_H
