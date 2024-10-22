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

OpCode::OpCode(bstring && bcode, bool byte_operands, enum DIALECT eDialect):
    m_operands{std::move(bcode)}, m_byte_operands{byte_operands}, m_eDialect{eDialect}
{

}

std::ostream& operator<<(std::ostream& os, const OpCode &oCode)
{
    const uint8_t &code = oCode.m_operands[0];
    os << (oCode.m_byte_operands?"b ":"w ");
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
        default:
            break;
    }
    switch(code) {
        case LAHF:
        {
            os << "lahf";
        }
        break;
        case RET:
        {
            os << "ret";
        }
        break;
        case RETF:
        {
            os << "retf";
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
        case CLI:
        {
            os << "cli";
        }
        break;
        case SAHF:
        {
            os << "sahf";
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
            uint16_t goto_address = oCode.m_own_address +2;
            goto_address += (int8_t) oCode.m_operands[1];
            os << " ";
            print16(os, goto_address);
        }
        return os;
        default:
            break;
    }
    stringstream ss;
    ss << "operator<< is missing for opcodes:";
    for (auto code: oCode.m_operands) {
        ss << " " << code;
    }
    throw sim86::unimplemented_exception(ss.str());

}

string OpCode::decode_mod_rm() const
{
    uint8_t sw = m_operands[1] & 0xC0;
    stringstream ss;
    ss << m_operands[0] << " " << m_operands[1] << " ";
    if (sw != 0xC0) {
        ss << (m_byte_operands?"byte ptr [":"word ptr [");
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
        } else /*0x80*/{
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

uint32_t OpCode::calc_address(uint16_t segment, uint16_t offset) {
    uint32_t ret = offset;
    ret += (segment << 4);
    return ret;
}

ssize_t OpCode::calc_new_address(uint32_t start, size_t offset) {
    ssize_t new_offset = offset;
    if (m_operands[0] == LJMP) {
        //new_offset = calc_address(m_operands[4] << 8 | m_operands[3], m_operands[2] << 8 | m_operands[1]);
        new_offset = calc_address(TO_UINT16(m_operands[3], m_operands[4]), TO_UINT16(m_operands[1], m_operands[2]));
        new_offset -= start;
    }
    return new_offset;
}

#if 0
OpCodeNA::OpCodeNA(bstring && bcode, enum DIALECT eDialect):
    OpCode(std::move(bcode), eDialect)
{

}

static void to_reg(string& mnemonic, uint8_t code)
{
    switch (code & 0x07) {
        case 0:
            mnemonic.append("al");
            break;
        case 1:
            mnemonic.append("cl");
            break;
        case 2:
            mnemonic.append("dl");
            break;
        case 3:
            mnemonic.append("bl");
            break;
        case 4:
            mnemonic.append("ah");
            break;
        case 5:
            mnemonic.append("ch");
            break;
        case 6:
            mnemonic.append("dh");
            break;
        case 7:
            mnemonic.append("bh");
            break;
    }
}

static void to_reg16(string& mnemonic, uint8_t code)
{
    switch (code & 0x07) {
        case 0:
            mnemonic.append("ax");
            break;
        case 1:
            mnemonic.append("cx");
            break;
        case 2:
            mnemonic.append("dx");
            break;
        case 3:
            mnemonic.append("bx");
            break;
        case 4:
            mnemonic.append("sp");
            break;
        case 5:
            mnemonic.append("bp");
            break;
        case 6:
            mnemonic.append("si");
            break;
        case 7:
            mnemonic.append("di");
            break;
    }
}

OpCode::OpCode(const char *head, uint8_t code):
m_head(head)
{
    m_operands += code;
}

OpCodeNA::OpCodeNA(uint8_t code):
OpCode("n/a", code)
{
}

OpCodeTwo::OpCodeTwo(const char *head, uint8_t code, uint8_t code2):
OpCode(head, code)
{
    m_operands += code2;
}

OpCodeRelJump::OpCodeRelJump(const char *head, uint8_t code):
OpCodeTwo(head, code)
{
    
}

OpCodeThree::OpCodeThree(const char *head, uint8_t code):
OpCode(head, code)
{
    
}

pOpCode OpCode::get(const bin_string& inp, size_t& offset) {
    m_operands.erase(1); // clear previous operands if any    
    offset++; //move pointer to the first operand
    return _get(inp, offset);
}

std::ostream& operator<<(std::ostream& os, const OpCode oCode)
{
    os << "m_head=" << oCode.m_head
       << " m_mnemonic=" << oCode.m_mnemonic;
    return os;
}

pOpCode OpCode::_get(const bin_string& inp, size_t& offset) {
    if (inp.size() < offset) {
        stringstream ss;
        ss << m_head << " abruptly interrupted at " << offset;
        throw range_error(ss.str());
    }
    m_mnemonic = m_head;
    return make_shared<OpCode>(*this);
}

const string& OpCode::mnemonic() const{
    return m_mnemonic;
}

pOpCode OpCodeTwo::_get(const bin_string& inp, size_t& offset) 
{
    m_mnemonic = m_head;
    if (inp.size() <= offset) {
        stringstream ss;
        ss << m_head << " abruptly interrupted at " << offset;
        throw range_error(ss.str());
    }   
    m_operands += inp[offset++];
    readOperand(offset);
    return make_shared<OpCode>(*this);
}

void OpCodeTwo::readOperand([[maybe_unused]] size_t offset)
{
    stringstream sdata;
    sdata << ", 0x" << std::hex << (int) m_operands[1]; 
    m_mnemonic.append(sdata.str());
}

void OpCodeRelJump::readOperand([[maybe_unused]] size_t offset)
{
    stringstream sdata;
    int data = (int8_t) m_operands[1];
    offset += data;
    sdata << " 0x" << std::setfill('0') << std::setw(6) << std::hex << (int) offset; 
    m_mnemonic.append(sdata.str());
}

pOpCode OpCodeThree::_get(const bin_string& inp, size_t& offset) 
{
    m_mnemonic = m_head;
    if (inp.size() <= (offset + 1)) {
            stringstream ss;
            ss << m_head << " abruptly interrupted at " << offset;
            throw range_error(ss.str());
    }   
    stringstream sdata;
    uint16_t data = inp[offset++];
    m_operands += data;
    data += ((uint16_t) (inp[offset])) << 8;
    m_operands += inp[offset++];
    sdata << ", 0x" << std::hex << (int) data;
    m_mnemonic.append(sdata.str()); 
    return make_shared<OpCode>(*this);
}


AriphmeticOpCode::AriphmeticOpCode(const char* head, uint8_t code):
OpCode(head, code)
{

}

void AriphmeticOpCode::readOperands(const bin_string& inp, size_t& offset)
{
    size_t count = 2;
    do {
        if (inp.size() <= offset) {
            stringstream ss;
            ss << m_head << " abruptly interrupted at " << offset;
            throw range_error(ss.str());
        }
        m_operands += inp[offset];
        if ((inp[offset] & 0xC0) != 0x40) {
            offset++;
            break;
        }
        offset++;
        --count;
    } while(count);
     
    if (m_operands[1] > 0x7F && m_operands[1] < 0xC0) {
        // Not implemeneted
        stringstream ss;
        ss << m_head << " has n/a operand 0x" << std::hex << (int) m_operands[1] << " at " << offset;
        throw runtime_error(ss.str());
    }
    m_mnemonic = m_head;

}

OpCode0::OpCode0(const char* head, uint8_t code):
AriphmeticOpCode(head, code)
{

}

OpCode1::OpCode1(const char* head, uint8_t code):
AriphmeticOpCode(head, code)
{

}

OpCode2::OpCode2(const char* head, uint8_t code):
AriphmeticOpCode(head, code)
{

}

OpCode3::OpCode3(const char* head, uint8_t code):
AriphmeticOpCode(head, code)
{

}

OpCodeMulti::OpCodeMulti(uint8_t code):
OpCode("", code) 
{
}

pOpCode OpCode0::_get(const bin_string& inp, size_t& offset) {
    readOperands(inp, offset);
    uint8_t code = m_operands[1];
    size_t count = 2;
    if (code >= 0xC0) {
        do {
            m_mnemonic.append(" ");
            to_reg(m_mnemonic, code);
            count--;
            if (count) {
                m_mnemonic.append(",");
                code = ((code >> 3) & 0x07);

            }
        } while(count);
    } else /* < 0x80 */{
        m_mnemonic.append(" byte ptr");
        switch (m_operands[1] & 0x07) {
            case 0:
                m_mnemonic.append(" [bx + si");
                break;
            case 1:
                m_mnemonic.append(" [bx + di");
                break;
            case 2:
                m_mnemonic.append(" [bp + si");
                break;
            case 3:
                m_mnemonic.append(" [bp + di");
                break;
            case 4:
                m_mnemonic.append(" [si");
                break;
            case 5:
                m_mnemonic.append(" [di");
                break;
            case 6:
                {
                    stringstream ss;
                    ss << m_head << " has n/a operand 0x" << std::hex << (int) m_operands[1] << " at " << offset;
                    throw runtime_error(ss.str());

                }
                break;
            case 7:
                m_mnemonic.append(" [bx");
                break;
        }
        if ((m_operands[1] & 0xC0) == 0x40) {
            stringstream ss;
            ss << " + 0x" << std::hex << (int) m_operands[2];
            m_mnemonic.append(ss.str());
        }
        m_mnemonic.append("], ");
        code = m_operands[1] >> 3;
        to_reg(m_mnemonic, code);
    }
    return make_shared<OpCode>(*this);
}

pOpCode OpCode1::_get(const bin_string& inp, size_t& offset) {
    readOperands(inp, offset);
    uint8_t code = m_operands[1];
    size_t count = 2;
    
    if (code >= 0xC0) {
        do {
            m_mnemonic.append(" ");
            to_reg16(m_mnemonic, code);
            count--;
            if (count) {
                m_mnemonic.append(",");
                code >>= 3;
            }
        } while(count);
    } else /* < 0x80 */{
        m_mnemonic.append(" word ptr");
        switch (m_operands[1] & 0x07) {
            case 0:
                m_mnemonic.append(" [bx + si");
                break;
            case 1:
                m_mnemonic.append(" [bx + di");
                break;
            case 2:
                m_mnemonic.append(" [bp + si");
                break;
            case 3:
                m_mnemonic.append(" [bp + di");
                break;
            case 4:
                m_mnemonic.append(" [si");
                break;
            case 5:
                m_mnemonic.append(" [di");
                break;
            case 6:
                {
                    stringstream ss;
                    ss << m_head << " has n/a operand 0x" << std::hex << (int) m_operands[1] << " at " << offset;
                    throw runtime_error(ss.str());

                }
                break;
            case 7:
                m_mnemonic.append(" [bx");
                break;
        }
        if ((m_operands[1] & 0xC0) == 0x40) {
            stringstream ss;
            ss << " + 0x" << std::hex << (int) m_operands[2];
            m_mnemonic.append(ss.str());
        }
        m_mnemonic.append("], ");
        code = m_operands[1] >> 3;
        to_reg16(m_mnemonic, code);
    }
    return make_shared<OpCode>(*this);
}

pOpCode OpCode2::_get(const bin_string& inp, size_t& offset) {
    readOperands(inp, offset);
    uint8_t code = m_operands[1];
    size_t count = 2;
    
    if (code >= 0xC0) {
        m_mnemonic.append(" ");
        to_reg(m_mnemonic, (code >> 3) & 0x07);
        m_mnemonic.append(", ");
        to_reg(m_mnemonic, code);
    } else /* < 0x80 */{
        m_mnemonic.append(" ");
        to_reg(m_mnemonic, (code >> 3) & 0x07);
        m_mnemonic.append(", byte ptr");
        switch (m_operands[1] & 0x07) {
            case 0:
                m_mnemonic.append(" [bx + si");
                break;
            case 1:
                m_mnemonic.append(" [bx + di");
                break;
            case 2:
                m_mnemonic.append(" [bp + si");
                break;
            case 3:
                m_mnemonic.append(" [bp + di");
                break;
            case 4:
                m_mnemonic.append(" [si");
                break;
            case 5:
                m_mnemonic.append(" [di");
                break;
            case 6:
                {
                    stringstream ss;
                    ss << m_head << " has n/a operand 0x" << std::hex << (int) m_operands[1] << " at " << offset;
                    throw runtime_error(ss.str());

                }
                break;
            case 7:
                m_mnemonic.append(" [bx");
                break;
        }
        if ((m_operands[1] & 0xC0) == 0x40) {
            stringstream ss;
            ss << " + 0x" << std::hex << (int) m_operands[2];
            m_mnemonic.append(ss.str());
        }
        m_mnemonic.append("]");
        //code = m_operands[1] >> 3;
        //to_reg(m_mnemonic, code);
    }
    return make_shared<OpCode>(*this);
}

pOpCode OpCode3::_get(const bin_string& inp, size_t& offset) {
    readOperands(inp, offset);
    uint8_t code = m_operands[1];
    size_t count = 2;
    m_mnemonic.append(" ");
    
    if (code >= 0xC0) {
        to_reg16(m_mnemonic, (code >> 3) & 0x07);
        m_mnemonic.append(", ");
        to_reg16(m_mnemonic, code);
    } else /* < 0x80 */{
        to_reg16(m_mnemonic, (code >> 3) & 0x07);
        m_mnemonic.append(", word ptr");
        switch (m_operands[1] & 0x07) {
            case 0:
                m_mnemonic.append(" [bx + si");
                break;
            case 1:
                m_mnemonic.append(" [bx + di");
                break;
            case 2:
                m_mnemonic.append(" [bp + si");
                break;
            case 3:
                m_mnemonic.append(" [bp + di");
                break;
            case 4:
                m_mnemonic.append(" [si");
                break;
            case 5:
                m_mnemonic.append(" [di");
                break;
            case 6:
                {
                    stringstream ss;
                    ss << m_head << " has n/a operand 0x" << std::hex << (int) m_operands[1] << " at " << offset;
                    throw runtime_error(ss.str());

                }
                break;
            case 7:
                m_mnemonic.append(" [bx");
                break;
        }
        if ((m_operands[1] & 0xC0) == 0x40) {
            stringstream ss;
            ss << " + 0x" << std::hex << (int) m_operands[2];
            m_mnemonic.append(ss.str());
        }
        m_mnemonic.append("]");
        
    }
    return make_shared<OpCode>(*this);
}

pOpCode OpCodeMulti::_get(const bin_string& inp, size_t& offset)
{
    return OpCode::_get(inp, offset);
}
#endif
}
