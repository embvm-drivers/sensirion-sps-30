
sps30_test_files = files(
	'sps30_no_hardware.cpp',
)

clangtidy_files += sps30_test_files

catch2_tests_dep += declare_dependency(
	sources: sps30_test_files,
	dependencies: driver_test_lib_native_dep
)
