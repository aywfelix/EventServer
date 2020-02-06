#
# FindMariaDB.cmake
# Note: Windows only
#
# Try to find the include directory

# IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
#   SET(PFILES $ENV{ProgramW6432})   
# ELSE()
#   SET(PFILES $ENV{ProgramFiles})   
# ENDIF()


# FIND_PATH(MARIADB_INCLUDE_DIR mysql.h
#     $ENV{MARIADB_INCLUDE_DIR}
#     $ENV{MARIADB_DIR}/include
#     $ENV{MARIADB_DIR}/include/mariadb
#     ${PFILES}/MariaDB/*/include)

# IF(MARIADB_INCLUDE_DIR)
#   MESSAGE(STATUS "Found MariaDB includes: ${MARIADB_INCLUDE_DIR}")
# ENDIF()

# IF(WIN32)
#   SET (LIB_NAME mariadbclient.lib)

# # Try to find mariadb client libraries
#   FIND_PATH(MARIADB_LIBRARY_DIR ${LIB_NAME}
#       $ENV{MARIADB_LIBRARY}
#       ${PFILES}/MariaDB/*/lib
#       $ENV{MARIADB_DIR}/lib/mariadb
#       $ENV{MARIADB_DIR}/lib
#       $ENV{MARIADB_DIR}/libmariadb)

#   IF(MARIADB_LIBRARY)
#     GET_FILENAME_COMPONENT(MARIADB_LIBRARY_DIR ${MARIADB_LIBRARY} PATH)
#   ENDIF()

# ELSE()
  # SET(MARIADB_LIB libmariadbclient.a)
  # FIND_PATH(MARIADB_CONFIG_DIR mariadb_config
  #   $ENV{MARIADB_DIR}/bin
  #   ${MARIADB_DIR}/bin)
  FIND_PROGRAM(MARIADB_CONFIG mariadb_config
    PATHS
    $ENV{MARIADB_DIR}/bin
    ${MARIADB_DIR}/bin
    /opt/netscan/bin)
  IF(MARIADB_CONFIG)
    EXEC_PROGRAM(${MARIADB_CONFIG}
                 ARGS "--version"
                 OUTPUT_VARIABLE MARIADB_VERSION)
    EXEC_PROGRAM(${MARIADB_CONFIG}
                 ARGS "--include"
                 OUTPUT_VARIABLE MY_TMP)
    STRING(REGEX REPLACE "-I([^ ]*)( .*)?" "\\1" MY_TMP "${MY_TMP}")
    SET(MARIADB_INCLUDE_DIR ${MY_TMP} CACHE PATH "MariaDB include directory")
    EXEC_PROGRAM(${MARIADB_CONFIG}
                 ARGS "--libs_r"
                 OUTPUT_VARIABLE MY_TMP)
    SET(MARIADB_LIBRARY ${MY_TMP} CACHE STRING "MariaDB library")
    STRING(SUBSTRING ${MARIADB_LIBRARY} 2 -1 MARIADB_LIBRARY_DIR)
    STRING(FIND ${MARIADB_LIBRARY_DIR} " -l" MY_LENGTH)
    STRING(SUBSTRING ${MARIADB_LIBRARY_DIR} 0 ${MY_LENGTH} MARIADB_LIBRARY_DIR)
    # ADD_DEFINITIONS(${MARIADB_INCLUDE_DIR})
  ENDIF()
# ENDIF()

IF(MARIADB_LIBRARY_DIR AND MARIADB_INCLUDE_DIR)
  MESSAGE(STATUS "MariaDB version: ${MARIADB_VERSION}")
  SET(MARIADB_FOUND TRUE)
ELSE()
  MESSAGE(FATAL_ERROR "Cannot find MariaDB. Include dir: ${MARIADB_INCLUDE_DIR}  library dir: ${MARIADB_LIBRARY_DIR}")
ENDIF()
