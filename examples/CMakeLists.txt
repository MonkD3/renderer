link_libraries(renderer)

add_executable(
    001
    ./001-simple-triangle/src.cpp
)

add_executable(
    002
    ./002-mesh-triangle/src.cpp
)

add_executable(
    003
    ./003-mesh3D-triangle/src.cpp
)

add_executable(
    004
    ./004-ball3D-simple/src.cpp
)

add_executable(
    005
    ./005-ball3D-moving/src.cpp
)

add_executable(
    test
    ./test-ball-rendering/src.cpp
)

add_executable(
    teapot
    ./utah-teapot/src.cpp
)

add_executable(
    blend
    ./test-blend/src.cpp
)

add_executable(
    uniform
    ./test-uniform-blocks/src.cpp
)
