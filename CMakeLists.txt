cmake_minimum_required(VERSION 3.22)
project(web-server LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAG "-Wall -Wextra -Wformat-nonliteral -Wcast-align
  -Wpointer-arith -Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes
  -Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow
  -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -ansi")

find_package(Doxygen)
option(BUILD_DOCS "Creating documentation" ${DOXYGEN_FOUND})

if(BUILD_DOCS)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation")
  endif()

  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)
  message("Building documentation...")

  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile_in}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
endif()

add_executable(${PROJECT_NAME} src/main.c)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${PROJECT_BINARY_DIR}
    lib
)

target_link_directories(${PROJECT_NAME}
  PRIVATE
    lib
)

# cmake --install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
