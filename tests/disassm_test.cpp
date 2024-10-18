#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <exception>

#include "opcode.h"
#include "disassm.h"
#include "mock.h"

using std::range_error;
using std::runtime_error;
using std::array;
using std::string;

using namespace ::testing;
using ::testing::_;

namespace sim86 {
class TestDisassm : public Disassm, public ::testing::Test {
    public:
        pOpCode modregrm(const uint8_t &code, const bstring& inp, size_t& offset) {
            return Disassm::modregrm(code, inp, offset);
        }
};

TEST_F(TestDisassm, test_modregrm_reg) {
    //Disassm disassm;
    for (uint8_t tt = 0; tt < 16; tt++) {
        size_t offset = 1;
        uint8_t top = (tt & 0x08)?0xCC:0;
        const uint8_t op = (tt & 0x07) | top;
        bstring inp{00, op, 0xFA, 0xFC};
        if (tt == 0x06) {
            bstring inp2{00, op, 0xFA};
           // modregrm(00, inp2, offset);
            EXPECT_THROW(modregrm(00, inp2, offset), std::out_of_range);
            offset = 1;
            pOpCode ret = modregrm(00, inp, offset);
            ASSERT_TRUE(offset == 4);            
        } else {
            pOpCode ret = modregrm(00, inp, offset);
            ASSERT_TRUE(offset == 2);
        }
    }
}

TEST_F(TestDisassm, test_modregrm) {
    for (uint8_t tt = 0; tt < 8; tt++) {
        size_t offset = 1;
        uint8_t op = 0x40 | tt;
        bstring inp{0x23, op, 0xFA};
        pOpCode ret = modregrm(00, inp, offset);
        ASSERT_TRUE(offset == 3);
    }
    for (uint8_t tt = 0; tt < 8; tt++) {
        size_t offset = 1;
        uint8_t op = 0x80 | tt;
        bstring inp{0x32, op, 0xFA, 0xFC};
        pOpCode ret = modregrm(00, inp, offset);
        ASSERT_TRUE(offset == 4);
    }
    {
        size_t offset = 1;
        bstring inp{0x21, 0x40};
        EXPECT_THROW(modregrm(0x22, inp, offset), std::out_of_range);
    }
    {
        size_t offset = 1;
        bstring inp{0x21, 0x80, 0xFA};
        EXPECT_THROW(modregrm(0x22, inp, offset), std::out_of_range);
    }

}

TEST(Disassm, test_one_operand)
{
   
    Disassm disassm;
    uint8_t codes[] = {
        0x27,   //DAA
        0x2F,   //DAS
        0x37,   //AAA
        0x3F,   //AAS
        0x90, 0x91, 0x92, 0x93,
        0x94, 0x95, 0x96, 0x97,
        0x98,   //CBW
        0x99,   //CWD
        0x9B,   //WAIT
        0x9C, 0x9D, 0x9E, 0x9F,
        0xA4,   //MOVS
        0xA5,   //MOVS
        0xA6,   //CMPS
        0xA7,   //CMPS
        0xAA,   //STOS
        0xAB,   //STOS
        0xAC,   //LODS
        0xAD,   //LODS
        0xAE,   //SCAS
        0xAF,   //SCAS
        0xC3,   //RET
        0xCB,   //RET
        0xCC,   //INT3
        0xCE,   //INTO   
        0xCF,   //IRET
        0xD7,
        0xEE,
        0xEF,
        0xEC,
        0xED,
        0xF0,   //LOCK
        0xF2,   //REP
        0xF3,   //REP
        0xF4,   //HLT
        0xF5,   //CMC
        0xF8,   //CLC
        0xF9,   //STC
        0xFA,   //CLI
        0xFB,   //STI
        0xFC,   //CLD
        0xFD,   //STD


    };
    for (auto code:codes) {
        size_t offset = 0;
        bstring inp{code};
        pOpCode ret = disassm.decode(inp, offset);
        EXPECT_TRUE(offset == 1);
        EXPECT_TRUE(ret->operands().size() == 1);
    }
    // also 0x40..0x4F - INC/DEC
    for (uint8_t code = 0x40; code < 0x50; code++) {
        size_t offset = 0;
        bstring inp{code};
        pOpCode ret = disassm.decode(inp, offset);
        EXPECT_TRUE(offset == 1);
        EXPECT_TRUE(ret->operands().size() == 1);
    }
}

TEST(CallRegRm, test_call_regrmXXX) 
{
    /*
    0x00..0x03    modXXXr/m ADD  
    0x08..0x0B    modXXXr/m OR
    0x10..0x13    modXXXr/m ADC
    0x18..0x1B    modXXXr/m SBB
    0x20..0x23    modXXXr/m AND
    0x28..0x2B    modXXXr/m SUB
    0x30..0x33    modXXXr/m XOR
    0x38..0x3B    modXXXr/m CMP
    0x84..0x85    modXXXr/m TEST
    0xD8..0xDF    modXXXr/m ESC

    0x86..0x87    modXXXr/m XCHG
    0x88..0x8B    modXXXr/m MOV
    0x8D          modXXXr/m LEA
    0xC4..0xC5    modXXXr/m LES/LDS
*/

    uint8_t codes[] {
        0x00, 0x01, 0x02, 0x03,  
        0x08, 0x09, 0x0A, 0x0B,
        0x10, 0x11, 0x12, 0x13,
        0x18, 0x19, 0x1A, 0x1B,
        0x20, 0x21, 0x22, 0x23,
        0x28, 0x29, 0x2A, 0x2B,
        0x30, 0x31, 0x32, 0x33,
        0x38, 0x39, 0x3A, 0x3B,
        0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8A, 0x8B,
        0x8D, 0xC4, 0xC5,
        0xD8, 0xD9, 0xDA, 0xDB,
        0xDC, 0xDD, 0xDE, 0xDF
    };
    

    MockDisassm m;
    EXPECT_CALL(m, modregrm(_,_,_)).Times(42+9);
    for (auto code:codes) {
        size_t offset = 0;
        bstring inp{code, 0x00};
        m.decode(inp, offset);
    }
    for (auto code:codes) {
        size_t offset = 0;
        bstring inp{code};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }

}


TEST(CallRegRm, test_call_regrm_rotate)
{
    /*
    0xD0..0xD3    mod000r/m ROL
    0xD0..0xD3    mod001r/m ROR
    0xD0..0xD3    mod010r/m RCL
    0xD0..0xD3    mod011r/m RCR
    0xD0..0xD3    mod100r/m SHL/SAL
    0xD0..0xD3    mod101r/m SHR
                  0xD0..0xD3  mod110r/m Exception
    0xD0..0xD3    mod111r/m SAR
    */
       uint8_t codes[] {
        0xD0, 0xD1, 0xD2, 0xD3
    };

    {
        MockDisassm m;
        EXPECT_CALL(m, modregrm(_,_,_)).Times(28);
        for (auto code:codes) {
            for (size_t tt = 0; tt < 8; tt++) {
                if (tt != 6) {
                    size_t offset = 0;
                    uint8_t op = 0x40 | (tt << 3);
                    bstring inp{code, op};
                    m.decode(inp, offset);
                }
            }
        }
    }
    
    {
        for (auto code:codes) {
            MockDisassm m;
            size_t offset = 0;
            bstring inp{code, 0x70};
            EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
        }
    }
    {
        for (auto code:codes) {
            MockDisassm m;
            size_t offset = 0;
            bstring inp{code};
            EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
        }
    }
    

}

TEST(CallRegRm, test_call_regrm80) 
{
    /*
    0x80..0x81    mod000r/m ADD                  
    0x80..0x81    mod001r/m OR
    0x80..0x81    mod010r/m ADC
    0x80..0x81    mod011r/m SBB
                  0x80..0x81    mod100r/m   Exception
    0x80..0x81    mod101r/m SUB
    0x80..0x81    mod110r/m XOR
    0x80..0x81    mod111r/m CMP
    */
    
    {
        MockDisassm m;
        EXPECT_CALL(m, modregrm(_,_,_)).Times(14);
        for (size_t tt = 0; tt < 8; tt++) {
            if (tt != 4) {
                uint8_t code = 0x40 | (tt << 3);
                {
                    size_t offset = 0;
                    bstring inp{0x80, code};
                    m.decode(inp, offset);
                }
                {
                    size_t offset = 0;
                    bstring inp{0x81, code};
                    m.decode(inp, offset);
                }
            }
        }
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0x80, 0x60};
        EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0x81, 0x60};
        EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0x80};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0x81};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }

}

