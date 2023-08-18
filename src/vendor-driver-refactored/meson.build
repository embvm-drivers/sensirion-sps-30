
sps30_vendor_driver_lib = static_library('sps30_vendor_driver',
    [
    	'sensirion_common.c',
    	'sps30.c',
    ],
    include_directories: include_directories('.'),
    build_by_default: false,
)

sps30_vendor_driver_dep = declare_dependency(
	link_with: sps30_vendor_driver_lib,
	include_directories: include_directories('.')
)

sps30_i2c_example_usage = executable('sps30_i2c_example_usage',
	[
		'sps30_example_usage.c',
		'sensirion_hw_i2c_aardvark_implementation.c'
	],
	dependencies: [
		sps30_vendor_driver_dep,
		aardvark_vendor_native_driver_dep
	],
	native: true
)
