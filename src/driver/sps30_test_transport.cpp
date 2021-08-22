#include <cassert>
#include <cstring>
#include <driver.hpp> // for some details, like SPS30_SERIAL_NUM_BUFFER_LEN
#include <sps30_transport.hpp>
#include <stdio.h>

using namespace sps30;

#pragma mark - Private Data -

namespace
{
constexpr const char SPS30_TEST_SERIAL[] = "SPS30TESTTRANSPORT";

// One potential problem here is that our duration in the test transport is a file level static.
// This means that once we change it during a test run, it stays changed - regardless of whether the
// transport object is reinitialized. This will potentially mess up tests in the future, but it also
// mimics how the update works on the sensor device - once written to flash, it stays there. So we
// will leave this as-is for now and think about how to update it in the future if it causes
// problems.
std::chrono::duration<uint32_t> autoclean_interval_(604800); // defaults to one week (in seconds)
}; // namespace

#pragma mark - Private Functions -

namespace
{
void handle_get_serial(uint8_t* const data, const size_t length)
{
	static_assert(sizeof(SPS30_TEST_SERIAL) <= sps30::sensor::SPS30_SERIAL_NUM_BUFFER_LEN);
	assert(length >= sensor::SPS30_SERIAL_NUM_BUFFER_LEN);
	memcpy(data, SPS30_TEST_SERIAL, strlen(SPS30_TEST_SERIAL) + 1);
}

void handle_get_version(uint8_t* const data, const size_t length)
{
	assert(length == 2); // expected length for I2C at the very least
	*reinterpret_cast<uint16_t* const>(data) = 0x0201; // expected version 2.1
}

void handle_get_autoclean_interval(uint8_t* const data, const size_t length)
{
	assert(length == 4); // expected data size
	*reinterpret_cast<uint32_t* const>(data) = autoclean_interval_.count();
}

void handle_set_autoclean_interval(const uint8_t* const data, const size_t length)
{
	assert(length == 4); // expected data size
	autoclean_interval_ =
		std::chrono::duration<uint32_t>(*reinterpret_cast<const uint32_t* const>(data));
}

}; // namespace

#pragma mark - Public Interface -

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
