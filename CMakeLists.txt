cmake_minimum_required(VERSION 3.10)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")


project(btd_ai VERSION 0.1)

configure_file(btd_aiConfig.h.in btd_aiConfig.h)

include_directories(include)

add_subdirectory(src)

add_executable(btd_ai main.cpp)

target_link_libraries(btd_ai btd_ai_classes)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
