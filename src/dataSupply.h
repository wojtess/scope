#pragma once
#include "buffer.h"
#include <vector>
#include <string>

namespace supply {

    struct entry {
        long timestamp;
        float value;
    };


    class data {
        private:
            
        public:
            std::vector<std::string> ports;
            std::string port;
            Buffer<entry> buf;
            int max_value;
            int min_value;
            
            data();
            data(int size);
            void setSize(int size);
            void push(supply::entry data);
            int getMaxValue();
            int getMinValue();
            std::vector<supply::entry> getValues();
    };

}