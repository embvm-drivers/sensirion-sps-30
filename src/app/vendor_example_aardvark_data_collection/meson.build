# This application is used to collect I2C data using an Aardvark for use with
# the test suite.

sps30_i2c_example_usage = executable('sps30_i2c_example_usage_data_collection',
	[
		'sps30_example_usage.c',
		'sensirion_hw_i2c_aardvark_implementation.c'
	],
	dependencies: [
		sps30_vendor_driver_native_dep,
		aardvark_vendor_native_driver_dep
	],
	native: true
)
