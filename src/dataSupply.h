#pragma once
#include "buffer.h"
#include <vector>

namespace supply {

    struct entry {
        long timestamp;
        float value;
    };


    class data {
        private:
            Buffer<entry> buf;
            int max_value;
            int min_value;
        public:
            data();
            data(int size);
            void setSize(int size);
            void push(supply::entry data);
            int getMaxValue();
            int getMinValue();
            std::vector<supply::entry> getValues();
    };

}