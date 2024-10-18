#include "disassm.h"
#include <stdexcept>
#include <memory>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <exception>
#include <iomanip>

namespace sim86{
using std::out_of_range;
using std::invalid_argument;
//using std::make_shared;
using std::stringstream;
using std::shared_ptr;
using std::to_string;
using std::stringstream;
using std::ostream;
using std::hex;
using std::setw;


#define THROW_INVALID(offset, code, code2) {\
    stringstream ss;\
    ss << "Invalid code " << code << " " << code2 << " at " << offset;\
    throw invalid_argument(ss.str());\
}

ostream &operator<<(ostream& os, const uint8_t &u) {
    os << "0x" << setw(2) << hex << (int) u;
    return os;
}

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

#define ADD_IA_MASK 0x04
#define ADC_IA_MASK 0x14
#define SUB_IA_MASK 0x2C
#define SSB_IA_MASK 0x1C
#define CMP_IA_MASK 0x3C
#define AND_IA_MASK 0x24
#define TEST_IA_MASK    0xA8 //no w
#define OR_IA_MASK  0x0C
#define XOR_IA_MASK 0x34



Disassm::Disassm(enum FLAVOUR flavour) :
m_flavour(flavour)/*,
m_opcode_lengths*/{

}
/*
m_opcodes{
    shared_ptr<OpCode>(new OpCode0{"add", //0x00}),
    shared_ptr<OpCode>(new OpCode0{"add", //0x01}),
    shared_ptr<OpCode>(new OpCode0{"add", //0x02}),
    shared_ptr<OpCode>(new OpCode0{"add", //0x03}),
    shared_ptr<OpCode>(new OpCodeTwo{"add al", //0x04}),
    shared_ptr<OpCode>(new OpCodeThree{"add ax", //0x05}),
    shared_ptr<OpCode>(new OpCode{"push es", //0x06}),
    shared_ptr<OpCode>(new OpCode{"pop es", //0x07}),
    shared_ptr<OpCode>(new OpCode0{"or", //0x08}),
    shared_ptr<OpCode>(new OpCode0{"or", //0x09}),
    shared_ptr<OpCode>(new OpCode0{"or", //0x0a}),
    shared_ptr<OpCode>(new OpCode0{"or", //0x0b}),
    shared_ptr<OpCode>(new OpCodeTwo{"or al", //0x0c}),
    shared_ptr<OpCode>(new OpCodeThree{"or ax", //0x0d}),
    shared_ptr<OpCode>(new OpCode{"push cs", //0x0e}),
    shared_ptr<OpCode>(new OpCode{"pop cs", //0x0f}),

    shared_ptr<OpCode>(new OpCode0{"adc", //0x10}),
    shared_ptr<OpCode>(new OpCode0{"adc", //0x11}),
    shared_ptr<OpCode>(new OpCode0{"adc", //0x12}),
    shared_ptr<OpCode>(new OpCode0{"adc", //0x13}),
    shared_ptr<OpCode>(new OpCodeTwo{"adc al", //0x14}),
    shared_ptr<OpCode>(new OpCodeThree{"adc ax", //0x15}),
    shared_ptr<OpCode>(new OpCode{"push ss", //0x16}),
    shared_ptr<OpCode>(new OpCode{"pop ss", //0x17}),
    shared_ptr<OpCode>(new OpCode0{"sbb", //0x18}),
    shared_ptr<OpCode>(new OpCode0{"sbb", //0x19}),
    shared_ptr<OpCode>(new OpCode0{"sbb", //0x1a}),
    shared_ptr<OpCode>(new OpCode0{"sbb", //0x1b}),
    shared_ptr<OpCode>(new OpCodeTwo{"sbb al", //0x1c}),
    shared_ptr<OpCode>(new OpCodeThree{"sbb ax", //0x1d}),
    shared_ptr<OpCode>(new OpCode{"push ds", //0x1e}),
    shared_ptr<OpCode>(new OpCode{"pop ds", //0x1f}),

    ////0x20 - //0x3F
    shared_ptr<OpCode>(new OpCode0{"and", //0x20}),
    shared_ptr<OpCode>(new OpCode0{"and", //0x21}),
    shared_ptr<OpCode>(new OpCode0{"and", //0x22}),
    shared_ptr<OpCode>(new OpCode0{"and", //0x23}),
    shared_ptr<OpCode>(new OpCodeTwo{"and al", //0x24}),
    shared_ptr<OpCode>(new OpCodeThree{"and ax", //0x25}),
    shared_ptr<OpCode>(new OpCode{"es:", //0x26}),
    shared_ptr<OpCode>(new OpCode{"daa", //0x27}),
    shared_ptr<OpCode>(new OpCode0{"sub", //0x28}),
    shared_ptr<OpCode>(new OpCode0{"sub", //0x29}),
    shared_ptr<OpCode>(new OpCode0{"sub", //0x2a}),
    shared_ptr<OpCode>(new OpCode0{"sub", //0x2b}),
    shared_ptr<OpCode>(new OpCodeTwo{"sub al", //0x2c}),
    shared_ptr<OpCode>(new OpCodeThree{"sub ax", //0x2d}),
    shared_ptr<OpCode>(new OpCode{"cs:", //0x2e}),
    shared_ptr<OpCode>(new OpCode{"das", //0x2f}),

    shared_ptr<OpCode>(new OpCode0{"xor", //0x30}),
    shared_ptr<OpCode>(new OpCode0{"xor", //0x31}),
    shared_ptr<OpCode>(new OpCode0{"xor", //0x32}),
    shared_ptr<OpCode>(new OpCode0{"xor", //0x33}),
    shared_ptr<OpCode>(new OpCodeTwo{"xor al", //0x34}),
    shared_ptr<OpCode>(new OpCodeThree{"xor ax", //0x35}),
    shared_ptr<OpCode>(new OpCode{"ss:", //0x36}),
    shared_ptr<OpCode>(new OpCode{"aaa", //0x37}),
    shared_ptr<OpCode>(new OpCode0{"cmp", //0x38}),
    shared_ptr<OpCode>(new OpCode0{"cmp", //0x39}),
    shared_ptr<OpCode>(new OpCode0{"cmp", //0x3a}),
    shared_ptr<OpCode>(new OpCode0{"cmp", //0x3b}),
    shared_ptr<OpCode>(new OpCodeTwo{"cmp al", //0x3c}),
    shared_ptr<OpCode>(new OpCodeThree{"cmp ax", //0x3d}),
    shared_ptr<OpCode>(new OpCode{"push ds:", //0x3e}),
    shared_ptr<OpCode>(new OpCode{"pop aas", //0x3f}),

    ////0x40 - //0x5F
    shared_ptr<OpCode>(new OpCode{"inc ax", //0x40}),
    shared_ptr<OpCode>(new OpCode{"inc cx", //0x41}),
    shared_ptr<OpCode>(new OpCode{"inc dx", //0x42}),
    shared_ptr<OpCode>(new OpCode{"inc bx", //0x43}),
    shared_ptr<OpCode>(new OpCode{"inc sp", //0x44}),
    shared_ptr<OpCode>(new OpCode{"inc bp", //0x45}),
    shared_ptr<OpCode>(new OpCode{"inc si", //0x46}),
    shared_ptr<OpCode>(new OpCode{"inc di", //0x47}),
    shared_ptr<OpCode>(new OpCode{"dec ax", //0x48}),
    shared_ptr<OpCode>(new OpCode{"dec cx", //0x49}),
    shared_ptr<OpCode>(new OpCode{"dec dx", //0x4a}),
    shared_ptr<OpCode>(new OpCode{"dec bx", //0x4b}),
    shared_ptr<OpCode>(new OpCode{"dec sp", //0x4c}),
    shared_ptr<OpCode>(new OpCode{"dec bp", //0x4d}),
    shared_ptr<OpCode>(new OpCode{"dec si", //0x4e}),
    shared_ptr<OpCode>(new OpCode{"dec di", //0x4f}),

    shared_ptr<OpCode>(new OpCode{"push ax", //0x50}),
    shared_ptr<OpCode>(new OpCode{"push cx", //0x51}),
    shared_ptr<OpCode>(new OpCode{"push dx", //0x52}),
    shared_ptr<OpCode>(new OpCode{"push bx", //0x53}),
    shared_ptr<OpCode>(new OpCode{"push sp", //0x54}),
    shared_ptr<OpCode>(new OpCode{"push bp", //0x55}),
    shared_ptr<OpCode>(new OpCode{"push si", //0x56}),
    shared_ptr<OpCode>(new OpCode{"push di", //0x57}),
    shared_ptr<OpCode>(new OpCode{"pop ax", //0x58}),
    shared_ptr<OpCode>(new OpCode{"pop cx", //0x59}),
    shared_ptr<OpCode>(new OpCode{"pop dx", //0x5a}),
    shared_ptr<OpCode>(new OpCode{"pop bx", //0x5b}),
    shared_ptr<OpCode>(new OpCode{"pop sp", //0x5c}),
    shared_ptr<OpCode>(new OpCode{"pop bp", //0x5d}),
    shared_ptr<OpCode>(new OpCode{"pop si", //0x5e}),
    shared_ptr<OpCode>(new OpCode{"pop di", //0x5f}),

    ////0x60 - //0x7F
    shared_ptr<OpCode>(new OpCode{"--", //0x60}),
    shared_ptr<OpCode>(new OpCode{"--", //0x61}),
    shared_ptr<OpCode>(new OpCode{"--", //0x62}),
    shared_ptr<OpCode>(new OpCode{"--", //0x63}),
    shared_ptr<OpCode>(new OpCode{"--", //0x64}),
    shared_ptr<OpCode>(new OpCode{"--", //0x65}),
    shared_ptr<OpCode>(new OpCode{"--", //0x66}),
    shared_ptr<OpCode>(new OpCode{"--", //0x67}),
    shared_ptr<OpCode>(new OpCode{"--", //0x68}),
    shared_ptr<OpCode>(new OpCode{"--", //0x69}),
    shared_ptr<OpCode>(new OpCode{"--", //0x6a}),
    shared_ptr<OpCode>(new OpCode{"--", //0x6b}),
    shared_ptr<OpCode>(new OpCode{"--", //0x6c}),
    shared_ptr<OpCode>(new OpCode{"--", //0x6d}),
    shared_ptr<OpCode>(new OpCode{"--", //0x6e}),
    shared_ptr<OpCode>(new OpCode{"--", //0x6f}),

    shared_ptr<OpCode>(new OpCodeRelJump{"jo", //0x70}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jno", //0x71}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jb", //0x72}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jnb", //0x73}),
    shared_ptr<OpCode>(new OpCodeRelJump{"je", //0x74}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jne", //0x75}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jbe", //0x76}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jnbe", //0x77}),
    shared_ptr<OpCode>(new OpCodeRelJump{"js", //0x78}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jns", //0x79}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jp", //0x7a}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jnp", //0x7b}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jl", //0x7c}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jnl", //0x7d}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jle", //0x7e}),
    shared_ptr<OpCode>(new OpCodeRelJump{"jnle", //0x7f}),

    ////0x80 - //0x9F
    shared_ptr<OpCode>(new OpCode{"TODO", //0x80}),
    shared_ptr<OpCode>(new OpCode{"TODO", //0x81}),
    shared_ptr<OpCode>(new OpCode{"TODO", //0x82}),
    shared_ptr<OpCode>(new OpCode{"TODO", //0x83}),
    shared_ptr<OpCode>(new OpCode0{"test", //0x84}),
    shared_ptr<OpCode>(new OpCode1{"test", //0x85}),
    shared_ptr<OpCode>(new OpCode2{"xchg", //0x86}),
    shared_ptr<OpCode>(new OpCode3{"xchg", //0x87}),
    shared_ptr<OpCode>(new OpCode0{"mov", //0x88}),
    shared_ptr<OpCode>(new OpCode1{"mov", //0x89}),
    shared_ptr<OpCode>(new OpCode2{"mov", //0x8a}),
    shared_ptr<OpCode>(new OpCode3{"mov", //0x8b}),
    shared_ptr<OpCode>(new OpCode1{"_mov", //0x8c}),
    shared_ptr<OpCode>(new OpCode3{"lea", //0x8d}),
    shared_ptr<OpCode>(new OpCode3{"_mov", //0x8e}),
    shared_ptr<OpCode>(new OpCode3{"_pop", //0x8f}),

    shared_ptr<OpCode>(new OpCode{"nop", //0x90}),
    shared_ptr<OpCode>(new OpCode{"xchg ax, cx", //0x91}),
    shared_ptr<OpCode>(new OpCode{"xchg ax, dx", //0x92}),
    shared_ptr<OpCode>(new OpCode{"xchg ax, bx", //0x93}),
    shared_ptr<OpCode>(new OpCode{"xchg ax, sp", //0x94}),
    shared_ptr<OpCode>(new OpCode{"xchg ax, bp", //0x95}),
    shared_ptr<OpCode>(new OpCode{"xchg ax, si", //0x96}),
    shared_ptr<OpCode>(new OpCode{"xchg ax, di", //0x97}),
    shared_ptr<OpCode>(new OpCode{"cbw", //0x98}),
    shared_ptr<OpCode>(new OpCode{"cwd", //0x99}),
    shared_ptr<OpCode>(new OpCode{"TODO", //0x9a}),
    shared_ptr<OpCode>(new OpCode{"wait", //0x9b}),
    shared_ptr<OpCode>(new OpCode{"pushf", //0x9c}),
    shared_ptr<OpCode>(new OpCode{"popf", //0x9d}),
    shared_ptr<OpCode>(new OpCode{"sahf", //0x9e}),
    shared_ptr<OpCode>(new OpCode{"lahf", //0x9f}),

    ////0xA0 - //0xBF
    shared_ptr<OpCode>(new OpCodeThree{"mov al", //0xa0}),
    shared_ptr<OpCode>(new OpCodeThree{"mov ax", //0xa1}),
    shared_ptr<OpCode>(new OpCodeThree{"mov", //0xa2}),
    shared_ptr<OpCode>(new OpCodeThree{"mov", //0xa3}),
    shared_ptr<OpCode>(new OpCode{"movsb", //0xa4}),
    shared_ptr<OpCode>(new OpCode{"movsw", //0xa5}),
    shared_ptr<OpCode>(new OpCode{"cmpsb", //0xa6}),
    shared_ptr<OpCode>(new OpCode{"cmpsw", //0xa7}),
    shared_ptr<OpCode>(new OpCodeTwo{"test al", //0xa8}),
    shared_ptr<OpCode>(new OpCodeThree{"test ax", //0xa9}),
    shared_ptr<OpCode>(new OpCode{"stosb", //0xaa}),
    shared_ptr<OpCode>(new OpCode{"stosw", //0xab}),
    shared_ptr<OpCode>(new OpCode{"lodsb", //0xac}),
    shared_ptr<OpCode>(new OpCode{"lodsw", //0xad}),
    shared_ptr<OpCode>(new OpCode{"scasb", //0xae}),
    shared_ptr<OpCode>(new OpCode{"scasw", //0xaf}),

    shared_ptr<OpCode>(new OpCodeTwo{"mov al", //0xb0}),
    shared_ptr<OpCode>(new OpCodeTwo{"mov cl", //0xb1}),
    shared_ptr<OpCode>(new OpCodeTwo{"mov dl", //0xb2}),
    shared_ptr<OpCode>(new OpCodeTwo{"mov bl", //0xb3}),
    shared_ptr<OpCode>(new OpCodeTwo{"mov ah", //0xb4}),
    shared_ptr<OpCode>(new OpCodeTwo{"mov ch", //0xb5}),
    shared_ptr<OpCode>(new OpCodeTwo{"mov dh", //0xb6}),
    shared_ptr<OpCode>(new OpCodeTwo{"mov bh", //0xb7}),
    shared_ptr<OpCode>(new OpCodeThree{"mov ax", //0xb8}),
    shared_ptr<OpCode>(new OpCodeThree{"mov cx", //0xb9}),
    shared_ptr<OpCode>(new OpCodeThree{"mov dx", //0xba}),
    shared_ptr<OpCode>(new OpCodeThree{"mov bx", //0xbb}),
    shared_ptr<OpCode>(new OpCodeThree{"mov sp", //0xbc}),
    shared_ptr<OpCode>(new OpCodeThree{"mov bp", //0xbd}),
    shared_ptr<OpCode>(new OpCodeThree{"mov si", //0xbe}),
    shared_ptr<OpCode>(new OpCodeThree{"mov di", //0xbf}),

    ////0xC0 - //0xDF
    shared_ptr<OpCode>(new OpCode{"n/a", //0xc0}),
    shared_ptr<OpCode>(new OpCode{"n/a", //0xc1}),
    shared_ptr<OpCode>(new OpCodeThree{"ret d16", //0xc2}),
    shared_ptr<OpCode>(new OpCode{"ret", //0xc3}),
    shared_ptr<OpCode>(new OpCode{"les", //0xc4}),
    shared_ptr<OpCode>(new OpCode{"lds", //0xc5}),
    shared_ptr<OpCode>(new OpCode{"_mov", //0xc6}),
    shared_ptr<OpCode>(new OpCode{"_mov", //0xc7}),
    shared_ptr<OpCode>(new OpCode{"n/a", //0xc8}),
    shared_ptr<OpCode>(new OpCode{"n/a", //0xc9}),
    shared_ptr<OpCode>(new OpCodeThree{"retf d16", //0xca}),
    shared_ptr<OpCode>(new OpCode{"retf", //0xcb}),
    shared_ptr<OpCode>(new OpCode{"int 3", //0xcc}),
    shared_ptr<OpCode>(new OpCodeTwo{"int d8", //0xcd}),
    shared_ptr<OpCode>(new OpCode{"into", //0xce}),
    shared_ptr<OpCode>(new OpCode{"iret", //0xcf}),

    shared_ptr<OpCode>(new OpCodeTwo{"_rot", //0xd0}),
    shared_ptr<OpCode>(new OpCodeTwo{"_rot", //0xd1}),
    shared_ptr<OpCode>(new OpCodeTwo{"_rot", //0xd2}),
    shared_ptr<OpCode>(new OpCodeTwo{"_rot", //0xd3}),
    shared_ptr<OpCode>(new OpCodeTwo{"aam", //0xd4}),
    shared_ptr<OpCode>(new OpCodeTwo{"aad", //0xd5}),
    shared_ptr<OpCode>(new OpCode{"salc", //0xd6}),
    shared_ptr<OpCode>(new OpCode{"xlat", //0xd7}),
    shared_ptr<OpCode>(new OpCodeTwo{"esc 0", //0xd8}),
    shared_ptr<OpCode>(new OpCodeTwo{"esc 1", //0xd9}),
    shared_ptr<OpCode>(new OpCodeTwo{"esc 2", //0xda}),
    shared_ptr<OpCode>(new OpCodeTwo{"esc 3", //0xdb}),
    shared_ptr<OpCode>(new OpCodeTwo{"esc 4", //0xdc}),
    shared_ptr<OpCode>(new OpCodeTwo{"esc 5", //0xdd}),
    shared_ptr<OpCode>(new OpCodeTwo{"esc 6", //0xde}),
    shared_ptr<OpCode>(new OpCodeTwo{"esc 7", //0xdf}),

    ////0xE0 - //0xFF
    shared_ptr<OpCode>(new OpCodeTwo{"loopnz", //0xe0}),
    shared_ptr<OpCode>(new OpCodeTwo{"loopz", //0xe1}),
    shared_ptr<OpCode>(new OpCodeTwo{"loop", //0xe2}),
    shared_ptr<OpCode>(new OpCodeTwo{"jcxz", //0xe3}),
    shared_ptr<OpCode>(new OpCodeTwo{"in al", //0xe4}),
    shared_ptr<OpCode>(new OpCodeTwo{"in ax", //0xe5}),
    shared_ptr<OpCode>(new OpCodeTwo{"out", //0xe6}),
    shared_ptr<OpCode>(new OpCodeTwo{"out", //0xe7}),
    shared_ptr<OpCode>(new OpCodeThree{"call", //0xe8}),
    shared_ptr<OpCode>(new OpCodeThree{"jmp", //0xe9}),
    shared_ptr<OpCode>(new OpCodeThree{"jmp seg", //0xea}),
    shared_ptr<OpCode>(new OpCodeTwo{"jmp", //0xeb}),
    shared_ptr<OpCode>(new OpCode{"in al", //0xec}),
    shared_ptr<OpCode>(new OpCode{"in ax", //0xed}),
    shared_ptr<OpCode>(new OpCode{"out", //0xee}),
    shared_ptr<OpCode>(new OpCode{"out", //0xef}),

    shared_ptr<OpCode>(new OpCode{"lock", //0xf0}),
    shared_ptr<OpCode>(new OpCode{"n/a", //0xf1}),
    shared_ptr<OpCode>(new OpCode{"repne", //0xf2}),
    shared_ptr<OpCode>(new OpCode{"rep", //0xf3}),
    shared_ptr<OpCode>(new OpCode{"hlt", //0xf4}),
    shared_ptr<OpCode>(new OpCode{"cmc", //0xf5}),
    shared_ptr<OpCode>(new OpCodeTwo{"_alu2", //0xf6}),
    shared_ptr<OpCode>(new OpCodeTwo{"_alu2", //0xf7}),
    shared_ptr<OpCode>(new OpCode{"clc", //0xf8}),
    shared_ptr<OpCode>(new OpCode{"stc", //0xf9}),
    shared_ptr<OpCode>(new OpCode{"cli", //0xfa}),
    shared_ptr<OpCode>(new OpCode{"sti", //0xfb}),
    shared_ptr<OpCode>(new OpCode{"cld", //0xfc}),
    shared_ptr<OpCode>(new OpCode{"std", //0xfd}),
    shared_ptr<OpCode>(new OpCodeTwo{"_misc", //0xfe}),
    shared_ptr<OpCode>(new OpCodeTwo{"_misc", //0xff}),
  
}*/
/*
pOpCode Disassm::modrm(const uint8_t &code, bstring& inp, size_t& offset){

}*/

