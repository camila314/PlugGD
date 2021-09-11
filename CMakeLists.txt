cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(pyedit VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -std=c++17 -DCC_TARGET_OS_MAC -fms-extensions -Xlinker --no-demangle")


include_directories(
	/usr/local/lib/Cacao/
	/usr/local/lib/Cacao/include
	/usr/local/lib/Cacao/include/cocos2dx
	/usr/local/lib/Cacao/include/cocos2dx/include
	/usr/local/lib/Cacao/include/cocos2dx/kazmath/include
	/usr/local/lib/Cacao/include/cocos2dx/platform/mac
	/usr/local/lib/Cacao/include/cocos2dx/platform/third_party/mac
	/usr/local/lib/Cacao/include/cocos2dx/platform/third_party/mac/OGLES
	/usr/local/lib/Cacao/include/cocos2dext
	/usr/local/lib/Cacao/include/cocos2dx/custom/Sprites/CCMenuItemSpriteExtra
	./pyedit_engine/
	./rapidjson/include
	.
)

add_subdirectory(Cacao)
add_subdirectory(pyedit_engine)
add_subdirectory(rapidjson)

add_library(${PROJECT_NAME} SHARED 
	main.cpp 
	ScriptLayer.cpp
	ScriptController.cpp
)
target_link_directories(${PROJECT_NAME} PRIVATE 
	Cacao
	/usr/local/opt/python@3.9/Frameworks/Python.framework/Versions/3.9/lib/python3.9/config-3.9-darwin
)
target_link_libraries(${PROJECT_NAME} Cacao "-framework CoreServices" pyedit_engine python3.9 "-framework Cocoa")

add_custom_command(
	TARGET  ${PROJECT_NAME}
    COMMAND sudo osxinj \"Geometry Dash\" *.dylib || true
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Injecting"
)
