#ifndef __DISASSM_H__
#define __DISASSM_H__

#include "opcode.h"
#include "basetypes.h"

using std::shared_ptr;
namespace sim86 {
class Disassm {

public:
    enum {E_BYTE_NONE, E_BYTE_ONE, E_BYTE_TWO, E_BYTE_THREE, E_BYTE_COMPLEX} E_BYTE_LENGTH;
    Disassm(enum FLAVOUR flavour = x8086_88);


    /*This function takes array of uint8_t pasing in inp
      and starts to break it into opcodes of different length,
      depends on opcode itself. Each time it successfully decode
      opcode, it returns it representation in pOpCode.
      offset after that is pointed at the next potential
      opcode. In case there is not enough bytes to
      determine opcode, std::out_of_range is thrown.
      In case this is unknown 0x8088 opcode, exception
      of std::invalid_argument is thrown.
      */
    virtual pOpCode decode(const bstring& inp, size_t& offset);    
protected:
    virtual pOpCode modregrm(const uint8_t &code, const bstring& inp, size_t& offset);
   // pOpCode modrm(const uint8_t &code, bstring& inp, size_t& offset);
    enum FLAVOUR m_flavour;
    //array<shared_ptr<OpCode>, 256> m_opcodes;
   // array<enum E_BYTE_LENGTH, 256> m_opcode_lengths;

};

}

#endif
