include(GoogleTest)

find_package(GTest REQUIRED)
add_executable(test tests.cc)
target_link_libraries(test goto_elimination GTest::GTest)

# gtest_discover_tests(test)
option(COLLECT_COVERAGE "Set to ON to generate the coverage data")
if (COLLECT_COVERAGE)
	set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
	message(${CMAKE_MODULE_PATH})

	include(CodeCoverage)
	append_coverage_compiler_flags()
	setup_target_for_coverage_gcovr_html(NAME test_coverage EXECUTABLE tests/test DEPENDENCIES test BASE_DIRECTORY "${PROJECT_SOURCE_DIR}/..")
endif()
