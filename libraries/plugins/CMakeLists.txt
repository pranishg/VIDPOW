
## for each subdirectory containing a CMakeLists.txt, add that subdirectory
#file( GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} * )
#foreach( child ${children} )
#   if( IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${child}" )
#      if( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${child}/CMakeLists.txt" )
#         add_subdirectory( "${child}" )
#      endif()
#   endif()
#endforeach()


###############
# manual
add_subdirectory( account_by_key )
add_subdirectory( account_history )
add_subdirectory( account_statistics )
add_subdirectory( auth_util )
add_subdirectory( block_info )
add_subdirectory( blockchain_statistics )
if( CHANGELOG_PLUGIN )
    add_subdirectory( changelog )
endif()
add_subdirectory( debug_node )
add_subdirectory( delayed_node )
add_subdirectory( follow )
add_subdirectory( private_message )
add_subdirectory( raw_block )
add_subdirectory( tags )
add_subdirectory( witness )
