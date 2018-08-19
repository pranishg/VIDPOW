file(GLOB HEADERS "include/wls/plugins/block_info/*.hpp")

add_library( wls_block_info
             ${HEADERS}
             block_info_plugin.cpp
             block_info_api.cpp
           )

target_link_libraries( wls_block_info wls_app wls_chain wls_protocol fc )
target_include_directories( wls_block_info
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
