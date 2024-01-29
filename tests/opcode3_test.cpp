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

TEST(OpCode3, test_offset_out_of_range)
{
    OpCode3 opCode("xor", 0x33);
 
    size_t offset = 1;
    bin_string inp;
    inp += (uint8_t) 0;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode3, test_truncated_instruction)
{
    OpCode3 opCode("xor", 0x33);
 
    size_t offset = 0;
    bin_string inp;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode3, test_truncated_instruction2)
{
    OpCode3 opCode("xor", 0x33);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0x40;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode3, test_na_opcode)
{
    OpCode3 opCode("xor", 0x33);

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

TEST(OpCode3, test_register2register)
{
    OpCode3 opCode("xor", 0x33);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp.resize(2);

    array<sTest, 15> tests {{
        {0xC0, "xor ax, ax"},
        {0xC1, "xor ax, cx"},
        {0xC2, "xor ax, dx"},
        {0xC3, "xor ax, bx"},
        {0xC4, "xor ax, sp"},
        {0xC5, "xor ax, bp"},
        {0xC6, "xor ax, si"},
        {0xC7, "xor ax, di"},
        {0xC8, "xor cx, ax"},
        {0xD0, "xor dx, ax"},
        {0xD8, "xor bx, ax"},
        {0xE0, "xor sp, ax"},
        {0xE8, "xor bp, ax"},
        {0xF0, "xor si, ax"},
        {0xF8, "xor di, ax"},
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

TEST(OpCode3, test_register2pointer)
{
    OpCode3 opCode("xor", 0x33);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp.resize(2);

    array<sTest, 14> tests {{
        {0x00, "xor ax, word ptr [bx + si]"},
        {0x01, "xor ax, word ptr [bx + di]"},
        {0x02, "xor ax, word ptr [bp + si]"},
        {0x03, "xor ax, word ptr [bp + di]"},
        {0x04, "xor ax, word ptr [si]"},
        {0x05, "xor ax, word ptr [di]"},
        {0x07, "xor ax, word ptr [bx]"},
        {0x08, "xor cx, word ptr [bx + si]"},
        {0x10, "xor dx, word ptr [bx + si]"},
        {0x18, "xor bx, word ptr [bx + si]"},
        {0x20, "xor sp, word ptr [bx + si]"},
        {0x28, "xor bp, word ptr [bx + si]"},
        {0x30, "xor si, word ptr [bx + si]"},
        {0x38, "xor di, word ptr [bx + si]"},
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

TEST(OpCode3, test_register2pointerWithData)
{
    OpCode3 opCode("xor", 0x33);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0; //placeholder, tests will replace it
    inp += (uint8_t) 0x13;
    
    array<sTest, 14> tests {{
        {0x40, "xor ax, word ptr [bx + si + 0x13]"},
        {0x41, "xor ax, word ptr [bx + di + 0x13]"},
        {0x42, "xor ax, word ptr [bp + si + 0x13]"},
        {0x43, "xor ax, word ptr [bp + di + 0x13]"},
        {0x44, "xor ax, word ptr [si + 0x13]"},
        {0x45, "xor ax, word ptr [di + 0x13]"},
        {0x47, "xor ax, word ptr [bx + 0x13]"},
        {0x48, "xor cx, word ptr [bx + si + 0x13]"},
        {0x50, "xor dx, word ptr [bx + si + 0x13]"},
        {0x58, "xor bx, word ptr [bx + si + 0x13]"},
        {0x60, "xor sp, word ptr [bx + si + 0x13]"},
        {0x68, "xor bp, word ptr [bx + si + 0x13]"},
        {0x70, "xor si, word ptr [bx + si + 0x13]"},
        {0x78, "xor di, word ptr [bx + si + 0x13]"},
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