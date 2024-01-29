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

TEST(OpCode1, test_offset_out_of_range)
{
    OpCode1 opCode("adc", 0x11);
 
    size_t offset = 1;
    bin_string inp;
    inp += (uint8_t) 0;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode1, test_truncated_instruction)
{
    OpCode1 opCode("adc", 0x11);
 
    size_t offset = 0;
    bin_string inp;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode1, test_truncated_instruction2)
{
    OpCode1 opCode("adc", 0x11);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0x40;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode1, test_na_opcode)
{
    OpCode1 opCode("adc", 0x11);

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

TEST(OpCode1, test_register2register)
{
    OpCode1 opCode("adc", 0x11);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp.resize(2);

    array<sTest, 15> tests {{
        {0xC0, "adc ax, ax"},
        {0xC1, "adc cx, ax"},
        {0xC2, "adc dx, ax"},
        {0xC3, "adc bx, ax"},
        {0xC4, "adc sp, ax"},
        {0xC5, "adc bp, ax"},
        {0xC6, "adc si, ax"},
        {0xC7, "adc di, ax"},
        {0xC8, "adc ax, cx"},
        {0xD0, "adc ax, dx"},
        {0xD8, "adc ax, bx"},
        {0xE0, "adc ax, sp"},
        {0xE8, "adc ax, bp"},
        {0xF0, "adc ax, si"},
        {0xF8, "adc ax, di"},
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

TEST(OpCode1, test_register2pointer)
{
    OpCode1 opCode("adc", 0x11);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp.resize(2);

    array<sTest, 14> tests {{
        {0x00, "adc word ptr [bx + si], ax"},
        {0x01, "adc word ptr [bx + di], ax"},
        {0x02, "adc word ptr [bp + si], ax"},
        {0x03, "adc word ptr [bp + di], ax"},
        {0x04, "adc word ptr [si], ax"},
        {0x05, "adc word ptr [di], ax"},
        {0x07, "adc word ptr [bx], ax"},
        {0x08, "adc word ptr [bx + si], cx"},
        {0x10, "adc word ptr [bx + si], dx"},
        {0x18, "adc word ptr [bx + si], bx"},
        {0x20, "adc word ptr [bx + si], sp"},
        {0x28, "adc word ptr [bx + si], bp"},
        {0x30, "adc word ptr [bx + si], si"},
        {0x38, "adc word ptr [bx + si], di"},
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

TEST(OpCode1, test_register2pointerWithData)
{
    OpCode1 opCode("adc", 0x11);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0; //placeholder, tests will replace it
    inp += (uint8_t) 0x13;
    
    array<sTest, 14> tests {{
        {0x40, "adc word ptr [bx + si + 0x13], ax"},
        {0x41, "adc word ptr [bx + di + 0x13], ax"},
        {0x42, "adc word ptr [bp + si + 0x13], ax"},
        {0x43, "adc word ptr [bp + di + 0x13], ax"},
        {0x44, "adc word ptr [si + 0x13], ax"},
        {0x45, "adc word ptr [di + 0x13], ax"},
        {0x47, "adc word ptr [bx + 0x13], ax"},
        {0x48, "adc word ptr [bx + si + 0x13], cx"},
        {0x50, "adc word ptr [bx + si + 0x13], dx"},
        {0x58, "adc word ptr [bx + si + 0x13], bx"},
        {0x60, "adc word ptr [bx + si + 0x13], sp"},
        {0x68, "adc word ptr [bx + si + 0x13], bp"},
        {0x70, "adc word ptr [bx + si + 0x13], si"},
        {0x78, "adc word ptr [bx + si + 0x13], di"},
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