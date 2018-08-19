file(GLOB HEADERS "include/wls/delayed_node/*.hpp")

add_library( wls_delayed_node
             delayed_node_plugin.cpp
           )

target_link_libraries( wls_delayed_node wls_chain wls_protocol wls_app )
target_include_directories( wls_delayed_node
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if(MSVC)
  set_source_files_properties( delayed_node_plugin.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

install( TARGETS
   wls_delayed_node

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
