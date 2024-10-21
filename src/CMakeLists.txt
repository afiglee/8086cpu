add_library(disassm
            opcode.cpp
            basetypes.cpp
            disassm.cpp
            labels.cpp
            rom.cpp)

set_target_properties(disassm PROPERTIES LINKER_LANGUAGE CXX)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
target_compile_features(disassm PRIVATE cxx_std_17)

#target_link_libraries(disassm ..)




