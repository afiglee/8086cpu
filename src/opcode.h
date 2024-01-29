#ifndef __OPCODE_H__
#define __OPCODE_H__

#include <set>
#include <string>
#include <array>
#include <string_view>

using std::set;
using std::string;
using std::array;
using std::string_view;

class OpCode;

typedef std::basic_string<uint8_t>  bin_string;
typedef std::shared_ptr<OpCode>    pOpCode;

class Heap {

};

enum FLAVOUR {
    x8086_88,
    x286,
    x386,
    x486
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
        virtual void apply();//TODO
    protected:
    enum FLAVOUR m_flavour;
};

class OpCode {
    public:
        OpCode(const char* head, uint8_t code);

        //Creates opcode and returns shared pointer to it
        //May through exception
        pOpCode get(const bin_string& inp, size_t& offset);

        const string& mnemonic() const;
        friend std::ostream& operator<<(std::ostream& os, const OpCode oCode);
    protected:
        virtual pOpCode _get(const bin_string& inp, size_t& offset);

    string_view m_head;
    string m_mnemonic;
    bin_string m_operands;
};

class OpCodeTwo : public OpCode {
    public:
        OpCodeTwo(const char *head, uint8_t code);

    protected:
        virtual pOpCode _get(const bin_string& inp, size_t& offset) override; 
        virtual void readOperand();  
};

class OpCodeRelJump: public OpCodeTwo {
    public:
        OpCodeRelJump(const char *head, uint8_t code);

    protected:
        virtual void readOperand() override;  
};

class OpCodeThree : public OpCode {
    public:
        OpCodeThree(const char *head, uint8_t code);

    protected:
        virtual pOpCode _get(const bin_string& inp, size_t& offset) override;   
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

#endif
