#include <cassert>
#include <chrono>
#include <sps30_transport.hpp>

static constexpr std::chrono::duration<uint16_t, std::micro> START_STOP_DELAY_USEC =
	std::chrono::microseconds(20000);
static constexpr std::chrono::duration<uint16_t, std::micro> CMD_DELAY_USEC =
	std::chrono::microseconds(5000);
static constexpr std::chrono::duration<uint16_t, std::micro> WRITE_FLASH_DELAY_USEC =
	std::chrono::microseconds(20000);

static constexpr uint16_t i2c_transport_command_conversion[] = {
	// SPS30_CMD_START_MEASUREMENT
	0x0010,
	// SPS30_CMD_START_MEASUREMENT_ARG
	0x0300,
	// SPS30_CMD_STOP_MEASUREMENT
	0x0104,
	// SPS30_CMD_READ_MEASUREMENT
	0x0300,
	// SPS30_CMD_GET_DATA_READY
	0x2020,
	// SPS30_CMD_AUTOCLEAN_INTERVAL
	0x8004,
	// SPS30_CMD_GET_FIRMWARE_VERSION
	0xd100,
	// SPS30_CMD_GET_SERIAL
	0xd033,
	// SPS30_CMD_RESET
	0xd304,
	// SPS30_CMD_SLEEP
	0x1001,
	// SPS30_CMD_READ_DEVICE_STATUS_REG
	0xd206,
	// SPS30_CMD_START_MANUAL_FAN_CLEANING
	0x5607,
	// SPS30_CMD_WAKE_UP
	0x1103};

#if 0
transport::status_t transport::read(const transport::command_t command, uint8_t* const data,
									const size_t length) const
{
	assert(data && length);

	switch(command)
	{
		case transport::command_t::SPS30_CMD_GET_SERIAL:
			handle_get_serial(data, length);
			break;
		case transport::command_t::SPS30_CMD_AUTOCLEAN_INTERVAL:
			handle_get_autoclean_interval(data, length);
			break;
		case transport::command_t::SPS30_CMD_GET_FIRMWARE_VERSION:
			handle_get_version(data, length);
			break;
		default:
			assert(0); // unexpected input
	}

	return transport::status_t::OK;
}

transport::status_t transport::write(const transport::command_t command, const uint8_t* const data,
									 const size_t length) const
{
	assert(data && length);

	switch(command)
	{
		case transport::command_t::SPS30_CMD_AUTOCLEAN_INTERVAL:
			handle_set_autoclean_interval(data, length);
			break;
		default:
			assert(0); // unexpected input
	}

	return transport::status_t::OK;
}

transport::status_t transport::transcieve(const transport::command_t command,
										  const uint8_t* const tx_data, const size_t tx_length,
										  uint8_t* const rx_data, const size_t rx_length) const
{
	assert(tx_data && tx_length);
	assert(rx_data && rx_length);

	switch(command)
	{
		default:
			assert(0); // unexpected input
	}

	return transport::status_t::OK;
}
#endif
