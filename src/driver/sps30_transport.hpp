

/// Possible transport function status return values
enum sps30_transport_status_t
{
	/// Operation was successful
	OK = 0,
	/// An error occurred while transmitting or receiving data on the hardware bus
	BUS_ERROR,
	/// The driver's state is not valid for this operation
	/// E.g., trying to call read() when the driver that has not been started
	INVALID_STATE,
	/// The device firmware does not support the specified command
	SENSOR_FIRWMARE_DOES_NOT_SUPPORT_COMMAND,
	/// The action you are attempting is not currently implemented
	NOT_IMPLEMENTED,
	/// An error occurred, but its details are not specified
	UNKNOWN_ERROR,
};

/// Generic representation of commands that can be issued across the SPS-30
/// transport layer.
enum sps30_transport_commands_t
{
	SPS30_CMD_START_MEASUREMENT = 0,
	SPS30_CMD_START_MEASUREMENT_ARG,
	SPS30_CMD_STOP_MEASUREMENT,
	SPS30_CMD_READ_MEASUREMENT,
	SPS30_CMD_GET_DATA_READY,
	SPS30_CMD_AUTOCLEAN_INTERVAL,
	SPS30_CMD_GET_FIRMWARE_VERSION,
	SPS30_CMD_GET_SERIAL,
	SPS30_CMD_RESET,
	SPS30_CMD_SLEEP,
	SPS30_CMD_READ_DEVICE_STATUS_REG,
	SPS30_CMD_START_MANUAL_FAN_CLEANING,
	SPS30_CMD_WAKE_UP,
};
