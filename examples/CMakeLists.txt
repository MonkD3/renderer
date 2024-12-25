link_libraries(renderer)

add_executable(
    hello_triangle
    ./hello_triangle/src.cpp
)

add_executable(
    hello_mesh
    ./hello_mesh/src.cpp
)

add_executable(
    function_visualisation
    ./function_visualisation/src.cpp
)
add_executable(
    colormap
    ./colormaps/src.cpp
)
add_executable(
    advect
    ./advection/src.cpp
)
