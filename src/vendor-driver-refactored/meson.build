
refactored_sps30_vendor_driver_lib = static_library('sps30_vendor_driver_refactored',
    [
    	'sensirion_common.c',
    	'sps30.c',
    ],
    include_directories: include_directories('.'),
    build_by_default: false,
)

refactored_sps30_vendor_driver_dep = declare_dependency(
	link_with: sps30_vendor_driver_lib,
	include_directories: include_directories('.')
)

refactored_sps30_vendor_driver_native_lib = static_library('sps30_vendor_driver_refactored_native',
    [
        'sensirion_common.c',
        'sps30.c',
    ],
    include_directories: include_directories('.'),
    native: true,
    build_by_default: false,
)

refactored_sps30_vendor_driver_native_dep = declare_dependency(
    link_with: sps30_vendor_driver_native_lib,
    include_directories: include_directories('.')
)
