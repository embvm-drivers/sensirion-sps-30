vendor_driver_tests = files(
	'vendor_driver_tests.cpp',
)

clangtidy_files += vendor_driver_tests

catch2_tests_dep += declare_dependency(
	sources: vendor_driver_tests,
	dependencies: driver_test_lib_native_dep
)
