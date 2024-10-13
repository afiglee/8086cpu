#ifndef __CPU_H__
#define __CPU_H__

#include <cstdint>
#include "basetypes.h"

namespace afiglee {
class Reg final {
public:


protected:
    uint16_t m_val;
};

class CPU {
public:
    CPU() = default;
    virtual ~CPU() = default;

    // returns number of cycles operation took
    virtual size_t process(const ROM& rom){ return 0;}
};

class CPU_8086: public CPU 
{ 
public: 
        CPU_8086() = default;

        void reset();
protected:
    Reg acc;
    Reg AX;
    Reg CX;
    Reg DX;
    Reg BX;

    Reg BP;
    Reg SP;
    Reg SI;
    Reg DI;
    //segments
    Reg ES;
    Reg CS;
    Reg SS;
    Reg DS;
};

}

#endif