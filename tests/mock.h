#ifndef _MOCK_H__
#define _MOCK_H__

#include <gmock/gmock.h>
#include "disassm.h"

namespace sim86 {

class MockDisassm : public Disassm {
public:    
    MOCK_METHOD5(modregrm, pOpCode(const uint8_t&, const bstring&, size_t&, bool, enum PREFIX));
};
}

#endif
