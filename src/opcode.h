#ifndef __OPCODE_H__
#define __OPCODE_H__

#include <set>
#include <string>
#include <array>
#include <string_view>
#include <memory>

#include "basetypes.h"

using std::set;
using std::string;
using std::array;
using std::string_view;

// 1 byte instructions:

#define BAA     0x27
#define DAS     0x2F
#define AAA     0x37
#define AAS     0x3F
#define INC_AX  0x40
#define INC_CX  0x41
#define INC_DX  0x42
#define INC_BX  0x43
#define INC_SP  0x44
#define INC_BP  0x45
#define INC_SI  0x46
#define INC_DI  0x47
#define DEC_AX  0x48
#define DEC_CX  0x49
#define DEC_DX  0x4A
#define DEC_BX  0x4B
#define DEC_SP  0x4C
#define DEC_BP  0x4D
#define DEC_SI  0x4E
#define DEC_DI  0x4F

#define CBW     0x98
#define CWD     0x99
#define WAIT    0x9B
#define MOVS    0xA4
#define MOVSW   0xA5
#define CMPS    0xA6
#define CMPSW   0xA7

#define STOS    0xAA
#define STOSW   0xAB
#define LODS    0xAC
#define LODSW   0xAD
#define SCAS    0xAE
#define SCASW   0xAF

#define RET     0xC3
#define RETL    0xCB

#define INT     0xCC    //Type 3
#define INTO    0xCE
#define IRET    0xCF

#define LOCK    0xF0
#define REP     0xF2
#define REPZ    0xF3


#define HLT     0xF4
#define CMC     0xF5

#define CLC     0xF8
#define STC     0xF9
#define CLI     0xFA
#define STI     0xFB
#define CLD     0xFC
#define STD     0xFD

// 2 bytes instruction


//Complex size
#define ADD_DW_MASK 0x00
#define ADC_DW_MASK 0x10
#define SUB_DW_MASK 0x28
#define SSB_DW_MASK 0x18
#define CMP_DW_MASK 0x38
#define AND_DW_MASK 0x20
#define TEST_DW_MASK    0x84 //no w
#define OR_DW_MASK  0x08
#define XOR_DW_MASK 0x30




class OpCode;
typedef std::shared_ptr<OpCode>    pOpCode;

class Heap {

};

enum FLAVOUR {
    x8086_88,
    x286,
    x386,
    x486
};

enum DIALECT {
    INTEL;
}

template<typename T>
class Register{

protected:
    T data;
};

enum FLAG {
    FLAG_CF, //Carry
    FLAG_PF, //Parity
    FLAG_AX, //Aux
    FLAG_ZF, //Zero
    FLAG_SF, //Sign
    FLAG_OF, //Overflow
    FLAG_IF, //Interrupt enable
    FLAG_DF, //Direction
    FLAG_TF  //Trap
};

class CPU {
    public:
        CPU(enum FLAVOUR flavour);
       // virtual void apply();//TODO
    protected:
    enum FLAVOUR m_flavour;
};

class OpCode {
    public:
        //OpCode(uint8_t code, enum DIALECT eDialect = INTEL);
        Opcode(bstring && bcode, enum DIALECT eDialect = INTEL);

   //     const string& mnemonic() const;
        friend std::ostream& operator<<(std::ostream& os, const OpCode &oCode);
    protected:
  //      virtual pOpCode _get(const bin_string& inp, size_t& offset);

//    string_view m_head;
//    string m_mnemonic;
    bstring m_operands;
    enum DIALECT m_eDialect;
};

class OpCodeNA : public OpCode {
    public:    
        OpCodeNA(uint8_t code, enum DIALECT eDialect);

};

class OpCodeTwo : public OpCode {
    public:
 //       OpCodeTwo(uint8_t code, uint8_t code2);
        OpcodeTwo(bstring && bcode, enum DIALECT eDialect = INTEL);
};
/*
class OpCodeRelJump: public OpCodeTwo {
    public:
        OpCodeRelJump(const char *head, uint8_t code, uint8_t code2);

    protected:
        virtual void readOperand([[maybe_unused]] size_t offset) override;  
};
*/

class OpCodeThree : public OpCode {
    public:
        OpCodeThree(bstring && bcode, enum DIALECT eDialect = INTEL);
         
};
/*
class AriphmeticOpCode: public OpCode {
    public:
        AriphmeticOpCode(const char* head, uint8_t code);

    protected:
        void readOperands(const bin_string& inp, size_t& offset);
};*/

class OpCode0 : public AriphmeticOpCode 
{
    public:
        OpCode0(const char* head, uint8_t code);
    protected:
        virtual pOpCode _get(const bin_string& inp, size_t& offset) override;
};

class OpCode1 : public AriphmeticOpCode 
{
    public:
        OpCode1(const char* head, uint8_t code);
    protected:
        virtual pOpCode _get(const bin_string& inp, size_t& offset) override;

};

class OpCode2 : public AriphmeticOpCode 
{
    public:
        OpCode2(const char* head, uint8_t code);
    protected:
        virtual pOpCode _get(const bin_string& inp, size_t& offset) override;

};

class OpCode3 : public AriphmeticOpCode 
{
    public:
        OpCode3(const char* head, uint8_t code);
    protected:
        virtual pOpCode _get(const bin_string& inp, size_t& offset) override;

};

class OpCodeMulti :public OpCode {
    public:
        OpCodeMulti(uint8_t code);
    protected:
        virtual pOpCode _get(const bin_string& inp, size_t& offset) override;
};

#endif
