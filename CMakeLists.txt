cmake_minimum_required(VERSION 3.22.1)

project(TealEngine VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-volatile -g3 -fpermissive")
if(NOT CMAKE_CXX_COMPILER MATCHES "x86_64-w64-mingw32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

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
Network/*.cpp 
Utils/*.cpp
Editor/*.cpp
ResourceManagment/*.cpp
Components/*.cpp
)

file(GLOB_RECURSE TEST_SOURCES Tests/*.cpp)

set(SOURCES ${SUBDIR_SOURCES} 
Core.cpp 
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


file(GLOB_RECURSE KDSOURCES Kirino/*.cpp)
add_library(Kirino SHARED ${KDSOURCES})

add_executable(${PROJECT_NAME}Tests ${TEST_SOURCES})

add_library(${PROJECT_NAME} SHARED
${SOURCES}
)

add_subdirectory(libs/glfw)
add_subdirectory(libs/freetype2)
add_subdirectory(libs/enet)

set(INCLUDE_DIRS
PUBLIC ./ 
PUBLIC ./libs 
PUBLIC ./libs/imgui 
PUBLIC ./libs/glfw/include 
PUBLIC ./libs/glew-2.2.0/include
PUBLIC ./libs/freetype2/include
PUBLIC ./libs/enet/include
)

target_include_directories(${PROJECT_NAME} ${INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME}Launcher ${INCLUDE_DIRS})
target_include_directories(Kirino ${INCLUDE_DIRS})


set(LINK_DIRS
PUBLIC libs/glew-2.2.0/lib-win
PUBLIC libs/freetype2
PUBLIC libs/enet
)

target_link_directories(${PROJECT_NAME} ${LINK_DIRS})
target_link_directories(${PROJECT_NAME}Launcher ${LINK_DIRS})
target_link_directories(${PROJECT_NAME}Tests ${LINK_DIRS})
target_link_directories(Kirino ${LINK_DIRS})

find_package(OpenGL REQUIRED)
if(MINGW)
target_link_libraries(Kirino
    PRIVATE "${CMAKE_SOURCE_DIR}/libs/glew-2.2.0/lib-win/libglew32.dll.a"
)
target_link_libraries(TealEngine
    PRIVATE "${CMAKE_SOURCE_DIR}/libs/glew-2.2.0/lib-win/libglew32.dll.a"
)
endif()

set(LINK_LIBS 
PUBLIC ${OPENGL_LIBRARY}
PUBLIC glfw
PUBLIC freetype
PUBLIC enet
PUBLIC OpenGL::GL
PUBLIC freetype
)

if(MINGW)
target_link_options(${PROJECT_NAME} PRIVATE -Wl,--stack,16777216)
target_link_options(${PROJECT_NAME}Launcher PRIVATE -Wl,--stack,16777216)
target_link_options(Kirino PRIVATE -Wl,--stack,16777216)
else()
set(LINK_LIBS ${LINK_LIBS} PUBLIC GLEW)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC Kirino ${LINK_LIBS})
add_dependencies(${PROJECT_NAME}Launcher TealEngine)
target_link_libraries(${PROJECT_NAME}Launcher PRIVATE TealEngine)
target_link_libraries(Kirino ${LINK_LIBS})
target_link_libraries(${PROJECT_NAME}Tests TealEngine)


set_target_properties(${PROJECT_NAME} PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
set_target_properties(Kirino PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)