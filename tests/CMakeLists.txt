include(GoogleTest)

find_package(GTest REQUIRED)
# include_directories(ast goto_elimination tests)
# file(GLOB SOURCES tests.cc ../src/*.cc ../ast/*.cc ../goto_elimination/*.cc)
add_executable(test tests.cc)
target_link_libraries(test goto_elimination GTest::GTest)

# gtest_add_tests(TARGET tests TEST_PREFIX old:)
gtest_discover_tests(test)
# add_test(NAME monolithic COMMAND tests)
