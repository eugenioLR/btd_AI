cmake_minimum_required(VERSION 3.10)

project(AI)

include_directories("${btd_ai_SOURCE_DIR}/include/AI")

add_library(AI Actor.cpp EvolAI.cpp)

target_link_libraries(AI imgui)