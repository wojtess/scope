#include "dataSupply.h"

supply::data::data(int size): buf(size) {

}

supply::data::data(): buf(0) {
    buf.setSize(100);
    for(int i = 0;i < 100; i++) {
        buf.push(supply::entry {
            1,
            i * 10.0f
        });
    }
    max_value = 100 * 10;
    min_value = 0;
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