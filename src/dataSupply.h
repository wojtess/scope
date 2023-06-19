#pragma once
#include "buffer.h"
#include <vector>
#include <string>
#include "serial/serial.h"

namespace supply {

    struct entry {
        long timestamp;
        float value;
    };


    class data {
        private:
            
        public:
            std::vector<serial::PortInfo> ports;
            serial::Serial serial;
            Buffer<entry> buf;
            int max_value;
            int min_value;
            
            data();
            data(int size);
            void refresh();
            void setSize(int size);
            void push(supply::entry data);
            int getMaxValue();
            int getMinValue();
            std::vector<supply::entry> getValues();
            void readData();
    };

}