
driver_lib_inc = [
	include_directories('.'),
]

driver_lib = static_library('driver',
    'sps30_i2c_transport.cpp',
    build_by_default: false,
)

driver_lib_native = static_library('driver_native',
    'sps30_i2c_transport.cpp',
    build_by_default: false,
    native: true
)

driver_lib_dep = declare_dependency(
	include_directories: driver_lib_inc,
	link_with: driver_lib,
)

driver_lib_native_dep = declare_dependency(
	include_directories: driver_lib_inc,
	link_with: driver_lib_native,
)
