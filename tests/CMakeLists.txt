set(SRC_PATH ${CMAKE_SOURCE_DIR}/src/app)

enable_testing()

add_executable(
  tests
  tests.cpp
  
)

target_include_directories(
  tests
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  PRIVATE
    ${SRC_PATH}
    
)

target_link_libraries(
  tests
    antlr4_cpp_runtime
    parserlib
    semanticlib
    optimizerlib
    codegen
    gram
    gtest_main   
)

include(GoogleTest)
gtest_discover_tests(tests)
