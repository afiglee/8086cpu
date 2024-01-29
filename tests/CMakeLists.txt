add_executable(disassm.t
               opcode0_test.cpp
               opcode1_test.cpp
               opcode2_test.cpp
               opcode3_test.cpp
               opcodeTwo_test.cpp
               opcodeThree_test.cpp
               opcodeSingle_test.cpp
               test_main.cpp)
target_compile_features(disassm.t PRIVATE cxx_std_17)               
target_include_directories(disassm.t PRIVATE . ../src/ /opt/homebrew/include/)

target_link_libraries(disassm.t disassm gtest)
target_link_directories(disassm.t PRIVATE /opt/homebrew/lib/)

add_test(NAME disassm.t 
         COMMAND $<TARGET_FILE:disassm.t> --gtest-output=xml:disassm.t.xunit.xml)
