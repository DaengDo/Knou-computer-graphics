cmake_minimum_required(VERSION 3.12)

project(knou-computer-graphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW
add_subdirectory(third_party/glfw)

# GLAD
add_library(glad third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)

# GLUT
add_library(glut STATIC IMPORTED)
set_target_properties(glut PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/freeglut/lib/freeglut.lib"
)
target_include_directories(glut INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/third_party/freeglut/include"
)

# GLEW
add_library(glew STATIC IMPORTED)
set_target_properties(glew PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/glew_win32/lib/Release/Win32/glew32.lib"
)
target_include_directories(glew INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/third_party/glew_win32/include"
)

# cpp opengl
add_subdirectory("1_cpp_opengl/01_triangle")

# c opengl
add_subdirectory("2_c_opengl/01_triangle")
