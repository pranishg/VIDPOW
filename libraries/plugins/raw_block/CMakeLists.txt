file(GLOB HEADERS "include/wls/plugins/raw_block/*.hpp")

add_library( wls_raw_block
             ${HEADERS}
             raw_block_plugin.cpp
             raw_block_api.cpp
           )

target_link_libraries( wls_raw_block wls_app wls_chain wls_protocol fc )
target_include_directories( wls_raw_block
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
