sps30_recorded_data = static_library('sps30_recorded_data',
	sources: 'sps30_recorded_data.c',
	# For now, this is native, but we can clarify this later
	# with both native and non-native options.
	build_by_default: false
)

sps30_recorded_data_dep = declare_dependency(
	link_with: sps30_recorded_data,
	include_directories: include_directories('.')
)

sps30_recorded_data_native = static_library('sps30_recorded_data_native',
	sources: 'sps30_recorded_data.c',
	# For now, this is native, but we can clarify this later
	# with both native and non-native options.
	native: true,
	build_by_default: false
)

sps30_recorded_data_native_dep = declare_dependency(
	link_with: sps30_recorded_data,
	include_directories: include_directories('.')
)
