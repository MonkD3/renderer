target_sources( 
    renderer
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/array.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/buffer.cpp" 
            "${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/window.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/colormaps.cpp"
)

add_subdirectory(./models)
