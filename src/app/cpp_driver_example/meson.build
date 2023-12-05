sensirion_i2c_example_src = [
	'sensirion_i2c_example.cpp'
]

sensirion_i2c_example = executable('sensirion_i2c_example',
	sensirion_i2c_example_src,
	dependencies: [
		driver_i2c_lib_dep,
	],
	install: false,
)

executable('sensirion_i2c_example_native',
	sensirion_i2c_example_src,
	dependencies: [
		driver_i2c_lib_native_dep,
		aardvark_vendor_native_driver_dep
	],
	install: false,
	native: true
)

#############################
# Output Conversion Targets #
#############################

example_app_bin = custom_target('sensirion_i2c_example.hex',
	input: sensirion_i2c_example,
	output: 'sensirion_i2c_example.hex',
	command: host_hex_conversion,
	build_by_default: true
)

example_app_hex = custom_target('sensirion_i2c_example.bin',
	input: sensirion_i2c_example,
	output: 'sensirion_i2c_example.bin',
	command: host_bin_conversion,
	build_by_default: true
)

