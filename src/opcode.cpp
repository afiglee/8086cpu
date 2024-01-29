#include "opcode.h"

#include <sstream>
#include <memory>
#include <iostream>


using std::runtime_error;
using std::range_error;
using std::make_shared;
using std::stringstream;

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

pOpCode OpCode::get(const bin_string& inp, size_t& offset) {
    m_operands.erase(1); // clear previous operands if any    
    offset++; //move pointer to the first operand
    return _get(inp, offset);
}

pOpCode OpCode::_get(const bin_string& inp, size_t& offset) {
    return make_shared<OpCode>(*this);
}

const string& OpCode::mnemonic() const{
    return m_mnemonic;
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

