cmake_minimum_required(VERSION 3.10)

project(opengl_classes)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
INCLUDE(FindPkgConfig)
pkg_check_modules(GLFW3 REQUIRED glfw3)

include_directories("${btd_ai_SOURCE_DIR}/include/opengl_classes")

add_library(opengl_classes Shader.cpp Texture.cpp SpriteRenderer.cpp ResourceManager.cpp)

target_include_directories(opengl_classes PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(opengl_classes PUBLIC ${GLEW_INCLUDE_DIRS})

target_link_libraries(opengl_classes ${JSONCPP_LIBRARIES})
target_link_libraries(opengl_classes imgui)
target_link_libraries(opengl_classes ${OPENGL_LIBRARIES})
target_link_libraries(opengl_classes ${GLEW_LIBRARIES})
target_link_libraries(opengl_classes ${GLFW3_LIBRARIES})
target_link_libraries(opengl_classes ${CMAKE_DL_LIBS})
