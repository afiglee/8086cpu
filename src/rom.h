#ifndef _ROM_H__
#define _ROM_H__

#include "basetypes.h"
#include "opcode.h"

#include <sstream>

using std::stringstream;

namespace sim86 {
class ROM : public bstring {
    public:
        ROM(int32_t start = 0) :
            d_start(start), bstring((uint8_t)0){} //TODO

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
    size_t get_next_offset(const pOpCode pCode, size_t offset) {
        ssize_t new_offset = pCode->calc_new_address(d_start, offset);
        if (new_offset < 0) {
            stringstream ss;
            ss << "LJMP new offset out of ROM " << new_offset;
            throw std::out_of_range(ss.str());
        }
        return (size_t) new_offset;
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
        std::istreambuf_iterator<char> eos;
        std::string s(std::istreambuf_iterator<char>(is), eos);
        rom.assign((uint8_t*) s.data(), s.length());
        std::cout << "SIZE=" << length << " str.length=" << s.size() <<std::endl;
        
        /*is.read((char*) rom.data(), length);
        if (is) {
            std::cout << "ALL good" << std::endl;
        } else {
            std::cout << "NOT good" << std::endl;
        }*/
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

}

#endif