refactored_vendor_driver_tests = files(
	'refactored_vendor_driver_tests.cpp',
	'refactored_vendor_driver_mock.cpp'
)

clangtidy_files += refactored_vendor_driver_tests

catch2_tests_dep += declare_dependency(
	sources: refactored_vendor_driver_tests,
	dependencies: [
		sps30_recorded_data_native_dep,
		refactored_sps30_vendor_driver_native_dep
	],
)
