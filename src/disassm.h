#ifndef __DISASSM_H__
#define __DISASSM_H__

#include "opcode.h"

class Disassm {

public:
    Disassm(enum FLAVOUR flavour = x8086_88);
    /* If offset is not nullptr offset will point to start instruction
    */
    virtual pOpCode decode(const bin_string& inp, size_t* offset = nullptr);    
protected:
    enum FLAVOUR m_flavour;
    array<OpCode, 1> m_opcodes;

};


#endif
