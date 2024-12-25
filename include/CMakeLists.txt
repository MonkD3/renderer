target_include_directories(
    renderer 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    renderer
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/array.hpp
              ${CMAKE_CURRENT_SOURCE_DIR}/buffer.hpp
              ${CMAKE_CURRENT_SOURCE_DIR}/renderer.hpp
              ${CMAKE_CURRENT_SOURCE_DIR}/scene.hpp
              ${CMAKE_CURRENT_SOURCE_DIR}/shader.hpp
              ${CMAKE_CURRENT_SOURCE_DIR}/window.hpp
)

add_subdirectory(./models)
