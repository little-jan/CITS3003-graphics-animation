cmake_minimum_required(VERSION 3.12)
project(cits3003_labs)

set(CMAKE_CXX_STANDARD 17)

# GLAD
add_subdirectory(lib/glad)
# end GLAD

# GLFW
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

# glm
add_subdirectory(lib/glm)
#end glm

# Function to configure dependencies and post-build steps for a lab executable
function(configure_lab_target TARGET_NAME)
    target_link_libraries(${TARGET_NAME} glfw glad glm)

    if (APPLE)
        # "-framework OpenGL"
        target_link_libraries(${TARGET_NAME} "-framework Cocoa" "-framework IOKit")
    endif()

    # Copy executable post build
    add_custom_command(TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET_NAME}> ${CMAKE_SOURCE_DIR})

    if (WIN32)
        # Due to symbolic links requiring higher privileges on Windows, manually create Junctions instead
        file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/res" _dstResDir)
        file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/res" _srcResDir)

        file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/config" _dstConfigDir)
        file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/config" _srcConfigDir)

        # Link the res and config dirs into where the build puts the exe
        add_custom_command(TARGET ${TARGET_NAME}
                POST_BUILD
                COMMAND cmd.exe /c if not exist "${_dstResDir}" mklink /J "${_dstResDir}" "${_srcResDir}"
                COMMAND cmd.exe /c if not exist "${_dstConfigDir}" mklink /J "${_dstConfigDir}" "${_srcConfigDir}")
    else()
        # Link the res and config dirs into where the build puts the exe
        # Note: create_symlink will fail if the link already exists, so we allow it to fail or just rely on it working once.
        # Ideally we might checking existence, but CMake's create_symlink is simple.
        add_custom_command(TARGET ${TARGET_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
                COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/config ${CMAKE_BINARY_DIR}/config
        )
    endif()
endfunction()

# LAB 1 ==============================================================================================
add_executable(lab1_example1 lab1/example1.cpp helpers/ShaderHelper.cpp)
configure_lab_target(lab1_example1)

add_executable(lab1_example2 lab1/example2.cpp helpers/ShaderHelper.cpp)
configure_lab_target(lab1_example2)

add_executable(lab1_q1circle lab1/q1circle.cpp helpers/ShaderHelper.cpp)
configure_lab_target(lab1_q1circle)