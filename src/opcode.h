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

#define LOOPNZ          0xE0
#define LOOPZ           0xE1
#define LOOP            0xE2
#define JCXZ            0xE3

#define SAHF            0x9E
#define LAHF            0x9F
#define MOV_IRW_MASK3   0xB8 //MOV Immediate data to Word Register, 3 bit mask 0xF8
#define MOV_IRB_MASK3   0xB0 //MOV Immediate data to Byte Register, 3 bit mask 0xF8
#define RET             0xC3
#define RETF            0xCB
#define ROT_MASK2       0xD0
#define LJMP            0xEA
#define CLI             0xFA

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

#define ADD_IA_MASK 0x04
#define ADC_IA_MASK 0x14
#define SUB_IA_MASK 0x2C
#define SSB_IA_MASK 0x1C
#define CMP_IA_MASK 0x3C
#define AND_IA_MASK 0x24
#define TEST_IA_MASK    0xA8 //no w
#define OR_IA_MASK  0x0C
#define XOR_IA_MASK 0x34

#define MOV_R2SEG   0x8E
#define MOV_SEG2R   0x8C

#define MOV_2R_MASK 0x88

#define IN_MASK     0xE4
#define OUT_MASK    0xE6
#define JMP_SEG     0xE9
#define IN_MASK_REG 0xEC
#define OUT_MASK_REG 0xEE

#define INC_MASK    0xFE

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

enum PREFIX {
    PREFIX_DEFAULT,
    PREFIX_ES,
    PREFIX_CS,
    PREFIX_SS,
    PREFIX_DS
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
        OpCode(bstring && bcode, bool byte_operands, enum PREFIX ePrefix = PREFIX_DEFAULT, DIALECT eDialect = INTEL);

        static uint32_t calc_address(uint16_t segment, uint16_t offset);
        static std::string get_register8_name(uint8_t reg);
        static std::string get_register16_name(uint8_t reg);
        static std::string get_segregister_name(uint8_t reg);
        static uint16_t calc_disp(uint16_t addr, int8_t disp);
   //     const string& mnemonic() const;
        friend std::ostream& operator<<(std::ostream& os, const OpCode &oCode);
        const bstring &operands() const {return m_operands;}
        std::string decode_dw_mod_rm() const;
        virtual std::ostream &print(std::ostream &os) {
            return os << *this;
        }
        
        virtual ssize_t calc_new_address(uint32_t d_start, size_t offset) const;
        virtual bool isPrefix() const { return false;}
        virtual enum PREFIX prefix() const {return PREFIX_DEFAULT;}
    protected:
        std::string decode_register_name() const;
        std::string decode_mod_rm() const;
  
    bstring m_operands;
    bool m_byte_operands;
    enum PREFIX m_ePrefix;
    enum DIALECT m_eDialect;


};

class PrefixOpCode final : public OpCode {
    public:
        PrefixOpCode(uint8_t prefix): OpCode(bstring{prefix}, false) 
        {}
        bool isPrefix() const override {return true;}
        enum PREFIX prefix() const override {
            switch (m_operands[0]) {
                case 0x26:
                    return PREFIX_ES;
                case 0x2E:
                    return PREFIX_CS;
                case 0x36:
                    return PREFIX_SS;
                case 0x3E:
                    return PREFIX_DS;
                default:
                    return PREFIX_DEFAULT;
            }
        } 
};

// This class is used for relative jumps and takes offset address 
// inside segment
class AddressedOpCode : public OpCode {
public:
    AddressedOpCode(uint16_t own_address, uint8_t code, uint8_t offset, uint8_t offset_high, 
                    enum PREFIX ePrefix = PREFIX_DEFAULT, DIALECT eDialect = INTEL):
        OpCode(bstring{code, offset, offset_high}, false, ePrefix, eDialect), m_own_address(own_address) {
            if ((code & 0xE8) != 0xE8) {
                throw std::invalid_argument("Wrong opcode for AddressedOpCode");
            }
    }
    AddressedOpCode(uint16_t own_address, uint8_t code, uint8_t offset, 
                    enum PREFIX ePrefix = PREFIX_DEFAULT,DIALECT eDialect = INTEL):
        OpCode(bstring(code, offset), false, ePrefix, eDialect), m_own_address(own_address) {
            if ((code & JXX_MASK4) != JXX_MASK4 && (code & 0xFC) != 0xE0) {
                throw std::invalid_argument("Wrong opcode for AddressedOpCode");
            }
    }
    std::ostream &print(std::ostream &os) override {
            return os << *this;
    }
    ssize_t calc_new_address(uint32_t d_start, size_t offset) const override {
        uint16_t go_to = m_own_address + (int16_t) TO_UINT16(m_operands[1], m_operands[2]) + 3;
        //go_to += offset;
        return go_to;
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
