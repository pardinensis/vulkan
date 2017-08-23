cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(NAME Vulkan)
project(${NAME})


find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(XCB REQUIRED)

add_definitions(-std=c++14)

set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shader")
file(GLOB SHADERS "${SHADER_DIR}/*.vert" "${SHADER_DIR}/*.frag")
file(GLOB SOURCES *.cpp )

include_directories(${GLFW_INCLUDE_DIRS})

add_executable(${NAME} ${SOURCES} ${SHADERS})

target_link_libraries(${NAME} glfw ${Vulkan_LIBRARY} ${XCB_LIBRARY})
