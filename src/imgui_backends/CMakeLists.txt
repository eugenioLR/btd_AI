cmake_minimum_required(VERSION 3.10)

project(imgui_backends)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
INCLUDE(FindPkgConfig)
pkg_check_modules(GLFW3 REQUIRED glfw3)


include_directories("${btd_ai_SOURCE_DIR}/include/imgui_backends")

#/-----------------------------------/
#imgui

# Set where the ImGui files are stored
set(IMGUI_PATH  "${btd_ai_SOURCE_DIR}/imgui")

# Compile as static library
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_PATH})

#/-----------------------------------/

add_library(imgui_backends imgui_impl_glfw.cpp imgui_impl_opengl3.cpp)

target_include_directories(imgui_backends PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(imgui_backends PUBLIC ${GLEW_INCLUDE_DIRS})

target_link_libraries(imgui_backends imgui)
target_link_libraries(imgui_backends ${OPENGL_LIBRARIES})
target_link_libraries(imgui_backends ${GLEW_LIBRARIES})
target_link_libraries(imgui_backends ${GLFW3_LIBRARIES})
target_link_libraries(imgui_backends ${CMAKE_DL_LIBS})
