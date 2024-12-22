set(CRITERION_INCLUDE_DIR /opt/homebrew/include)
set(CRITERION_LIBRARY /opt/homebrew/lib/libcriterion.dylib)

add_executable(unit_tests
  tests_hash.c
  tests_dict_memory.c
  tests_dict_insert.c
)

target_include_directories(unit_tests PRIVATE ${CRITERION_INCLUDE_DIR})

target_link_libraries(unit_tests PRIVATE dict ${CRITERION_LIBRARY})

add_test(NAME unit_tests COMMAND unit_tests)