# - Find zstd
# Find the zstd compression library and includes
#
# ZSTD_INCLUDE_DIR - where to find zstd.h, etc.
# ZSTD_LIBRARIES - List of libraries when using zstd.
# ZSTD_FOUND - True if zstd found.

find_path(ZSTD_INCLUDE_DIR
  NAMES zstd.h
  HINTS ${ZSTD_ROOT_DIR}/include)

find_library(ZSTD_LIBRARIES
  #NAMES zstd
  NAMES libzstd.a
  HINTS ${ZSTD_ROOT_DIR}/lib) #/usr/lib/x86_64-linux-gnu

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(zstd DEFAULT_MSG ZSTD_LIBRARIES ZSTD_INCLUDE_DIR)

mark_as_advanced(
  ZSTD_LIBRARIES
  ZSTD_INCLUDE_DIR)

if(ZSTD_FOUND)
    add_library(zstd STATIC IMPORTED)
    set_property(TARGET zstd PROPERTY IMPORTED_LOCATION ${ZSTD_LIBRARIES})
endif()