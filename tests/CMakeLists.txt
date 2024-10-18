add_executable(disassm.t
               #opcode0_test.cpp
               #opcode1_test.cpp
               #opcode2_test.cpp
               #opcode3_test.cpp
               #opcodeTwo_test.cpp
               #opcodeThree_test.cpp
               #opcodeSingle_test.cpp
               #opcodeMulti_test.cpp
               disassm_test.cpp
               test_main.cpp)
target_compile_features(disassm.t PRIVATE cxx_std_17)  
message(NOTICE "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")   
    if(DEFINED ENV{<HOMEBREW_PREFIX>})        
        target_include_directories(disassm.t PRIVATE . ../src/ ${HOMEBREW_PREFIX}/include/)
    else()
        #execute_process(COMMAND brew --prefix googletest OUTPUT_VARIABLE GOOGLETEST_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
        #message("GOOGLETEST_PREFIX=${GOOGLETEST_PREFIX}/")
        #target_include_directories(disassm.t PRIVATE . ../src/ ${GOOGLETEST_PREFIX}/include/)
        target_include_directories(disassm.t PRIVATE ../src/ /usr/local/include/)
        target_link_directories(disassm.t PRIVATE /usr/local/lib/)
    endif()
else()
    target_include_directories(disassm.t PRIVATE . ../src/)
endif()

target_link_libraries(disassm.t disassm gtest gmock)
target_link_directories(disassm.t PRIVATE ${GOOGLETEST_PREFIX}/lib/)

add_test(NAME disassm.t 
         COMMAND disassm.t)
