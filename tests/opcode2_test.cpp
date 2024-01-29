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

TEST(OpCode2, test_offset_out_of_range)
{
    OpCode2 opCode("and", 0x22);
 
    size_t offset = 1;
    bin_string inp;
    inp += (uint8_t) 0;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode2, test_truncated_instruction)
{
    OpCode2 opCode("and", 0x22);
 
    size_t offset = 0;
    bin_string inp;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode2, test_truncated_instruction2)
{
    OpCode2 opCode("and", 0x22);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0x40;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode2, test_na_opcode)
{
    OpCode2 opCode("and", 0x22);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0x80;
    EXPECT_THROW(opCode.get(inp, offset), runtime_error);
}

typedef struct sTest {
    uint8_t code;
    string mnemonic;
}sTest;

TEST(OpCode2, test_register2register)
{
    OpCode2 opCode("and", 0x22);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp.resize(2);

    array<sTest, 15> tests {{
        {0xC0, "and al, al"},
        {0xC1, "and al, cl"},
        {0xC2, "and al, dl"},
        {0xC3, "and al, bl"},
        {0xC4, "and al, ah"},
        {0xC5, "and al, ch"},
        {0xC6, "and al, dh"},
        {0xC7, "and al, bh"},
        {0xC8, "and cl, al"},
        {0xD0, "and dl, al"},
        {0xD8, "and bl, al"},
        {0xE0, "and ah, al"},
        {0xE8, "and ch, al"},
        {0xF0, "and dh, al"},
        {0xF8, "and bh, al"},
    }
    };
    
    for (auto & t:tests) {  
        inp[1] =  t.code; 
        pOpCode ret = opCode.get(inp, offset);
        EXPECT_TRUE(offset == 2);
        EXPECT_TRUE(ret->mnemonic() == t.mnemonic);
        offset = 0;
    }
}

TEST(OpCode2, test_register2pointer)
{
    OpCode2 opCode("and", 0x22);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp.resize(2);

    array<sTest, 14> tests {{
        {0x00, "and al, byte ptr [bx + si]"},
        {0x01, "and al, byte ptr [bx + di]"},
        {0x02, "and al, byte ptr [bp + si]"},
        {0x03, "and al, byte ptr [bp + di]"},
        {0x04, "and al, byte ptr [si]"},
        {0x05, "and al, byte ptr [di]"},
        {0x07, "and al, byte ptr [bx]"},
        {0x08, "and cl, byte ptr [bx + si]"},
        {0x10, "and dl, byte ptr [bx + si]"},
        {0x18, "and bl, byte ptr [bx + si]"},
        {0x20, "and ah, byte ptr [bx + si]"},
        {0x28, "and ch, byte ptr [bx + si]"},
        {0x30, "and dh, byte ptr [bx + si]"},
        {0x38, "and bh, byte ptr [bx + si]"},
    }
    };
    
    for (auto & t:tests) {  
        inp[1] =  t.code; 
        pOpCode ret = opCode.get(inp, offset);
        EXPECT_TRUE(offset == 2);
        EXPECT_TRUE(ret->mnemonic() == t.mnemonic);
        offset = 0;
    }
    inp[1] = 0x06;
    EXPECT_THROW(opCode.get(inp, offset), runtime_error);
}

TEST(OpCode2, test_register2pointerWithData)
{
    OpCode2 opCode("and", 0x22);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0; //placeholder, tests will replace it
    inp += (uint8_t) 0x13;
    
    array<sTest, 14> tests {{
        {0x40, "and al, byte ptr [bx + si + 0x13]"},
        {0x41, "and al, byte ptr [bx + di + 0x13]"},
        {0x42, "and al, byte ptr [bp + si + 0x13]"},
        {0x43, "and al, byte ptr [bp + di + 0x13]"},
        {0x44, "and al, byte ptr [si + 0x13]"},
        {0x45, "and al, byte ptr [di + 0x13]"},
        {0x47, "and al, byte ptr [bx + 0x13]"},
        {0x48, "and cl, byte ptr [bx + si + 0x13]"},
        {0x50, "and dl, byte ptr [bx + si + 0x13]"},
        {0x58, "and bl, byte ptr [bx + si + 0x13]"},
        {0x60, "and ah, byte ptr [bx + si + 0x13]"},
        {0x68, "and ch, byte ptr [bx + si + 0x13]"},
        {0x70, "and dh, byte ptr [bx + si + 0x13]"},
        {0x78, "and bh, byte ptr [bx + si + 0x13]"},
    }
    };
    
    for (auto & t:tests) {  
        inp[1] =  t.code; 
        pOpCode ret = opCode.get(inp, offset);
        EXPECT_TRUE(offset == 3);
        EXPECT_TRUE(ret->mnemonic() == t.mnemonic);
        offset = 0;
    }
    inp[1] = 0x46;
    EXPECT_THROW(opCode.get(inp, offset), runtime_error);
}