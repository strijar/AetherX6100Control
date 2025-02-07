if(NOT UNIX)
  message(FATAL_ERROR "${PROJECT_NAME} is not compatible with non-UNIX systems.")
endif()

add_library(aether_x6100_control)

set_target_properties(
  aether_x6100_control
  PROPERTIES EXPORT_NAME "aether_x6100_control" #
             OUTPUT_NAME "aether_x6100_control" #
             DEFINE_SYMBOL "AETHER_X6100_CONTROL_EXPORTING" #
             VERSION "${PROJECT_VERSION}" #
             SOVERSION "${PROJECT_VERSION_MAJOR}")

set(AETHER_X6100_CONTROL_EXPORT_HEADER_FILE "${AETHER_INCLUDE_DIR}/x6100_control/api.h")

generate_export_header(
  aether_x6100_control
  BASE_NAME
  "AETHER_X6100_CONTROL"
  EXPORT_MACRO_NAME
  "AETHER_X6100_CONTROL_API"
  EXPORT_FILE_NAME
  ${AETHER_X6100_CONTROL_EXPORT_HEADER_FILE})

target_sources(aether_x6100_control PUBLIC FILE_SET HEADERS BASE_DIRS
                                           ${CMAKE_CURRENT_BINARY_DIR}/include)

target_sources(
  aether_x6100_control #
  PUBLIC FILE_SET HEADERS FILES
         ${CMAKE_CURRENT_BINARY_DIR}/${AETHER_X6100_CONTROL_EXPORT_HEADER_FILE})

add_subdirectory(include)
add_subdirectory(src)

include(cmake/install.cmake)
