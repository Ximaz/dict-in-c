find_path(CRITERION_INCLUDE_DIR criterion/criterion.h
          PATH_SUFFIXES criterion)

find_library(CRITERION_LIBRARY NAMES criterion libcriterion)

set(CRITERION_LIBRARIES ${CRITERION_LIBRARY})
set(CRITERION_INCLUDE_DIRS ${CRITERION_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIET and REQUIRED arguments and set CRITERION_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Criterion DEFAULT_MSG
                                  CRITERION_LIBRARY CRITERION_INCLUDE_DIR)

mark_as_advanced(CRITERION_INCLUDE_DIR CRITERION_LIBRARY)
# set(CRITERION_INCLUDE_DIR /opt/homebrew/include)
# set(CRITERION_LIBRARY /opt/homebrew/lib/libcriterion.dylib)

add_executable(unit_tests
  "tests_hash.c"
  "tests_dict_memory.c"
  "tests_dict_insert.c"
  "tests_dict_keys.c"
  "tests_dict_values.c"
)

target_include_directories(unit_tests PRIVATE ${CRITERION_INCLUDE_DIR})

target_link_libraries(unit_tests PRIVATE dict ${CRITERION_LIBRARY})

add_test(NAME unit_tests COMMAND unit_tests)