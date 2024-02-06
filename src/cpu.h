#ifndef __CPU_H__
#define __CPU_H__

class Reg final {
public:


protected:
    uint16_t m_val;
};

class CPU {
public:
    CPU();
    virtual ~CPU();

};

class CPU_8086 /*same as 8088*/{
public: 
        CPU_8086();

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
}

#endif