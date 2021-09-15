cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(pyedit_engine VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Xlinker --no-demangle -g")
set(CMAKE_CXX_STANDARD 17)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )

if(WIN32)
	set(CMAKE_SIZEOF_VOID_P 4)
	set(CMAKE_SYSTEM_PROCESSOR "i686")
	set(CMAKE_GENERATOR_PLATFORM x86)
endif()

find_package (Python3 COMPONENTS Development)
include (FindCython)

if(NOT CYTHON_EXECUTABLE)
  message(FATAL_ERROR "Can't find cython!")
endif()

if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
	add_definitions(-DCC_TARGET_OS_MAC)
else()
	add_definitions(-DCC_TARGET_OS_WIN32)
endif()

add_custom_command(
	COMMAND ${CYTHON_EXECUTABLE} --cplus ${CMAKE_CURRENT_SOURCE_DIR}/cy/main.pyx
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cy/main.pyx
	OUTPUT cy/main.h
	OUTPUT cy/main.cpp
	COMMENT "Generating headers"
)

add_library(${PROJECT_NAME} STATIC 
	engine.cpp
	thread_control.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/cy/main.cpp
	helper_hooks.cpp
)


add_custom_target(CyBindings ALL
   DEPENDS cy/main.h
   DEPENDS cy/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${Python3_INCLUDE_DIRS}
)

if (WIN32)
target_include_directories(${PROJECT_NAME} PRIVATE
	..//gd.h
)
endif()

ADD_DEPENDENCIES(pyedit_engine CyBindings)