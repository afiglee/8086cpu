#include "disassm.h"
#include <stdexcept>
#include <memory>
#include <sstream>
#include <iostream>
#include <typeinfo>


using std::range_error;
//using std::runtime_error;
//using std::make_shared;
using std::stringstream;
using std::shared_ptr;

Disassm::Disassm(enum FLAVOUR flavour) :
m_flavour(flavour),
m_opcodes{
    shared_ptr<OpCode>(new OpCode0{"add", 0x00}),
    shared_ptr<OpCode>(new OpCode0{"add", 0x01}),
    shared_ptr<OpCode>(new OpCode0{"add", 0x02}),
    shared_ptr<OpCode>(new OpCode0{"add", 0x03}),
    shared_ptr<OpCode>(new OpCodeTwo{"add al", 0x04}),
    shared_ptr<OpCode>(new OpCodeThree{"add ax", 0x05}),
}
{
}

pOpCode Disassm::decode(const bin_string& inp, size_t& offset) {
    const uint8_t &code = inp[0];
    return m_opcodes[code]->get(inp, offset);
}