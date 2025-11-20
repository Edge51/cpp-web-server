//
// Created by Edge51 on 2025/10/20.
//

#ifndef CPPWEBSERVER_DISABLECOPY_H
#define CPPWEBSERVER_DISABLECOPY_H

class DisableCopy {
public:
    DisableCopy() = default;
    DisableCopy(const DisableCopy&) = delete;
    DisableCopy& operator=(const DisableCopy& other) = delete;
};

#endif //CPPWEBSERVER_DISABLECOPY_H