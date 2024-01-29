#include <gtest/gtest.h>
#include <iostream>

#include "opcode.h"
#include "disassm.h"

using std::range_error;
using std::runtime_error;
using std::array;
using std::string;


TEST(OpCodeSingle, test_offset_out_of_range)
{
    OpCode opCode("push es", 0x06);
 
    size_t offset = 1;
    bin_string inp;
    inp += (uint8_t) 0x06;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCodeSingle, test_instruction)
{
    OpCode opCode("push es", 0x06);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0x06;
    
    pOpCode ret = opCode.get(inp, offset);
    EXPECT_TRUE(offset == 1);
    EXPECT_TRUE(ret->mnemonic() == string("push es"));
    
}
