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

#define JXX_MASK4       0x70
#define JO              0x70
#define JNO             0x71      //JNO
#define JB              0x72      //JB/JNAE
#define JAE             0x73      //JNB/JA
#define JE              0x74      //JE/JZ
#define JNE             0x75      //JNE/JNZ
#define JBE             0x76      //JBE/JNA
#define JA              0x77      //JNBE/JA
#define JS              0x78      //JS
#define JNS             0x79      //JNS
#define JP              0x7A      //JP/JPE
#define JNP             0x7B      //JNP/JPO
#define JL              0x7C      //JL/JNGE
#define JGE             0x7D      //JNL/JGE
#define JLE             0x7E      //JLE/JNG
#define JG              0x7F      //JNLE/JG

#define SAHF            0x9E
#define LAHF            0x9F
#define MOV_IRW_MASK3   0xB8 //MOV Immediate data to Word Register, 3 bit mask 0xF8
#define MOV_IRB_MASK3   0xB0 //MOV Immediate data to Byte Register, 3 bit mask 0xF8
#define RET             0xC3
#define RETF            0xCB
#define ROT_MASK2       0xD0
#define LJMP            0xEA
#define CLI             0xFA

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
#if 0
class CPU {
    public:
        CPU(enum FLAVOUR flavour);
       // virtual void apply();//TODO
    protected:
    enum FLAVOUR m_flavour;
};
#endif
class OpCode {
    public:
        //OpCode(uint8_t code, enum DIALECT eDialect = INTEL);
        OpCode(bstring && bcode, bool byte_operands, enum DIALECT eDialect = INTEL);

        static uint32_t calc_address(uint16_t segment, uint16_t offset);
        static std::string get_register8_name(uint8_t reg);
        static std::string get_register16_name(uint8_t reg);
   //     const string& mnemonic() const;
        friend std::ostream& operator<<(std::ostream& os, const OpCode &oCode);
        const bstring &operands() const {return m_operands;}
        virtual std::ostream &print(std::ostream &os) {
            return os << *this;
        }
        
        ssize_t calc_new_address(uint32_t d_start, size_t offset);

    protected:
        std::string decode_mod_rm() const;
  
    bstring m_operands;
    bool m_byte_operands;
    enum DIALECT m_eDialect;
};

// This class is used for relative jumps and takes offset address 
// inside segment
class AddressedOpCode : public OpCode {
public:
    AddressedOpCode(uint16_t own_address, uint8_t code, uint8_t offset, DIALECT eDialect = INTEL):
        OpCode(bstring(code, offset), false, eDialect), m_own_address(own_address) {
            if ((code & JXX_MASK4) != JXX_MASK4) {
                throw std::invalid_argument("Wrong opcode for AddressedOpCode");
            }
    }
    std::ostream &print(std::ostream &os) override {
            return os << *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const AddressedOpCode &oCode);
    protected:
        uint16_t m_own_address;
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
