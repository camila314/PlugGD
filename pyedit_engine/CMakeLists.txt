set(SDK_LOCATION $ENV{CACAO_SDK_LOCATION}) # pls work pls work

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(pyedit_engine VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -std=c++17 -DCC_TARGET_OS_MAC -mmacosx-version-min=10.7 -fms-extensions -Xlinker --no-demangle -g -isysroot ${SDK_LOCATION}")
#set(CMAKE_OSX_SYSROOT ${SDK_LOCATION})

add_custom_command(
	COMMAND cython --cplus ${CMAKE_CURRENT_SOURCE_DIR}/cy/main.pyx
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cy/main.pyx
	OUTPUT cy/main.h
	OUTPUT cy/main.cpp
	COMMENT "Generating headers"
)


add_library(${PROJECT_NAME} STATIC 
	engine.mm 
	thread_control.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/cy/main.cpp
	helper_hooks.mm
)


add_custom_target(CyBindings ALL
   DEPENDS cy/main.h
   DEPENDS cy/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	/usr/local/opt/python@3.9/Frameworks/Python.framework/Versions/3.9/include/python3.9
)

ADD_DEPENDENCIES(pyedit_engine CyBindings)