TEST(CallRegRm, test_call_regrm82) 
{
    /*
    0x82..0x83    mod000r/m ADD      
                  0x82..0x83    mod001r/m   Exception
    0x82..0x83    mod010r/m ADC
    0x82..0x83    mod011r/m SBB
                  0x82..0x83    mod100r/m   Exception
    0x82..0x83    mod101r/m SUB
                  0x82..0x83    mod110r/m   Exception
    0x82..0x83    mod111r/m CMP
    */
    
    {
        MockDisassm m;
        EXPECT_CALL(m, modregrm(_,_,_)).Times(10);
        for (size_t tt = 0; tt < 8; tt++) {
            switch (tt) {
                case 0:
                case 2:
                case 3:
                case 5:
                case 7:
                {
                    uint8_t code = 0x40 | (tt << 3);
                    {
                        size_t offset = 0;
                        bstring inp{0x82, code};
                        m.decode(inp, offset);
                    }
                    {
                        size_t offset = 0;
                        bstring inp{0x83, code};
                        m.decode(inp, offset);
                    }
                }
            }
        }
    }
    {
        MockDisassm m;
        for (size_t tt = 0; tt < 8; tt++) {
            switch (tt) {
                case 1:
                case 4:
                case 6:
                {
                    uint8_t code = 0x40 | (tt << 3);
                    {
                        size_t offset = 0;
                        bstring inp{0x82, code};
                        EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
                    }
                    {
                        size_t offset = 0;
                        bstring inp{0x83, code};
                        EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
                    }
                }
            }
        }
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0x82};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0x83};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }

}

