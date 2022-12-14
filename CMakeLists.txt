cmake_minimum_required(VERSION 3.22)
project(web-server LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAG "-Wall -Wextra -Wformat-nonliteral -Wcast-align
  -Wpointer-arith -Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes
  -Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow
  -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -ansi -O2")

option(CMAKE_C_ASAN "Enable address sanitiser" OFF)
option(CMAKE_C_UBSAN "Enable undefined behaviour sanitiser" OFF)
option(CMAKE_C_TSAN "Enable thread sanitiser" OFF)

if(CMAKE_C_ASAN)
  add_compile_options("-lasan -fsanitize=address")
elseif(CMAKE_C_UBSAN)
  add_compile_options("-lubsan -fsanitize=undefined")
elseif(CMAKE_C_TSAN)
  add_compile_options("-ltsan -fsanitize=thread")
endif()

find_package(Doxygen)
option(BUILD_DOCS "Creating documentation" ${DOXYGEN_FOUND})

if(BUILD_DOCS)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation")
  endif()

  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)
  message("-- Documentation generated from " ${doxyfile_in})

  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile_in}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
endif()

add_executable(${PROJECT_NAME} src/main.c)
add_subdirectory(lib)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${PROJECT_BINARY_DIR}
    lib
)

target_link_directories(${PROJECT_NAME}
  PRIVATE
    lib
)

target_link_libraries(${PROJECT_NAME} Library)

# cmake --install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
