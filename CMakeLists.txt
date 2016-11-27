cmake_minimum_required(VERSION 2.8)

project(sproto)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native ")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPy_BUILD_CORE ")

message(PROJECT_SOURCE_DIR =  ${PROJECT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(
	D:/SDK/Python-3.5.2/Include
)

link_directories(
	D:/SDK/Python-3.5.2/PCbuild/amd64
)
#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/res)

add_library(${PROJECT_NAME} SHARED
	clib/msvcint.h
	clib/sproto.c
	clib/sproto.h
	pysproto.c
)

target_link_libraries(${PROJECT_NAME}
	#${PROJECT_SOURCE_DIR}/Python35/libs/python3_d.lib
)


# Build examples

add_executable(TestSproto
	clib/msvcint.h
	clib/sproto.c
	clib/sproto.h
	pysproto.c
	testmain.cpp
)

target_link_libraries(TestSproto ${PROJECT_NAME})
