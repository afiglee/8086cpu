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

namespace sim86 {


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
    INTEL
};

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
        OpCode(bstring && bcode, enum DIALECT eDialect = INTEL);

   //     const string& mnemonic() const;
        friend std::ostream& operator<<(std::ostream& os, const OpCode &oCode){
            return os;
        }
        const bstring &operands() const {return m_operands;}
    protected:
  //      virtual pOpCode _get(const bin_string& inp, size_t& offset);

//    string_view m_head;
//    string m_mnemonic;
    bstring m_operands;
    enum DIALECT m_eDialect;
};

#if 0
class OpCodeNA : public OpCode {
    public:    
        OpCodeNA(uint8_t code, enum DIALECT eDialect = INTEL);
        OpCodeNA(bstring &&bcode, enum DIALECT eDialect = INTEL);

};

class OpCodeTwo : public OpCode {
    public:
 //       OpCodeTwo(uint8_t code, uint8_t code2);
        OpcodeTwo(bstring && bcode, enum DIALECT eDialect = INTEL);
};

class OpCodeRelJump: public OpCodeTwo {
    public:
        OpCodeRelJump(const char *head, uint8_t code, uint8_t code2);

    protected:
        virtual void readOperand([[maybe_unused]] size_t offset) override;  
};


class OpCodeThree : public OpCode {
    public:
        OpCodeThree(bstring && bcode, enum DIALECT eDialect = INTEL);
         
};

class AriphmeticOpCode: public OpCode {
    public:
        AriphmeticOpCode(const char* head, uint8_t code);

    protected:
        void readOperands(const bin_string& inp, size_t& offset);
};

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

}

#endif
