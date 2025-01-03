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

add_executable(
    rotate
    ./rotate_3D_test/src.cpp 
)

add_executable(
    tet
    ./hello_tet/src.cpp
)

add_executable(
    balls
    ./moving_balls/src.cpp
)
