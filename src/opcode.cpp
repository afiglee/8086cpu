#include "opcode.h"
#include "basetypes.h"

#include <sstream>
#include <memory>
#include <iostream>
#include  <iomanip>

using std::runtime_error;
using std::range_error;
using std::make_shared;
using std::stringstream;
namespace sim86{

OpCode::OpCode(bstring && bcode, bool byte_operands, enum PREFIX prefix, enum DIALECT eDialect):
    m_operands{std::move(bcode)}, m_byte_operands{byte_operands}, 
    m_ePrefix(prefix), m_eDialect{eDialect}
{

}

std::ostream& operator<<(std::ostream& os, const OpCode &oCode)
{
    const uint8_t &code = oCode.m_operands[0];
    //os << (oCode.m_byte_operands?"b ":"w ");
    switch (code & 0xF0) {
        default:
            break;
    }
    switch (code & 0xF8) {
        case MOV_IRW_MASK3:
        {
            os << "mov " << OpCode::get_register16_name(code) << ", " << TO_UINT16(oCode.m_operands[1], oCode.m_operands[2]);
        }
        return os;
        case MOV_IRB_MASK3:
        {
            os << "mov " << OpCode::get_register8_name(code) << ", " << oCode.m_operands[1];
        }
        return os;
        default:
            break;
    }
    switch (code & 0xFC) {
        case ROT_MASK2:
        {
            switch (oCode.m_operands[1] & 0x38) {
                case 0:
                    os << "rol ";
                    break;
                case 0x08:
                    os << "ror ";
                    break;
                case 0x10:
                    os << "rcl ";
                    break;
                case 0x18:
                    os << "rcr ";
                    break;
                case 0x20:
                    os << "shl ";
                    break;
                case 0x28:
                    os << "shr ";
                    break;
                case 0x30:
                    os << "--- ";
                    break;
                default:
                    os << "sar ";
                    break;
            }
            os << oCode.decode_mod_rm();
            if (code & 0x02) {
                os << ", cl";
            } else {
                os << ", 1";
            }
        }
        return os;
        case XOR_DW_MASK: os << "xor " << oCode.decode_dw_mod_rm(); return os;
        case OR_DW_MASK: os << "or " << oCode.decode_dw_mod_rm(); return os;        
        case MOV_2R_MASK: os << "mov " << oCode.decode_dw_mod_rm(); return os;
        case ADD_DW_MASK: os << "add " << oCode.decode_dw_mod_rm(); return os;
        case ADC_DW_MASK: os << "adc " << oCode.decode_dw_mod_rm(); return os;
        case SUB_DW_MASK: os << "sub " << oCode.decode_dw_mod_rm(); return os;
        case SSB_DW_MASK: os << "ssb " << oCode.decode_dw_mod_rm(); return os;
        case CMP_DW_MASK: os << "cmp " << oCode.decode_dw_mod_rm(); return os;
        case AND_DW_MASK: os << "and " << oCode.decode_dw_mod_rm(); return os;
        default:
            break;
    }
    switch (code & 0xFE) {
        case IN_MASK: os << "in " << (oCode.m_byte_operands?"al":"ax") << ", " << oCode.m_operands[1];
            return os;
        case IN_MASK_REG: os << "in " << (oCode.m_byte_operands?"al":"ax") << ", dx";
            return os;        
        case OUT_MASK: os << "out " << oCode.m_operands[1] << ", " << (oCode.m_byte_operands?"al":"ax");
            return os;
        case OUT_MASK_REG: os << "out dx, " << (oCode.m_byte_operands?"al":"ax"); 
            return os;
        case INC_MASK: os << "inc " << oCode.decode_mod_rm();
            return os;
        case ADD_IA_MASK: os << "add " << oCode.decode_data(); return os;
        case ADC_IA_MASK: os << "adc " << oCode.decode_data(); return os;
        case SUB_IA_MASK: os << "sub " << oCode.decode_data(); return os;
        case SSB_IA_MASK: os << "ssb " << oCode.decode_data(); return os;
        case CMP_IA_MASK: os << "cmp " << oCode.decode_data(); return os;
        case AND_IA_MASK: os << "and " << oCode.decode_data(); return os;
        case TEST_IA_MASK: os << "test " << oCode.decode_data(); return os;
        case OR_IA_MASK: os << "os " << oCode.decode_data(); return os;
        case XOR_IA_MASK: os << "xor " << oCode.decode_data(); return os;
        default:
            break;
    }
    switch(code) {
        case BAA: os << "baa"; break;
        case DAS: os << "das"; break;
        case AAA: os << "aaa"; break;
        case AAS: os << "aas"; break;
        case INC_AX: os << "inc ax"; break;
        case INC_CX: os << "inc cx"; break;
        case INC_DX: os << "inc dx"; break;
        case INC_BX: os << "inc bx"; break;
        case INC_SP: os << "inc sp"; break;
        case INC_BP: os << "inc bp"; break;
        case INC_SI: os << "inc si"; break;
        case INC_DI: os << "inc di"; break;
        case DEC_AX: os << "dec ax"; break;
        case DEC_CX: os << "dec cx"; break;
        case DEC_DX: os << "dec dx"; break;
        case DEC_BX: os << "dec bx"; break;
        case DEC_SP: os << "dec sp"; break;
        case DEC_BP: os << "dec bp"; break;
        case DEC_SI: os << "dec si"; break;
        case DEC_DI: os << "dec di"; break;
        case CBW: os << "cbw"; break;
        case CWD: os << "cwd"; break;
        case WAIT: os << "wait"; break;
        case MOVS: os << "movs"; break;
        case MOVSW: os << "movsw"; break;
        case CMPS: os << "cmps"; break;
        case CMPSW: os << "cmpsw"; break;
        case STOS: os << "stos"; break;
        case STOSW: os << "stosw"; break;
        case LODS: os << "lods"; break;
        case LODSW: os << "lodsw"; break;
        case SCAS: os << "scas"; break;
        case SCASW: os << "scasw"; break;
        case RET: os << "ret"; break;        
        case INT: os << "int3"; break;    //Type 3
        case INTO: os << "int"; break;
        case IRET: os << "iret"; break;
        case LOCK: os << "lock"; break;
        case REP: os << "rep"; break;
        case REPZ: os << "repz"; break;
        case HLT: os << "hlt"; break;
        case CMC: os << "cmc"; break;
        case CLC: os << "clc"; break;
        case STC: os << "stc"; break;
        case CLI: os << "cli"; break;
        case STI: os << "sti"; break;
        case CLD: os << "cld"; break;
        case STD: os << "std"; break;
        case LAHF: os << "lahf"; break;
        case SAHF: os << "sahf"; break;
        case RETF: os << "retf"; break;
        case MOV_R2SEG:
        {
            os << "mov " << OpCode::get_segregister_name(oCode.m_operands[1] >> 3) << ", " << oCode.decode_mod_rm();
        }
        break;
        case MOV_SEG2R:
        {
            os << "mov " << oCode.decode_mod_rm() << ", " << OpCode::get_segregister_name(oCode.m_operands[1] >> 3);
        }
        break;
        case LJMP: //long_jump
        {
            
            uint16_t offset = TO_UINT16(oCode.m_operands[1], oCode.m_operands[2]);
            uint16_t segm = TO_UINT16(oCode.m_operands[3], oCode.m_operands[4]);
            uint32_t addr = OpCode::calc_address(segm, offset);
            os << "ljmp ";
            print20(os, addr);
        }
        break;
        default:
        {
            stringstream ss;
            ss << "operator<< is missing for opcodes:";
            for (auto code: oCode.m_operands) {
                ss << " " << code;
            }
            throw sim86::unimplemented_exception(ss.str());
        }

        break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const AddressedOpCode &oCode)
{
    const uint8_t &code = oCode.m_operands[0];
    switch (code & 0xF0) {
        case JXX_MASK4:
        {
            //os << oCode.m_own_address << " " << oCode.m_operands[1] << " ";
            switch (code) {
                case JO:
                    os << "jo";
                    break;
                case JNO:
                    os << "jno";
                    break;
                case JB:
                    os << "jb";
                    break;
                case JAE:
                    os << "jae";
                    break;
                case JE:
                    os << "je";
                    break;
                case JNE:
                    os << "jne";
                    break;
                case JBE:
                    os << "jbe";
                    break;
                case JA:
                    os << "ja";
                    break;    
                case JS:
                    os << "js";
                    break;
                case JNS:
                    os << "jns";
                    break;
                case JP:
                    os << "jp";
                    break;
                case JNP:
                    os << "jnp";
                    break;
                case JL:
                    os << "jl";
                    break;
                case JGE:
                    os << "jge";
                    break;
                case JLE:
                    os << "jle";
                    break; 
                default:   
                case JG:
                    os << "jg";
                    break;

            }
            /*uint16_t goto_address = oCode.m_own_address +2;
            goto_address += (int8_t) oCode.m_operands[1];*/
            os << " ";
            print16(os, OpCode::calc_disp(oCode.m_own_address, (int8_t) oCode.m_operands[1]));
        }
        return os;
        default:
            break;
    }
    switch (code) {
        case JMP_SEG:
        {
            os << "jmp ";
            uint16_t goto_address = oCode.m_own_address + 3;
            goto_address += (int16_t) TO_UINT16(oCode.m_operands[1], oCode.m_operands[2]);
            os << " ";
            print16(os, goto_address);
            return os;
        }
        case LOOPNZ: os << "loopnz "; print16(os, OpCode::calc_disp(oCode.m_own_address, (int8_t) oCode.m_operands[1])); return os;
        case LOOPZ:  os << "loopz "; print16(os, OpCode::calc_disp(oCode.m_own_address, (int8_t) oCode.m_operands[1])); return os;
        case LOOP:   os << "loop "; print16(os, OpCode::calc_disp(oCode.m_own_address, (int8_t) oCode.m_operands[1])); return os;
        case JCXZ:   os << "jcxz "; print16(os, OpCode::calc_disp(oCode.m_own_address, (int8_t) oCode.m_operands[1])); return os;

        default:
            break;        
    }
    stringstream ss;
    ss << "operator << is missing for opcodes:";
    for (auto code: oCode.m_operands) {
        ss << " " << code;
    }
    throw sim86::unimplemented_exception(ss.str());

}

string OpCode::decode_dw_mod_rm() const
{
    string ret;
    if (m_operands[0] & 0x02) {
        ret = decode_register_name();
        ret += ", ";
        ret += decode_mod_rm();
    } else {
        ret = decode_mod_rm();
        ret += ", ";
        ret += decode_register_name();
    }
    return ret;
}

uint16_t OpCode::calc_disp(uint16_t addr, int8_t disp) {
    uint16_t goto_address = addr + 2;
    goto_address += disp;
    return goto_address;
}

string OpCode::decode_register_name() const
{
    if (m_byte_operands) {
        return get_register8_name(m_operands[1] >> 3);
    } else {
        return get_register16_name(m_operands[1] >> 3);
    }
}

string OpCode::decode_mod_rm() const
{
    uint8_t sw = m_operands[1] & 0xC0;
    stringstream ss;
    //ss << m_operands[0] << " " << m_operands[1] << " ";
    if (sw != 0xC0) {
        
        switch (m_ePrefix) {
            case PREFIX_CS: ss << (m_byte_operands?"byte ptr cs:[":"word ptr ["); break;
            case PREFIX_DS: ss << (m_byte_operands?"byte ptr ds:[":"word ptr ["); break;
            case PREFIX_ES: ss << (m_byte_operands?"byte ptr es:[":"word ptr ["); break;
            case PREFIX_SS: ss << (m_byte_operands?"byte ptr ss:[":"word ptr ["); break;
            default:
                ss << (m_byte_operands?"byte ptr [":"word ptr [");
        }
        switch (m_operands[1] & 0x07) {
            case 0:
                ss << "bx + si";
                break;
            case 1:
                ss << "bx + di";
                break;
            case 2:
                ss << "bp + si";
                break;
            case 3:
                ss << "bp + di";
                break;
            case 4:
                ss << "si";
                break;
            case 5:
                ss << "di";
                break;
            case 6:
                ss << "bp";
                break;
            default:
                ss << "bx";
        }
        if (sw == 0x40) {
            if (m_operands[2] & 0x80) {
                uint8_t op = m_operands[2] & 0x7F;
                op = 0xFF - op + 1;
                ss << " - " << op;
            } else {
                ss << " " << m_operands[2];
            }
        } else if (sw == 0x80) {
            uint16_t op = TO_UINT16(m_operands[2], m_operands[3]);
            if (op & 0x8000) {
                op &= 0x7FFF;
                op = 0xFFFF - op + 1;
                ss << " - ";
                print16(ss, op);
            } else {
                ss << " " << op;
            }
        }
        ss << "]";
    } else { //0xC0 - regs
        switch (m_operands[1] & 0x07) {
            case 0:
                ss << (m_byte_operands?"al":"ax");
                break;
            case 1:
                ss << (m_byte_operands?"cl":"cx");
                break;
            case 2:
                ss << (m_byte_operands?"dl":"dx");
                break;
            case 3:
                ss << (m_byte_operands?"bl":"bx");
                break;
            case 4:
                ss << (m_byte_operands?"ah":"sp");
                break;
            case 5:
                ss << (m_byte_operands?"ch":"bp");
                break;
            case 6:
                ss << (m_byte_operands?"dh":"si");
                break;
            default:
                ss << (m_byte_operands?"bh":"di");
                break;
        }
    }
    return ss.str();
}

std::string OpCode::get_register8_name(uint8_t reg)
{
    switch (reg & 0x07) {
        case 00:
            return "al";
        case 01:
            return "cl";
        case 02:
            return "dl";
        case 03:
            return "bl";
        case 04:
            return "ah";
        case 05:
            return "ch";
        case 06:
            return "dh";
        default: // to turn off warning    
        case 07:
            return "bh";
    }
}

std::string OpCode::get_register16_name(uint8_t reg)
{
    switch (reg & 0x07) {
        case 00:
            return "ax";
        case 01:
            return "cx";
        case 02:
            return "dx";
        case 03:
            return "bx";
        case 04:
            return "sp";
        case 05:
            return "bp";
        case 06:
            return "si";
        default: //to turn off warning    
        case 07:
            return "di";
    }

}

std::string OpCode::get_segregister_name(uint8_t reg)
{
    switch (reg & 0x03) {
        case 0:
            return "es";
        case 1:
            return "cs";
        case 2:
            return "ss";
        default:
            return "ds";    
    }
}

uint32_t OpCode::calc_address(uint16_t segment, uint16_t offset) {
    uint32_t ret = offset;
    ret += (segment << 4);
    return ret;
}

ssize_t OpCode::calc_new_address(uint32_t start, size_t offset) const {
    ssize_t new_offset = offset;
    if (m_operands[0] == LJMP) {
        //new_offset = calc_address(m_operands[4] << 8 | m_operands[3], m_operands[2] << 8 | m_operands[1]);
        new_offset = calc_address(TO_UINT16(m_operands[3], m_operands[4]), TO_UINT16(m_operands[1], m_operands[2]));
        new_offset -= start;
    }
    return new_offset;
}

string OpCode::decode_data() const {
    stringstream ss;
    if (m_byte_operands) {
        ss << m_operands[1];
    } else {
        print16(ss, TO_UINT16(m_operands[1], m_operands[2]));
    }
    return ss.str();
}
}
