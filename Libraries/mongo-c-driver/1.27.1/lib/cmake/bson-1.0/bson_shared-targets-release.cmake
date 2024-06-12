#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mongo::bson_shared" for configuration "Release"
set_property(TARGET mongo::bson_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(mongo::bson_shared PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libbson-1.0.0.0.0.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libbson-1.0.0.dylib"
  )

list(APPEND _cmake_import_check_targets mongo::bson_shared )
list(APPEND _cmake_import_check_files_for_mongo::bson_shared "${_IMPORT_PREFIX}/lib/libbson-1.0.0.0.0.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
