add_executable(disassm.t
               opcode_test.cpp
               test_main.cpp)
target_compile_features(disassm.t PRIVATE cxx_std_17)               
target_include_directories(disassm.t PRIVATE . /opt/homebrew/include/)

target_link_libraries(disassm.t gtest)
target_link_directories(disassm.t PRIVATE /opt/homebrew/lib/)

add_test(NAME disassm.t 
         COMMAND $<TARGET_FILE:disassm.t> --gtest-output=xml:disassm.t.xunit.xml)
