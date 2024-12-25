cmake_minimum_required(VERSION 3.10)

project(Renderer
    CXX
    C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(BUILD_SHARED_LIBS ON)

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
    NAMES glm
    REQUIRED
)

add_subdirectory(./glad/)

# Initialize an empty string for the source files
add_library(renderer SHARED)
add_subdirectory(./src)
add_subdirectory(./include)

target_compile_options(
    renderer PRIVATE
    -Wall -Werror -Wextra -DCMAKE_HOME_DIRECTORY="${CMAKE_HOME_DIRECTORY}"
)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(
        renderer PRIVATE 
        -DVERBOSE=5
    )
endif()


target_link_libraries(renderer PUBLIC glfw glm OpenGL glad)

install(
    TARGETS renderer
)

add_subdirectory(./examples/)
