#ifndef _BASETYPES_H__
#define _BASETYPES_H__

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

namespace sim86 {

class bstring : public std::basic_string<uint8_t>{
public:
    bstring(uint8_t code);
    bstring(uint8_t code, uint8_t code1);
    bstring(uint8_t code, uint8_t code1, uint8_t code2);
    bstring(uint8_t code, uint8_t code1, uint8_t code2, uint8_t code3);
    bstring(uint8_t code, uint8_t code1, uint8_t code2, uint8_t code3, uint8_t code4);

    friend std::ostream& operator<<(std::ostream &os, const bstring& bstr) {
        for (auto ch:bstr) {
            std::cout << (char) ch;
        }
        return os;
    }
};
#if 0
class ROM : public bstring {
    public:
        ROM(uint32_t start = 0) :
            d_start(start), bstring(0){} //TODO

    void mark_bits(size_t start, uint32_t count) {
        for (size_t tt = start; tt < start + count; tt++) {
            bitset[tt] = true;
        }
    }
    bool was_visited(size_t start, uint32_t count) {
        for (size_t tt = start; tt < start + count; tt++) {
            if (bitset[tt]) return true;
        }
        return false;
    }

    std::vector<ROM> get_uprocessed_areas(){
        std::vector<ROM> ret;
        ssize_t start = -1;
        for (size_t tt = 0; tt < size(); tt++) {
            if (start < 0 && !bitset[tt]) {
                start = tt;
            } else if (start >= 0 && bitset[tt]) {
                ret.emplace_back(construct_from(start, tt - start));
                start = -1;
            } 
        }
        return ret;
    }

    
    friend std::ostream& operator<<(std::ostream &os, const ROM& rom) {
        std::cout << "start=" << rom.d_start << std::endl;
        for (auto ch:(bstring) rom) {
            std::cout << (char) ch;
        }
        return os;
    }

    friend ROM& operator<<(ROM& rom, std::istream &is) {
        is.seekg (0, is.end);
        size_t length = is.tellg();
        is.seekg (0, is.beg);
        rom.reserve(length);
        is.read((char*) rom.data(), length);
        rom.bitset = std::vector<bool>(length, false);
        return rom;
    }
    protected:

        ROM construct_from(size_t start, size_t length) {
            ROM rom(start+d_start);
            rom.reserve(length);
            memcpy(rom.data(), data()+start, length);
            rom.bitset = std::vector<bool>(length, false);
            return rom;
        }

        uint32_t    d_start;
        std::vector<bool> bitset; 
};
#endif

}

#endif //_BASETYPES_H__