#pragma once
#include <stdint.h>
using namespace std;

/* 判断主机字节序，1代表big endian, 0代表小端，2代表unknown */
int judgeByteOrder()
{
    union JudgeByteOrder
    {
        uint16_t a;
        char b;
    } u;

    u.a = 0x1234;
    if (u.b == 0x34)
    {
        // littleEndian
        return 0;
    }
    else if (u.b == 0x12)
    {
        // bigEndian
        return 1;
    }
    return 2;
}
