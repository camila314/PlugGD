cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(PlugGD VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Xlinker --no-demangle")
set(CMAKE_CXX_STANDARD 17)
if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -rpath \"@loader_path\"")
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
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Out")
	set(CMAKE_BUILD_TYPE RelWithDebInfo)
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
	add_subdirectory(minhook EXCLUDE_FROM_ALL)
	add_subdirectory(w32bootstrap)
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

add_subdirectory(pyedit_engine)
add_subdirectory(rapidjson EXCLUDE_FROM_ALL)

add_library(${PROJECT_NAME} SHARED 
	src/main.cpp 
	src/ScriptLayer.cpp
	src/ScriptController.cpp
)


if(APPLE)
	set(PyEmbed ${CMAKE_BINARY_DIR}/PyMac/lib)
	file(COPY PyMac DESTINATION ./)
	message(WRNING ${PyEmbed})
else()
	set(PyEmbed ${CMAKE_SOURCE_DIR}/Pyw32)
	#file(COPY Pyw32 DESTINATION ./)
endif()

target_link_directories(${PROJECT_NAME} PRIVATE ${PyEmbed})

if(APPLE)
	target_link_directories(${PROJECT_NAME} PRIVATE Cacao)
	target_link_libraries(${PROJECT_NAME} Cacao python3.8 pyedit_engine "-framework CoreServices -framework Cocoa")

	#add_custom_command(
	#	TARGET ${PROJECT_NAME}
	#	COMMAND install_name_tool -change /Users/jakrillis/projects/pyedit/PyMac/lib/libpython3.8.dylib @loader_path/PyMac/lib/libpython3.8.dylib lib${PROJECT_NAME}.dylib
	#	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	#)

	add_custom_command(
		TARGET  ${PROJECT_NAME}
	    COMMAND sudo osxinj \"Geometry Dash\" *.dylib || true
	    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	    COMMENT "Injecting"
	)
else()
	target_link_libraries(${PROJECT_NAME} 
		minhook
		python38
		pyedit_engine
		"${CMAKE_SOURCE_DIR}/cocos-headers/cocos2dx/libcocos2d.lib"
		"${CMAKE_SOURCE_DIR}/cocos-headers/extensions/libExtensions.lib"
	)

	install(TARGETS ${PROJECT_NAME} 
	DESTINATION ${CMAKE_BINARY_DIR}/out
	)
	install(DIRECTORY Pyw32 DESTINATION ${CMAKE_BINARY_DIR}/out
		PATTERN "site-packages/Cython/" EXCLUDE
		)
endif()