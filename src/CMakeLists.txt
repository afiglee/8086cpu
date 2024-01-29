add_library(disassm
            opcode.cpp)

set_target_properties(disassm PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(disassm PRIVATE cxx_std_17)

#target_link_libraries(disassm ..)




