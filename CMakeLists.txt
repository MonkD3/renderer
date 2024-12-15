cmake_minimum_required(
    VERSION 3.10
)

project(
    Renderer
    CXX
    C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(BUILD_SHARED_LIBS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DVERBOSE=5)
endif()

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

add_subdirectory(./glad/)

aux_source_directory(
    ./src/ RENDERER_SRC
)
add_library(renderer SHARED ${RENDERER_SRC})
target_include_directories(
    renderer PUBLIC include
)

target_compile_options(
    renderer PRIVATE
    -Wall -Werror -Wextra
)

target_link_libraries(renderer PUBLIC glfw OpenGL glad)

install(
    TARGETS renderer
)

add_subdirectory(./examples/)
