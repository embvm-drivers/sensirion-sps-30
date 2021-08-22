# Test Build Definition

# TODO
# - Rename test targets
# - Add test files to the bulid

PROJECT_test_files = files(
	'test_suite.c',
)

clangtidy_files += PROJECT_test_files

catch2_tests_dep += declare_dependency(
	sources: 'catch2_test_case.cpp',
)
