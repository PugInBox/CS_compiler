cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME Csharp)

project(
  ${PROJECT_NAME}
  VERSION 1.0
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(tests)
