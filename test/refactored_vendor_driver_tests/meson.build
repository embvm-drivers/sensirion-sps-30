refactored_vendor_driver_tests = files(
	'refactored_vendor_driver_tests.cpp',
	'refactored_vendor_driver_mock.cpp'
)

clangtidy_files += refactored_vendor_driver_tests

# This is a separate dep from catch2_tests_dep because we want to build a second
# application to test out the refactored vendor driver. We don't want to link the vendor
# driver along with this one in a single catch app, because we will see linker issues
# regarding duplicate symbol definitions.
#
# The test application target is defined in the top-level meson.build, after the catch
# module is invoked.
refactored_vendor_driver_catch_dep = declare_dependency(
	sources: refactored_vendor_driver_tests,
	dependencies: [
		sps30_recorded_data_native_dep,
		refactored_sps30_vendor_driver_native_dep
	],
)
