file(GLOB HEADERS "include/wls/account_statistics/*.hpp")

add_library( wls_account_statistics
             account_statistics_plugin.cpp
             account_statistics_api.cpp
           )

target_link_libraries( wls_account_statistics wls_chain wls_protocol wls_app )
target_include_directories( wls_account_statistics
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   wls_account_statistics

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)