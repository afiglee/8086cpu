#include "basetypes.h"
#include <iostream>
#include <iomanip>
#include <ios>

using std::ostream;
using std::setw;
using std::setfill;
using std::hex;
using std::uppercase;

namespace sim86 {

ostream &operator<<(ostream& os, const uint8_t &u) {
    os << "0x" << setfill('0') << setw(2) << uppercase << hex << (int) u;
    return os;
}

ostream &print16(ostream& os, const uint16_t u) {
    os << "0x" << setfill('0') << setw(4) << uppercase << hex << (int) u;
    return os;
}

ostream &print20(ostream& os, const uint32_t u) {
    uint8_t upper = (u >> 16) & 0x0F;
    uint16_t left = u & 0xFFFF;
    os << "0x" << uppercase << hex << (int) upper;
    os << setfill('0') << setw(4) << uppercase << hex << (int) left;
    return os;
}
    bstring::bstring(uint8_t code){
        append(1, code);
    }
    bstring::bstring(uint8_t code, uint8_t code1){
        append(1, code);
        append(1, code1);
    }

    bstring::bstring(uint8_t code, uint8_t code1, uint8_t code2){
        append(1, code);
        append(1, code1);
        append(1, code2);
    }

    bstring::bstring(uint8_t code, uint8_t code1, uint8_t code2, uint8_t code3){
        append(1, code);
        append(1, code1);
        append(1, code2);
        append(1, code3);
    }

    bstring::bstring(uint8_t code, uint8_t code1, uint8_t code2, uint8_t code3, uint8_t code4){
        append(1, code);
        append(1, code1);
        append(1, code2);
        append(1, code3);
        append(1, code4);
    }

    bstring::bstring(uint8_t *data, size_t size){
        assign(data, size);
    }

}