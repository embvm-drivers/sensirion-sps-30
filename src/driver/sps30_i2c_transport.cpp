
#include <chrono>
#include <sps30_transport.hpp>

static constexpr std::chrono::duration<uint16_t, std::micro> START_STOP_DELAY_USEC =
	std::chrono::microseconds(20000);
static constexpr std::chrono::duration<uint16_t, std::micro> CMD_DELAY_USEC =
	std::chrono::microseconds(5000);
static constexpr std::chrono::duration<uint16_t, std::micro> WRITE_FLASH_DELAY_USEC =
	std::chrono::microseconds(20000);

// TODO: refactor for ease of use: SPS30_SERIAL_NUM_BUFFER_LEN
static constexpr uint8_t SERIAL_NUM_WORDS = 32 / 2;

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
