target_sources( 
    renderer
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/model.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/trimesh.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/balls.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/lines.cpp"
)