TEST(CallRegRm, test_call_regrmF6) 
{
    /*
    0xF6..0xF7    mod000r/m TEST
                  0xF6..0xF7    mod001r/m Exception
    0xF6..0xF7    mod010r/m NOT
    0xF6..0xF7    mod011r/m NEG
    0xF6..0xF7    mod100r/m MUL
    0xF6..0xF7    mod101r/m IMUL
    0xF6..0xF7    mod110r/m DIV
    0xF6..0xF7    mod111r/m IDIV
    */
    
    {
        MockDisassm m;
        EXPECT_CALL(m, modregrm(_,_,_)).Times(14);
        for (size_t tt = 0; tt < 8; tt++) {
            if (tt != 1) {
                uint8_t code = 0x40 | (tt << 3);
                {
                    size_t offset = 0;
                    bstring inp{0xF6, code};
                    m.decode(inp, offset);
                }
                {
                    size_t offset = 0;
                    bstring inp{0xF7, code};
                    m.decode(inp, offset);
                }
            }
        }
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0xF6, 0x08};
        EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0xF7, 0x08};
        EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0xF6};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }
    {
        MockDisassm m;
        size_t offset = 0;
        bstring inp{0xF7};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }
}

TEST(CallRegRm, test_call_regrmFE) 
{
    /*
    0xFE          mod000r/m INC  
    0xFE          mod001r/m DEC
    */
    
    {
        MockDisassm m;
        EXPECT_CALL(m, modregrm(_,_,_)).Times(2);
        {
            size_t offset = 0;
            bstring inp{0xFE, 0x0};
            m.decode(inp, offset);
        }
        {
            size_t offset = 0;
            bstring inp{0xFE, 0x08};
            m.decode(inp, offset);
        }
        {
            size_t offset = 0;
            bstring inp{0xFE};
            EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
        }
    }
    {
        MockDisassm m;
        for (size_t tt = 2; tt < 8; tt++) {
            uint8_t code = 0x40 | (tt << 3);
            {
                size_t offset = 0;
                bstring inp{0xFE, code};
                EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
            }
        }
    }
}

TEST(CallRegRm, test_call_regrmFF) 
{
    /*
    0xFF          mod000r/m INC
    0xFF          mod001r/m DEC
    0xFF          mod010r/m CALL
    0xFF          mod011r/m CALL
    0xFF          mod100r/m JMP
    0xFF          mod101r/m JMP
                  0xFF mod110r/m    Exception
                  0xFF mod111r/m    Exception
    */
    
    {
        MockDisassm m;
        {
            size_t offset = 0;
            bstring inp{0xFF, 0x30};
            EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
        }
        {
            size_t offset = 0;
            bstring inp{0xFF, 0x38};
            EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
        }
        {
            size_t offset = 0;
            bstring inp{0xFF};
            EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
        }
    }
    
    {
        MockDisassm m;
        EXPECT_CALL(m, modregrm(_,_,_)).Times(6);
        for (size_t tt = 0; tt < 6; tt++) {
            uint8_t code = 0x40 | (tt << 3);
            {
                size_t offset = 0;
                bstring inp{0xFF, code};
                m.decode(inp, offset);
            }
        }
    }
    
}

