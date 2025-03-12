cmake_minimum_required(VERSION 3.12)

project(knou-computer-graphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party/glfw)
add_library(glad third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)

add_executable(MyApp "./test.cpp")
target_link_libraries(MyApp PRIVATE glfw glad)