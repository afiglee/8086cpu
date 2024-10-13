cmake_minimum_required(VERSION 3.25)

project(disassm VERSION 1.0 LANGUAGES CXX)

enable_testing()

add_subdirectory(src)
add_subdirectory(client)
add_subdirectory(tests)

