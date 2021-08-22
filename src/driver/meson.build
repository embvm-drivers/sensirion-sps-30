
driver_lib_inc = [
	include_directories('.'),
]

driver_i2c_lib = static_library('driver_i2c',
    [
    	'sps30_i2c_transport.cpp',
    	'driver.cpp',
    ],
    build_by_default: false,
)

driver_i2c_lib_native = static_library('driver_i2c_native',
    [
    	'sps30_i2c_transport.cpp',
    	'driver.cpp',
    ],
    build_by_default: false,
    native: true
)

driver_test_lib_native = static_library('driver_test_native',
	[
    	'sps30_test_transport.cpp',
    	'driver.cpp',
    ],
	build_by_default: false,
	native: true
)

driver_i2c_lib_dep = declare_dependency(
	include_directories: driver_lib_inc,
	link_with: driver_i2c_lib,
)

driver_i2c_lib_native_dep = declare_dependency(
	include_directories: driver_lib_inc,
	link_with: driver_i2c_lib_native,
)

driver_test_lib_native_dep = declare_dependency(
	include_directories: driver_lib_inc,
	link_with: driver_test_lib_native
)
