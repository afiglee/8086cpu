#ifndef _LABELS_H__
#define _LABELS_H__

#include <string>
#include <map>

namespace sim86{

class LabelMap {
    public:
    enum LABEL_TYPE {E_LABEL_ANY, E_LABEL_JMP, E_LABEL_CALL};
        LabelMap() = default;
        std::string get_label(uint32_t addr, enum LABEL_TYPE eType = E_LABEL_JMP) {
            auto it = m_labels.find(addr);
            if (it == m_labels.end()) {
                std::string lbl;
                switch (eType) {
                    case E_LABEL_JMP:
                        lbl = std::string{"jmp" + std::to_string(++d_jmpIndx)};
                        break;
                    case E_LABEL_CALL:
                        lbl = std::string{"subr" + std::to_string(++d_callIndx)};
                        break;
                    default:
                        lbl = std::string{"lbl" + std::to_string(++d_anyIndx)};  
                }
                m_labels[addr] = lbl;
                return lbl;  
            }
            return it->second;
        }
    protected:
        uint16_t d_anyIndx;
        uint16_t d_jmpIndx;
        uint16_t d_callIndx;        
        std::map<uint32_t, std::string> m_labels;    
};
}

#endif
