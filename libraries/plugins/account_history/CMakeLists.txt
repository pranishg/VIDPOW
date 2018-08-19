file(GLOB HEADERS "include/wls/account_history/*.hpp")

add_library( wls_account_history
             account_history_plugin.cpp
           )

target_link_libraries( wls_account_history wls_chain wls_protocol wls_app )
target_include_directories( wls_account_history
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   wls_account_history

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
