#include <gtest/gtest.h>
#include <iostream>

#include "opcode.h"
#include "disassm.h"

using std::range_error;
using std::runtime_error;
using std::array;
using std::string;


TEST(Disassm, test_disassm)
{
   
    Disassm disassm;

    size_t offset = 0;
    bin_string inp;
    inp += (uint8_t) 0x04;
    inp += (uint8_t) 0xC2;
    pOpCode ret = disassm.decode(inp, offset);
    EXPECT_TRUE(offset == 2);
    EXPECT_TRUE(ret->mnemonic() == string("add al, 0xc2"));
}
