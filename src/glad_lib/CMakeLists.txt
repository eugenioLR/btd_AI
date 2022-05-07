cmake_minimum_required(VERSION 3.10)

project(glad_lib)

include_directories("${btd_ai_SOURCE_DIR}/include/glad")

add_library(glad_lib glad.c)
