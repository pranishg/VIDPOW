file(GLOB HEADERS "include/wls/account_by_key/*.hpp")

add_library( wls_account_by_key
             account_by_key_plugin.cpp
             account_by_key_api.cpp
           )

target_link_libraries( wls_account_by_key wls_chain wls_protocol wls_app )
target_include_directories( wls_account_by_key
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   wls_account_by_key

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