pOpCode Disassm::modregrm(const uint8_t &code, const bstring& inp, size_t& offset) {
    std::cout << "CALLED" << std::endl;
    if (offset >= inp.size()) {
        throw out_of_range("at " + to_string(offset - 1));
    }
    const uint8_t &code2 = inp[offset++];
   /* string mnemonic;
    switch (0xFC & code) {
        case TEST_DW_MASK:    //0x84 //no w
            mnemonic = "test";
            break;
        case ADD_DW_MASK: //0x00
            mnemonic = "add";
            break;
        case ADC_DW_MASK: //0x10
            mnemonic = "adc";
            break;
        case SUB_DW_MASK: //0x28
            mnemonic = "sub";
            break;
        case SSB_DW_MASK: //0x18
            mnemonic = "ssb";
            break;
        case CMP_DW_MASK: //0x38
            mnemonic = "cmp";
            break;
        case AND_DW_MASK: //0x20
            mnemonic = "and";
            break;
        case OR_DW_MASK:  //0x08
            mnemonic = "or";
            break;
        case XOR_DW_MASK: //0x30
            mnemonic = "xor";
            break;
    }*/
    switch (code2 & 0xC0) {
        case 0x40:
        {
            if (offset >= inp.size()) {
                throw out_of_range("at " + to_string(offset - 2));
            }
            const uint8_t &code3 = inp[offset++];
            return shared_ptr<OpCode>(new OpCode(bstring{code, code2, code3}));
        }
        case 0xC0:
        case 0x00:
            if (code2 != 06) {
                return shared_ptr<OpCode>(new OpCode(bstring{code, code2}));
            }
            // no break: if code2 == 06 - fell through to 0x80 handler
        case 0x80:
        default:
            if ((offset + 1) >= inp.size()) {
                throw out_of_range("at " + to_string(offset - 2));
            }
            const uint8_t &code3 = inp[offset++];
            const uint8_t &code4 = inp[offset++];
            return shared_ptr<OpCode>(new OpCode(bstring{code, code2, code3, code4}));
    }

}

