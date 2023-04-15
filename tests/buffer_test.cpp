#include "../src/buffer.h"
#include <cstdio>

int main() {
    Buffer<int> buf(10);
    for (int i = 0; i < 16; i++) {
        buf.push(i + 1);
    }
    
    {
        auto data = buf.getVector();
        if (data.size() != 10) {
            return 1;
        }
        int expected[] = {7,8,9,10,11,12,13,14,15,16};
        for (int i = 0; i < data.size(); i++) {
            if(expected[i] != data[i]) {
                return 1;
            }
        }
    }
    

    buf.setSize(12);
    buf.push(101);
    buf.push(102);
    {
        auto data = buf.getVector();
        int expected[] = {9,10,11,12,13,14,15,16,16,16,101,102};
        for (int i = 0; i < data.size(); i++) {
            if(expected[i] != data[i]) {
                return 1;
            }
        }
    }
    

    buf.setSize(8);
    {
        auto data = buf.getVector();
        int expected[] = {9,10,11,12,13,14,15,16};
        for (int i = 0; i < data.size(); i++) {
            if(expected[i] != data[i]) {
                return 1;
            }
        }
    }

    return 0;
}