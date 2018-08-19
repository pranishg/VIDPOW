file(GLOB HEADERS "include/wls/plugins/changelog/*.hpp")


add_library( wls_changelog
        ${HEADERS}
        changelog_plugin.cpp
        changelog_api.cpp
        )



target_link_libraries( wls_changelog wls_app wls_chain wls_protocol fc )
target_include_directories( wls_changelog
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS wls_changelog
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
       )
INSTALL( FILES ${HEADERS} DESTINATION "include/steemit/plugins/changelog" )
