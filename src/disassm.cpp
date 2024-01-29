#include "disassm.h"
#include <stdexcept>
#include <memory>
#include <sstream>

using std::range_error;
//using std::runtime_error;
//using std::make_shared;
using std::stringstream;






Disassm::Disassm(enum FLAVOUR flavour) :
m_flavour(flavour),
m_opcodes{
    OpCode0("add", 0)
}
{
}

pOpCode Disassm::decode(const bin_string& inp, size_t* offset) {
    if (offset != nullptr) {
        if (inp.empty() || *offset > (inp.size() - 1)) {
            throw range_error("Offset to decode out of range");
        }
    }
    size_t off = offset?*offset:0;
    const uint8_t &code = inp[0];

    return m_opcodes[code].get(inp, off);
}