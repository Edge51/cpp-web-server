//
// Created by Edge51 on 2025/10/19.
//

#ifndef CPPWEBSERVER_FIXEDBUFFER_H
#define CPPWEBSERVER_FIXEDBUFFER_H

constexpr int BUFFER_SIZE = 1024;

class FixedBuffer {
public:
    FixedBuffer();
    ~FixedBuffer();
private:
    char m_buffer[BUFFER_SIZE];
    char *m_current;
};


#endif //CPPWEBSERVER_FIXEDBUFFER_H