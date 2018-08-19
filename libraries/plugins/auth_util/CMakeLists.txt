file(GLOB HEADERS "include/wls/plugins/auth_util/*.hpp")

add_library( wls_auth_util
             ${HEADERS}
             auth_util_plugin.cpp
             auth_util_api.cpp
           )

target_link_libraries( wls_auth_util wls_app wls_chain wls_protocol fc )
target_include_directories( wls_auth_util
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
