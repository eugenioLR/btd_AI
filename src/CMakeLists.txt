cmake_minimum_required(VERSION 3.10)

project(btd_ai_classes)

find_package(PkgConfig REQUIRED)
INCLUDE(FindPkgConfig)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
pkg_check_modules(LIBPNG REQUIRED libpng)

include_directories("${btd_ai_SOURCE_DIR}/include")

add_subdirectory(opengl_classes)
add_subdirectory(imgui_backends)
add_subdirectory(glad_lib)
#add_subdirectory(AI)

add_library(btd_ai_classes Game.cpp Bloon.cpp Tower.cpp Projectile.cpp RoundGenerator.cpp MapLayout.cpp)

#target_link_libraries(btd_ai_classes opengl_classes imgui_backends glad_lib AI)
target_link_libraries(btd_ai_classes opengl_classes imgui_backends glad_lib)

target_include_directories(btd_ai_classes PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(btd_ai_classes PUBLIC ${GLEW_INCLUDE_DIRS})

target_link_libraries(btd_ai_classes imgui)
target_link_libraries(btd_ai_classes ${JSONCPP_LIBRARIES})
target_link_libraries(btd_ai_classes ${LIBPNG_LIBRARIES})
target_link_libraries(btd_ai_classes ${CMAKE_DL_LIBS})
