cmake_minimum_required(VERSION 3.22.1)

project(TealEngine VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-volatile")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE
SUBDIR_SOURCES
DevTools/*.cpp
FileSystem/*.cpp 
GameNode/*.cpp 
Graphics/*.cpp 
Math/*.cpp 
Physics/*.cpp
System/*.cpp
NlohmannJson/*.cpp 
Utils/*.cpp
Editor/*.cpp
ResourceManagment/*.cpp
)


set(SOURCES ${SUBDIR_SOURCES} 
Core.cpp 
Resources.cpp
Project.cpp
libs/imgui/imgui.cpp
libs/imgui/imgui_tables.cpp
libs/imgui/imgui_draw.cpp
libs/imgui/imgui_widgets.cpp
libs/imgui/backends/imgui_impl_opengl3.cpp 
libs/imgui/backends/imgui_impl_glfw.cpp
libs/imgui/misc/cpp/imgui_stdlib.cpp
libs/tinyfiledialogs.c
)

add_executable(${PROJECT_NAME}Launcher
main.cpp
ComponentExports.cpp
)

add_library(${PROJECT_NAME} SHARED
${SOURCES}
)

add_subdirectory(libs/glfw)
add_subdirectory(libs/freetype2)

set(INCLUDE_DIRS
PUBLIC ./ 
PUBLIC ./libs 
PUBLIC ./libs/imgui 
PUBLIC ./libs/glfw/include 
PUBLIC ./libs/glew-2.2.0/include
PUBLIC ./libs/freetype2/include
PUBLIC ${OPENGL_INCLUDE_DIR}
)

target_include_directories(${PROJECT_NAME} ${INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME}Launcher ${INCLUDE_DIRS})

set(LINK_DIRS
PUBLIC libs/glfw/src
PUBLIC libs/glew/lib
PUBLIC libs/freetype2
)

target_link_directories(${PROJECT_NAME} ${LINK_DIRS})
target_link_directories(${PROJECT_NAME}Launcher ${LINK_DIRS})

set(LINK_LIBS 
${OPENGL_LIBRARY}
PUBLIC glfw
PUBLIC glfw3
PUBLIC GLEW
PUBLIC GL
PUBLIC ${OPENGL_LIBRARY}
PUBLIC freetype
)

target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
target_link_libraries(${PROJECT_NAME}Launcher TealEngine)