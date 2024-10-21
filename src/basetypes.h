#ifndef _BASETYPES_H__
#define _BASETYPES_H__

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <exception>

namespace sim86 {

#define TO_UINT16(LOWER, UPPER) ((uint16_t) ((UPPER << 8) | (LOWER)))

std::ostream &operator<<(std::ostream& os, const uint8_t &u);
std::ostream &print16(std::ostream& os, const uint16_t u);
std::ostream &print20(std::ostream& os, const uint32_t u);
//std::ostream &operator<<(std::ostream& os, const uint16_t &u);

class unimplemented_exception : public std::exception {
    public:
        unimplemented_exception(std::string && msg):
            m_msg{std::move(msg)}{}
        unimplemented_exception(const std::string &msg):
            m_msg{msg}{}   

        const char* what() const noexcept override{
            return m_msg.c_str();
        };     
    protected:    
        std::string m_msg;
};
class bstring : public std::basic_string<uint8_t>{
public:
    bstring(uint8_t code);
    bstring(uint8_t code, uint8_t code1);
    bstring(uint8_t code, uint8_t code1, uint8_t code2);
    bstring(uint8_t code, uint8_t code1, uint8_t code2, uint8_t code3);
    bstring(uint8_t code, uint8_t code1, uint8_t code2, uint8_t code3, uint8_t code4);
    bstring(uint8_t *data, size_t size);
    friend std::ostream& operator<<(std::ostream &os, const bstring& bstr) {
        for (auto ch:bstr) {
            std::cout << (char) ch;
        }
        return os;
    }
};

}

#endif //_BASETYPES_H__