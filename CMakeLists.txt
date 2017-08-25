cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(NAME Vulkan)
project(${NAME})


set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/source")
set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shader")
set(CONTRIB_DIR "${CMAKE_SOURCE_DIR}/contrib")

find_package(glfw3 3.2 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(XCB REQUIRED)
find_package(assimp REQUIRED)

add_definitions(-std=c++14)

include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${CONTRIB_DIR})

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp )
add_executable(${NAME} ${SOURCES} ${SHADERS})
target_link_libraries(${NAME} glfw ${Vulkan_LIBRARY} ${XCB_LIBRARY})

# compile shaders
file(GLOB SHADERS "${SHADER_DIR}/*.vert" "${SHADER_DIR}/*.frag")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/shader")

set(COMPILED_SHADERS)
foreach(_file ${SHADERS})
	file (RELATIVE_PATH _file_rel "${CMAKE_SOURCE_DIR}" "${_file}")
	string(REPLACE "." "_" _spv ${_file_rel})
	set(_spv "${CMAKE_BINARY_DIR}/${_spv}.spv")
	add_custom_command(
		OUTPUT ${_spv}
		COMMAND ${CMAKE_SOURCE_DIR}/tools/glslangValidator -V ${_file} -o ${_spv}
		DEPENDS ${_file}
	)
	list (APPEND COMPILED_SHADERS "${_spv}")
endforeach(_file)
add_custom_target(shaders DEPENDS ${COMPILED_SHADERS})
add_dependencies(${NAME} shaders)
