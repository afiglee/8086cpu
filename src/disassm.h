#ifndef __DISASSM_H__
#define __DISASSM_H__

#include "opcode.h"

using std::shared_ptr;

class Disassm {

public:
    Disassm(enum FLAVOUR flavour = x8086_88);
    virtual pOpCode decode(const bin_string& inp, size_t& offset);    
protected:
    enum FLAVOUR m_flavour;
    array<shared_ptr<OpCode>, 6> m_opcodes;

};


#endif
