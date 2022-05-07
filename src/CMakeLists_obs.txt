cmake_minimum_required(VERSION 3.10)

project(btd_ai_classes)

find_package(Eigen3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
INCLUDE(FindPkgConfig)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(LIBPNG REQUIRED libpng)


include_directories("${btd_ai_SOURCE_DIR}/include")
include_directories(${jsoncpp_INCLUDE_DIRS})
link_libraries(${JSONCPP_LIBRARIES})

#/-----------------------------------/
#imgui

# Set where the ImGui files are stored
set(IMGUI_PATH  "../imgui")

# Compile as static library
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_PATH})

#/-----------------------------------/

add_library(btd_ai_classes imgui_impl_glfw.cpp imgui_impl_opengl3.cpp Game.cpp Bloon.cpp Monkey.cpp Projectile.cpp MapLayout.cpp glad.c Shader.cpp Texture.cpp SpriteRenderer.cpp ResourceManager.cpp)

target_include_directories(btd_ai_classes PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(btd_ai_classes PUBLIC ${GLEW_INCLUDE_DIRS})

target_link_libraries(btd_ai_classes ${JSONCPP_LIBRARIES})
target_link_libraries(btd_ai_classes Eigen3::Eigen)
target_link_libraries(btd_ai_classes imgui)
target_link_libraries(btd_ai_classes ${OPENGL_LIBRARIES})
target_link_libraries(btd_ai_classes ${GLEW_LIBRARIES})
target_link_libraries(btd_ai_classes ${LIBPNG_LIBRARIES})
target_link_libraries(btd_ai_classes ${GLFW3_LIBRARIES})
target_link_libraries(btd_ai_classes ${CMAKE_DL_LIBS})
