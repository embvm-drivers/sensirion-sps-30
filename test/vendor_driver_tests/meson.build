vendor_driver_tests = files(
	'vendor_driver_tests.cpp',
	'vendor_driver_mock.cpp'
)

clangtidy_files += vendor_driver_tests

catch2_tests_dep += declare_dependency(
	sources: vendor_driver_tests,
	dependencies: [
		sps30_recorded_data_native_dep,
		sps30_vendor_driver_dep
	],
)
