file(GLOB HEADERS "include/wls/plugins/debug_node/*.hpp")

add_library( wls_debug_node
             ${HEADERS}
             debug_node_plugin.cpp
             debug_node_api.cpp
           )

target_link_libraries( wls_debug_node wls_app wls_chain wls_protocol fc )
target_include_directories( wls_debug_node
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS wls_debug_node
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
       )
INSTALL( FILES ${HEADERS} DESTINATION "include/steemit/plugins/debug_node" )
