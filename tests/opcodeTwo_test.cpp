#include <gtest/gtest.h>
#include <iostream>

#include "opcode.h"
#include "disassm.h"

using std::range_error;
using std::runtime_error;
using std::array;
using std::string;

    //0) Test when offset is more then inp length
    //1) Test when no operands - throws  range_error
    //2) Test when 1 operand but requires 2 - throws range_error
    //3) Test when operand between 0x80..0xBF - throws runtime_error
    //4) Test 0xC0 .. 0xFF cases
    //5) Test 0x00 .. 0x3F cases
    //6) Test 0x40 .. 0x7F cases

TEST(OpCodeTwo, test_offset_out_of_range)
{
    OpCodeTwo opCode("add al", 0x04);
 
    size_t offset = 1;
    bin_string inp;
    inp += (uint8_t) 0;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCodeTwo, test_truncated_instruction)
{
    OpCodeTwo opCode("add al", 0x04);
 
    size_t offset = 0;
    bin_string inp;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCodeTwo, test_register)
{
    OpCodeTwo opCode("add al", 0x04);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0x49;
    
    pOpCode ret = opCode.get(inp, offset);
    std::cout << "mnemonic='" << ret->mnemonic() << "'" << std::endl; 
    EXPECT_TRUE(offset == 2);
    EXPECT_TRUE(ret->mnemonic() == string("add al, 0x49"));
    
}

TEST(OpCodeJmpRel, test_reljump)
{
    OpCodeRelJump opCode("jne", 0x75);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0x75;
    inp += (uint8_t) 0xFF;
    
    pOpCode ret = opCode.get(inp, offset);
    EXPECT_TRUE(offset == 2);
    EXPECT_TRUE(ret->mnemonic() == string("jne -1"));
    
}