TEST(TwoBytes, testTwoBytes) {
    /*
       case 0xCD:      //int x
        case 0xEB:      //jmp
        case 0x70:      //JO
        case 0x71:      //JNO
        case 0x72:      //JB/JNAE
        case 0x73:      //JNB/JA
        case 0x74:      //JE/JZ
        case 0x75:      //JNE/JNZ
        case 0x76:      //JBE/JNA
        case 0x77:      //JNBE/JA
        case 0x78:      //JS
        case 0x79:      //JNS
        case 0x7A:      //JP/JPE
        case 0x7B:      //JNP/JPO
        case 0x7C:      //JL/JNGE
        case 0x7D:      //JNL/JGE
        case 0x7E:      //JLE/JNG
        case 0x7F:      //JNLE/JG
    */
    
    Disassm m;
    for (uint8_t tt = 0x70; tt < 0x80; tt++) {
            size_t offset = 0;
            bstring inp{tt, 0xFF};
            auto pC = m.decode(inp, offset);
            ASSERT_TRUE(offset == 2);
            ASSERT_TRUE(pC->operands().size() == 2);
    }
    for (uint8_t tt = 0x70; tt < 0x80; tt++) {
            size_t offset = 0;
            bstring inp{tt};
            EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }

    uint8_t codes[] {
        0x04, 0x0C, 0x14, 0x1C,
        0x24, 0x2C, 0x34, 0x3C,
        0xA8, 0xCD, 
        0xE4, 0xE5, 0xE6, 0xE7,
        0xEB};


    /* 2 bytes
    0x04
    0x0C
    0x14
    0x1C
    0x24
    0x2C
    0x34
    0x3C
    0xA8
    0xCD
    0xEB
    */

    
    for (auto code:codes)
    {
        size_t offset = 0;
        bstring inp{code, 0xFF};
        auto pC = m.decode(inp, offset);
        ASSERT_TRUE(offset == 2);
        ASSERT_TRUE(pC->operands().size() == 2);
    }
    for (auto code:codes)
    {
        size_t offset = 0;
        bstring inp{code};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }
    {
        size_t offset = 0;
        bstring inp{0xD4, 0x0A};
        auto pC = m.decode(inp, offset);
        ASSERT_TRUE(offset == 2);
        ASSERT_TRUE(pC->operands().size() == 2);
    }
    {
        size_t offset = 0;
        bstring inp{0xD5, 0x0A};
        auto pC = m.decode(inp, offset);
        ASSERT_TRUE(offset == 2);
        ASSERT_TRUE(pC->operands().size() == 2);
    }
    {
        size_t offset = 0;
        bstring inp{0xD4};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }
    {
        size_t offset = 0;
        bstring inp{0xD5};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }
    {
        size_t offset = 0;
        bstring inp{0xD4, 0x01};
        EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
    }
    {
        size_t offset = 0;
        bstring inp{0xD5, 0x01};
        EXPECT_THROW(m.decode(inp, offset), std::invalid_argument);
    }
}

TEST(ThreeBytes, testThreeBytes) {
    /* 3 bytes
    0x05
    0x0D
    0x15
    0x1D
    0x25
    0x35
    0x2D
    0x3D
    0xA9

    0xC2
    0xCA
    0xE8
    0xE9
    */
    
    Disassm m;

    uint8_t codes[] {
        0x05, 0x0D, 0x15, 0x1D,
        0x25, 0x2D, 0x35, 0x3D,
        0xA9, 0xC2, 0xCA, 0xE8,
        0xE9
    };
    
    for (auto code:codes)
    {
        size_t offset = 0;
        bstring inp{code, 0xFC, 0xFA};
        auto pC = m.decode(inp, offset);
        ASSERT_TRUE(offset == 3);
        ASSERT_TRUE(pC->operands().size() == 3);
    }
    for (auto code:codes)
    {
        size_t offset = 0;
        bstring inp{code, 0xFC};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }
}

TEST(FourBytes, testFourBytes) {
    /* 4 bytes
    0xEA,
    0x9A
    */
    
    Disassm m;

    uint8_t codes[] {
        0xEA, 0x9A
    };
    
    for (auto code:codes)
    {
        size_t offset = 0;
        bstring inp{code, 0xFC, 0xFA, 0xFC, 0xFA};
        auto pC = m.decode(inp, offset);
        ASSERT_TRUE(offset == 5);
        ASSERT_TRUE(pC->operands().size() == 5);
    }
    for (auto code:codes)
    {
        size_t offset = 0;
        bstring inp{code, 0xFC, 0xFC, 0xFA};
        EXPECT_THROW(m.decode(inp, offset), std::out_of_range);
    }
}

}

