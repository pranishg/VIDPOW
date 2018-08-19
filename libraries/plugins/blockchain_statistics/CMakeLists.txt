file(GLOB HEADERS "include/wls/blockchain_statistics/*.hpp")

add_library( wls_blockchain_statistics
             blockchain_statistics_plugin.cpp
             blockchain_statistics_api.cpp
           )

target_link_libraries( wls_blockchain_statistics wls_chain wls_protocol wls_app )
target_include_directories( wls_blockchain_statistics
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   wls_blockchain_statistics

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)