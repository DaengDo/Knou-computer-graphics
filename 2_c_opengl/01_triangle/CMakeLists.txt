# Add Executable
add_executable(01-triangle-c main.c)
target_link_libraries(01-triangle-c PRIVATE glew glut)

if (WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        add_custom_command(TARGET 01-triangle-c POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/third_party/freeglut/bin/x64/freeglut.dll"
                $<TARGET_FILE_DIR:01-triangle-c>)

        add_custom_command(TARGET 01-triangle-c POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/third_party/glew_win32/bin/Release/x64/glew32.dll"
                $<TARGET_FILE_DIR:01-triangle-c>)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_custom_command(TARGET 01-triangle-c POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/third_party/freeglut/bin/freeglut.dll"
                $<TARGET_FILE_DIR:01-triangle-c>)

        add_custom_command(TARGET 01-triangle-c POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/third_party/glew_win32/bin/Release/Win32/glew32.dll"
                $<TARGET_FILE_DIR:01-triangle-c>)
    endif()
endif()


