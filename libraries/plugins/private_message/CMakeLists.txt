file(GLOB HEADERS "include/wls/private_message/*.hpp")

add_library( wls_private_message
             private_message_plugin.cpp
           )

target_link_libraries( wls_private_message wls_chain wls_protocol wls_app )
target_include_directories( wls_private_message
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   wls_private_message

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
