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

TEST(OpCode0, test_offset_out_of_range)
{
    OpCode0 opCode("add", 0);
 
    size_t offset = 1;
    bin_string inp;
    inp += (uint8_t) 0;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode0, test_truncated_instruction)
{
    OpCode0 opCode("add", 0);
 
    size_t offset = 0;
    bin_string inp;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode0, test_truncated_instruction2)
{
    OpCode0 opCode("add", 0);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0x40;
    EXPECT_THROW(opCode.get(inp, offset), range_error);
}

TEST(OpCode0, test_na_opcode)
{
    OpCode0 opCode("add", 0);

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

TEST(OpCode0, test_register2register)
{
    OpCode0 opCode("add", 0);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp.resize(2);

    array<sTest, 15> tests {{
        {0xC0, "add al, al"},
        {0xC1, "add cl, al"},
        {0xC2, "add dl, al"},
        {0xC3, "add bl, al"},
        {0xC4, "add ah, al"},
        {0xC5, "add ch, al"},
        {0xC6, "add dh, al"},
        {0xC7, "add bh, al"},
        {0xC8, "add al, cl"},
        {0xD0, "add al, dl"},
        {0xD8, "add al, bl"},
        {0xE0, "add al, ah"},
        {0xE8, "add al, ch"},
        {0xF0, "add al, dh"},
        {0xF8, "add al, bh"},
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

TEST(OpCode0, test_register2pointer)
{
    OpCode0 opCode("add", 0);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp.resize(2);

    array<sTest, 14> tests {{
        {0x00, "add byte ptr [bx + si], al"},
        {0x01, "add byte ptr [bx + di], al"},
        {0x02, "add byte ptr [bp + si], al"},
        {0x03, "add byte ptr [bp + di], al"},
        {0x04, "add byte ptr [si], al"},
        {0x05, "add byte ptr [di], al"},
        {0x07, "add byte ptr [bx], al"},
        {0x08, "add byte ptr [bx + si], cl"},
        {0x10, "add byte ptr [bx + si], dl"},
        {0x18, "add byte ptr [bx + si], bl"},
        {0x20, "add byte ptr [bx + si], ah"},
        {0x28, "add byte ptr [bx + si], ch"},
        {0x30, "add byte ptr [bx + si], dh"},
        {0x38, "add byte ptr [bx + si], bh"},
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

TEST(OpCode0, test_register2pointerWithData)
{
    OpCode0 opCode("add", 0);

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0;
    inp += (uint8_t) 0; //placeholder, tests will replace it
    inp += (uint8_t) 0x13;
    
    array<sTest, 14> tests {{
        {0x40, "add byte ptr [bx + si + 0x13], al"},
        {0x41, "add byte ptr [bx + di + 0x13], al"},
        {0x42, "add byte ptr [bp + si + 0x13], al"},
        {0x43, "add byte ptr [bp + di + 0x13], al"},
        {0x44, "add byte ptr [si + 0x13], al"},
        {0x45, "add byte ptr [di + 0x13], al"},
        {0x47, "add byte ptr [bx + 0x13], al"},
        {0x48, "add byte ptr [bx + si + 0x13], cl"},
        {0x50, "add byte ptr [bx + si + 0x13], dl"},
        {0x58, "add byte ptr [bx + si + 0x13], bl"},
        {0x60, "add byte ptr [bx + si + 0x13], ah"},
        {0x68, "add byte ptr [bx + si + 0x13], ch"},
        {0x70, "add byte ptr [bx + si + 0x13], dh"},
        {0x78, "add byte ptr [bx + si + 0x13], bh"},
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