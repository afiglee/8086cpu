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
        virtual pOpCode get(const bin_string& inp, size_t& offset);

    protected:

    set<FLAG> m_affected_flags;
    //enum FLAVOUR m_flavour;
    string_view m_head;
    string m_mnemonic;
    bin_string m_operands;
};

class OpCode0 : public OpCode 
{
    public:
        OpCode0(const char* head, uint8_t code);
        virtual pOpCode get(const bin_string& inp, size_t& offset) override;
};

class OpCode1 : public OpCode 
{
    public:
        OpCode1(const char* head, uint8_t code);
        virtual pOpCode get(const bin_string& inp, size_t& offset) override;
};

class OpCode1 : public OpCode 
{
    public:
        OpCode1(const char* head, uint8_t code);
        virtual pOpCode get(const bin_string& inp, size_t& offset) override;
};

class OpCode2 : public OpCode 
{
    public:
        OpCode2(const char* head, uint8_t code);
        virtual pOpCode get(const bin_string& inp, size_t& offset) override;
};


class Disassm {

public:
    Disassm(enum FLAVOUR flavour = x8086_88);
    /* If offset is not nullptr offset will point to start instruction
    */
    virtual pOpCode decode(const bin_string& inp, size_t* offset = nullptr);    
protected:
    enum FLAVOUR m_flavour;
    array<OpCode, 1> m_opcodes;

};

#endif
