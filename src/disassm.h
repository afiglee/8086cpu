#ifndef __DISASSM_H__
#define __DISASSM_H__

#include "opcode.h"
#include "basetypes.h"

using std::shared_ptr;
namespace afiglee {
class Disassm {

public:
    enum {E_BYTE_NONE, E_BYTE_ONE, E_BYTE_TWO, E_BYTE_THREE, E_BYTE_COMPLEX} E_BYTE_LENGTH;
    Disassm(enum FLAVOUR flavour = x8086_88);
    virtual pOpCode decode(const bstring& inp, size_t& offset);    
protected:
    pOpCode Disassm::modregrm(const uint8_t &code, bstring& inp, size_t& offset);
    enum FLAVOUR m_flavour;
    //array<shared_ptr<OpCode>, 256> m_opcodes;
    array<enum E_BYTE_LENGTH, 256> m_opcode_lengths;

};

}

#endif
