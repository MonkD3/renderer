cmake_minimum_required(VERSION 3.13)

project(Renderer
    CXX
    C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(BUILD_SHARED_LIBS ON)

# Compile the examples
option(ENABLE_EXAMPLES OFF)

find_package(
    glfw
    NAMES glfw GLFW glfw3
    REQUIRED
)

find_library(
    OpenGL
    NAMES OpenGL
    REQUIRED
)

find_package( 
    glm 
    CONFIG
    REQUIRED
)

add_subdirectory(./deps/glad/)

# Initialize an empty string for the source files
add_library(renderer SHARED)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(
    renderer PRIVATE
    -Wall -Werror -Wextra -DASSET_DIR="${Renderer_SOURCE_DIR}/assets/"
)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(
        renderer PRIVATE 
        -DVERBOSE=5
        -g
        -fsanitize=address
    )
endif()

target_link_libraries(renderer PUBLIC glfw glm::glm OpenGL glad)

install(
    TARGETS renderer
)

if (ENABLE_EXAMPLES)
    message("========= Compiling examples ==========")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/)

    # For examples in development or simple tests that should not be 
    # committed yet
    if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/playground/)
        message("========= Compiling playground ==========")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/playground/)
    endif()
endif()
