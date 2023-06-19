#include "dataSupply.h"
#include <string>
#include <cerrno>
#include <chrono>
#include <cstdio>

supply::data::data(int size): buf(size) {
    ports = serial::list_ports();
}

supply::data::data(): data(0) {
    buf.setSize(500);
    max_value = 100 * 10;
    min_value = 0;
}

void supply::data::refresh() {
    ports = serial::list_ports();
}

void supply::data::setSize(int size) {
    buf.setSize(size);
}

void supply::data::push(supply::entry data) {
    buf.push(data);
}

std::vector<supply::entry> supply::data::getValues() {
    return buf.getVector();
}

int supply::data::getMaxValue() {
    return max_value;
}

int supply::data::getMinValue() {
    return min_value;
}

void supply::data::readData() {
    try {
        if(!serial.isOpen() && !serial.getPort().empty()) {
            serial.open();
        }
        char buffer[10];
        if(serial.available() < sizeof(buffer)) {
            return;
        }
        serial.read((uint8_t*)&buffer, sizeof(buffer));
        // buffer[9] = '\0';
        using namespace std::chrono;
        float value = std::atof(buffer);

        printf("value: %f buffer: %s\n", value, buffer);

        float min = 0xFFFFFFFF;
        float max = 0;

        for(auto vectorValue:buf.getVector()) {
            if(vectorValue.value > max) {
                max = vectorValue.value;
            }
            if(vectorValue.value < min) {
                min = vectorValue.value;
            }
        }

        min_value = min;
        max_value = max;
        
        push(supply::entry {
            duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(),
            (float)value
        });
    }
    catch(const std::invalid_argument& e) {
        if(strcmp(e.what(), "Empty port is invalid.") != 0) {
            throw e;   
        }
        return;
    }
    catch(const serial::IOException& e) {
        if(strncmp(e.what(), "IO Exception: ::tcgetattr", 25) == 0) {
            printf("errno: %s, code: %d\n", std::strerror(errno), errno);
        }
        return;
    }
}