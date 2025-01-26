target_include_directories(
    renderer 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    renderer
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/model.hpp
              ${CMAKE_CURRENT_SOURCE_DIR}/trimesh.hpp
              ${CMAKE_CURRENT_SOURCE_DIR}/balls.hpp
              ${CMAKE_CURRENT_SOURCE_DIR}/lines.hpp
)

