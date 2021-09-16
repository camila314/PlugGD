cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(pyedit VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Xlinker --no-demangle")
set(CMAKE_CXX_STANDARD 17)
if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
	add_definitions(-DCC_TARGET_OS_MAC)
	include_directories(
		Cacao/
		Cacao/include
		Cacao/include/cocos2dx
		Cacao/include/cocos2dx/include
		Cacao/include/cocos2dx/kazmath/include
		Cacao/include/cocos2dx/platform/mac
		Cacao/include/cocos2dx/platform/third_party/mac
		Cacao/include/cocos2dx/platform/third_party/mac/OGLES
		Cacao/include/cocos2dext
		Cacao/include/cocos2dx/custom/Sprites/CCMenuItemSpriteExtra
	)
else()

	add_definitions(-DCC_TARGET_OS_WIN32)
	include_directories(
		gd.h/cocos2dx
		gd.h/include
		gd.h/
		cocos-headers/
		cocos-headers/extensions
		cocos-headers/cocos2dx
		cocos-headers/cocos2dx/include
		cocos-headers/cocos2dx/kazmath/include
		cocos-headers/cocos2dx/platform/win32
		cocos-headers/cocos2dx/platform/third_party/win32
		cocos-headers/cocos2dx/platform/third_party/win32/OGLES
		minhook/include
	)
	add_subdirectory(minhook)
endif()

include_directories(
	./pyedit_engine/
	./rapidjson/include
	./src
)

if(APPLE)
	add_subdirectory(Cacao)
endif()


if(WIN32)
	message(WARNING "void p is " ${CMAKE_SIZEOF_VOID_P})
	set(CMAKE_SIZEOF_VOID_P 4)
	set(CMAKE_SYSTEM_PROCESSOR "i686")
	set(CMAKE_GENERATOR_PLATFORM x86)
endif()

find_package (Python3 COMPONENTS Development)

if(NOT Python3_FOUND)
  message(FATAL_ERROR "Can't find python3!")
endif()

if(WIN32)
	set(Python32 0)
	execute_process(
		COMMAND ${Python3_EXECUTABLE} -c "import struct;print(struct.calcsize(\"P\")*8, end=\"\")"
		OUTPUT_VARIABLE Python32)

	if(Python32 EQUAL 64)
		message(FATAL_ERROR "Python must be 32-bit! " ${Python3_LIBRARIES})
	endif()
endif()

add_subdirectory(pyedit_engine)
add_subdirectory(rapidjson)

add_library(${PROJECT_NAME} SHARED 
	src/main.cpp 
	src/ScriptLayer.cpp
	src/ScriptController.cpp
)

target_link_libraries(${PROJECT_NAME} pyedit_engine ${Python3_LIBRARIES})

if(APPLE)
	target_link_directories(${PROJECT_NAME} PRIVATE Cacao)
	target_link_libraries(${PROJECT_NAME} Cacao "-framework CoreServices -framework Cocoa")

	add_custom_command(
		TARGET  ${PROJECT_NAME}
	    COMMAND sudo osxinj \"Geometry Dash\" *.dylib || true
	    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	    COMMENT "Injecting"
	)
else()
	target_link_libraries(${PROJECT_NAME} 
		minhook
		"${CMAKE_SOURCE_DIR}/cocos-headers/cocos2dx/libcocos2d.lib"
		"${CMAKE_SOURCE_DIR}/cocos-headers/extensions/libExtensions.lib"
	)

endif()