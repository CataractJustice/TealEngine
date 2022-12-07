cmake_minimum_required(VERSION 3.22.1)

project(TealEngine VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE CPP_SOURCES Debug/*.cpp DevTools/*.cpp EventSystem/*.cpp GameNode/*.cpp Graphics/*.cpp Math/*.cpp Physics/*.cpp System/*.cpp Utils/*.cpp)

add_executable(${PROJECT_NAME} main.cpp Core.cpp Resources.cpp libs/imgui/backends/imgui_impl_opengl3.cpp libs/imgui/backends/imgui_impl_glfw.cpp ${CPP_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ./)


target_include_directories(${PROJECT_NAME} PRIVATE ./libs/)
target_include_directories(${PROJECT_NAME} PRIVATE ./libs/imgui)

find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR})

find_package(GLEW REQUIRED)
target_link_libraries(${MAIN} GLEW::GLEW)

find_package(Freetype REQUIRED)
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})