pOpCode Disassm::decode(const bstring& inp, size_t& offset) {
    if (offset >= inp.size()) {
        throw out_of_range("at " + to_string(offset));
    }
    const uint8_t &code = inp[offset++];

    switch (code) {  
        case BAA:     //0x27
        case DAS:     //0x2F
        case AAA:     //0x37
        case AAS:     //0x3F
        case INC_AX:  //0x40
        case INC_CX:  //0x41
        case INC_DX:  //0x42
        case INC_BX:  //0x43
        case INC_SP:  //0x44
        case INC_BP:  //0x45
        case INC_SI:  //0x46
        case INC_DI:  //0x47
        case DEC_AX:  //0x40
        case DEC_CX:  //0x41
        case DEC_DX:  //0x42
        case DEC_BX:  //0x43
        case DEC_SP:  //0x44
        case DEC_BP:  //0x45
        case DEC_SI:  //0x46
        case DEC_DI:  //0x47
        case CBW:     //0x98
        case CWD:     //0x99
        case WAIT:    //0x9B
        case MOVS:    //0xA4
        case MOVSW:   //0xA5
        case CMPS:    //0xA6
        case CMPSW:   //0xA7
        case STOS:    //0xAA
        case STOSW:   //0xAB
        case LODS:    //0xAC
        case LODSW:   //0xAD
        case SCAS:    //0xAE
        case SCASW:   //0xAF
        case RET:     //0xC3
        case RETL:    //0xCB
        case INT:     //0xCC    //Type 3
        case INTO:    //0xCE
        case IRET:    //0xCF
        case LOCK:    //0xF0
        case REP:     //0xF2
        case REPZ:    //0xF3
        case HLT:     //0xF4
        case CMC:     //0xF5
        case CLC:     //0xF8
        case STC:     //0xF9
        case CLI:     //0xFA
        case STI:     //0xFB
        case CLD:     //0xFC
        case STD:     //0xFD
            return shared_ptr<OpCode>(new OpCode{bstring{code}});
        case 0xFF: 
        {
            if (offset >= inp.size()) {
                throw out_of_range("at " + to_string(offset - 1));
            }
            const uint8_t &code2 = inp[offset];
            if ((code2 & 0x38) == 0x38 ||
                (code2 & 0x38) == 0x30) {
                
                //return shared_ptr<OpCode>(new OpCodeNA{bstring{code, code2}});
                THROW_INVALID(offset - 1, code, code2);
            }
            std::cout << "HERE" << std::hex << (int) code << std::endl;
            return modregrm(code, inp, offset);
        }    
        break;
        case 0xEA:      //intersegment jump
        case 0x9A:      //intersegment call
        {
            if ((offset + 3) >= inp.size()) {
                throw out_of_range("at " + to_string(offset - 1));
            }
            const uint8_t &code2 = inp[offset++];
            const uint8_t &code3 = inp[offset++];
            const uint8_t &code4 = inp[offset++];
            const uint8_t &code5 = inp[offset++];
            return shared_ptr<OpCode>(new OpCode{bstring{code, code2, code3, code4, code5}});
        }
        break;
        case 0xCD:      //int x
        case 0xEB:      //jmp
        case 0x74:      //JE/JZ
        case 0x7C:      //JL/JNGE
        case 0x7E:      //JLE/JNG
        case 0x72:      //JB/JNAE
        case 0x76:      //JBE/JNA
        case 0x7A:      //JP/JPE
        case 0x70:      //JO
        case 0x78:      //JS
        case 0x75:      //JNE/JNZ
        case 0x7D:      //JNL/JGE
        case 0x7F:      //JNLE/JG
        case 0x73:      //JNB/JA
        case 0x77:      //JNBE/JA
        case 0x7B:      //JNP/JPO
        case 0x71:      //JNO
        case 0x79:      //JNS
        //or (code & 0xF0 == 0x70)
        {
            if (offset >= inp.size()) {
                throw out_of_range("at " + to_string(offset - 1));
            }
            const uint8_t &code2 = inp[offset++];
            return shared_ptr<OpCode>(new OpCode{bstring{code, code2}});
        }    
        default:
        break;
    }

    switch (0xFC & code) {
        case 0xD0: //Rotate
        {
            if (offset >= inp.size()) {
                throw out_of_range("at " + to_string(offset - 1));
            }
            const uint8_t &code2 = inp[offset];
            if ((code2 & 0x38) == 0x30) {
                THROW_INVALID(offset - 1, code, code2);
                //return shared_ptr<OpCode>(new OpCodeNA(bstring{code, code2}));
            }
            std::cout << "HERE=0x" << std::setw(2) << std::hex << (int) code << " " << std::hex << (int) code2 << std::endl;
            return modregrm(code, inp, offset);
        }       
        case TEST_DW_MASK:    //0x84 //no w
            if ((0xFE & code) == 0x86) {
                break;
            }
        //break; deliberately no break
        case ADD_DW_MASK: //0x00
        case ADC_DW_MASK: //0x10
        case SUB_DW_MASK: //0x28
        case SSB_DW_MASK: //0x18
        case CMP_DW_MASK: //0x38
        case AND_DW_MASK: //0x20
        case OR_DW_MASK:  //0x08
        case XOR_DW_MASK: //0x30

        case 0xF6:
        {
            if (offset >= inp.size()) {
                throw out_of_range("at " + to_string(offset - 1));
            }
            const uint8_t &code2 = inp[offset];
            if (code == 0xF6 && code2 == 0x01) {
                THROW_INVALID(offset - 1, code, code2);
                //return shared_ptr<OpCode>(new OpCodeNA(bstring{code, code2}));
            }
        }
            // no break, fell through

        case 0x80: //Group
        
            std::cout << "HERE" << std::hex << (int) code << std::endl;
            return modregrm(code, inp, offset);
        case 0xE0:
            {
                if (offset >= inp.size()) {
                    throw out_of_range("at " + to_string(offset - 1));
                }
                const uint8_t &code2 = inp[offset++];  
                THROW_INVALID(offset - 1, code, code2);
                //return shared_ptr<OpCode>(new OpCodeNA(bstring{code, code2}));          
            }
        default:
            break;
    }
    switch (0xFE & code) {
        case 0xE8:          // jmp or call
        case ADD_IA_MASK: //0x04
        case ADC_IA_MASK: //0x14
        case SUB_IA_MASK: //0x2C
        case SSB_IA_MASK: //0x1C
        case CMP_IA_MASK: //0x3C
        case AND_IA_MASK: //0x24
        case TEST_IA_MASK: //0xA8 //no w
        case OR_IA_MASK:  //0x0C
        case XOR_IA_MASK: //0x34
        {
            if (offset >= inp.size()) {
                throw out_of_range("at " + to_string(offset - 1));
            }
            const uint8_t &code2 = inp[offset++];
            if ((code & 0x01) == 0x01) {
                if (offset >= inp.size()) {
                    throw out_of_range("at " + to_string(offset - 2));
                }
                const uint8_t &code3 = inp[offset++];
                return shared_ptr<OpCode>(new OpCode{bstring{code, code2, code3}});
            } else {
                return shared_ptr<OpCode>(new OpCode{bstring{code, code2}});
            }            
        }
    }
    if ((code & 0xF8) == 0xD8) { //ESC
        std::cout << "HERE" << std::hex << (int) code << std::endl;
        return modregrm(code, inp, offset);
    }
    if ((code & 0xF7) == 0xC2) { // variants of RET
        if ((offset + 1) >= inp.size()) {
            throw out_of_range("at " + to_string(offset - 1));
        }
        const uint8_t &code2 = inp[offset++];
        const uint8_t &code3 = inp[offset++];
        return shared_ptr<OpCode>(new OpCode{bstring{code, code2, code3}});
    }

    throw out_of_range("AT THE END ");
}

}
