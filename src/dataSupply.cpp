#include "dataSupply.h"
#include <string>

supply::data::data(int size): buf(size) {

}

supply::data::data(): buf(0) {
    buf.setSize(500);
    max_value = 100 * 10;
    min_value = 0;
    ports.push_back("value01");
    ports.push_back("value02");
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