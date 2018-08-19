add_executable( whaled main.cpp )
if( UNIX AND NOT APPLE )
  set(rt_library rt )
endif()

find_package( Gperftools QUIET )
if( GPERFTOOLS_FOUND )
    message( STATUS "Found gperftools; compiling whaled with TCMalloc")
    list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
endif()

if( STEEM_STATIC_BUILD )
   target_link_libraries( whaled PRIVATE
      "-static-libstdc++ -static-libgcc"
      wls_plugins
      wls_mf_plugins
      wls_app
      wls_witness
      wls_account_history
      wls_chain
      wls_protocol
      fc
      ${ROCKSDB_LIBRARY}
      ${ZSTD_LIBRARIES}
      ${SNAPPY_LIBRARIES}
      ${LZ4_LIBRARY}
      ${CMAKE_DL_LIBS}
      ${PLATFORM_SPECIFIC_LIBS}
   )
else( STEEM_STATIC_BUILD )
   target_link_libraries( whaled PRIVATE
      wls_plugins
      wls_mf_plugins
      wls_app
      wls_witness
      wls_account_history
      wls_chain
      wls_protocol
      fc
      ${ROCKSDB_LIBRARY}
      ${ZSTD_LIBRARIES}
      ${SNAPPY_LIBRARIES}
      ${LZ4_LIBRARY}
      ${CMAKE_DL_LIBS}
      ${PLATFORM_SPECIFIC_LIBS}
   )
endif( STEEM_STATIC_BUILD )

install( TARGETS
   whaled

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
