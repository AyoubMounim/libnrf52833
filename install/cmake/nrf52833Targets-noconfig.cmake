#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "::nrf52833" for configuration ""
set_property(TARGET ::nrf52833 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(::nrf52833 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libnrf52833.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ::nrf52833 )
list(APPEND _IMPORT_CHECK_FILES_FOR_::nrf52833 "${_IMPORT_PREFIX}/lib/libnrf52833.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
