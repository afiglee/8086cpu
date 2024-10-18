#include "basetypes.h"

namespace sim86 {
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
        append(1, code3);
    }

}