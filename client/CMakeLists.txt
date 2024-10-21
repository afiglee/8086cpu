add_executable(cpurun 
    main.cpp)

target_compile_features(cpurun PRIVATE cxx_std_17)
target_include_directories(cpurun PRIVATE . ../src/)
target_link_libraries(cpurun disassm)