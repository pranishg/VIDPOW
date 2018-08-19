file(GLOB HEADERS "include/wls/tags/*.hpp")

add_library( wls_tags
             tags_plugin.cpp )

target_link_libraries( wls_tags wls_chain wls_protocol wls_app )
target_include_directories( wls_tags
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   wls_tags

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
