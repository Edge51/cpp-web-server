//
// Created by Edge51 on 2025/6/8.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define LOG(fmt, ...) \
    printf("[%s:%d][%s]" fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

class Logger {

};

#define CHK_PRT(expr, log) \
    do { \
      if (!expr) { \
        log;\
      }\
    } while (false)


#endif //LOGGER_H
