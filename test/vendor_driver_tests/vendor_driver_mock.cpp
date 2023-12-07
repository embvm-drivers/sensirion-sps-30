/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include <unistd.h>
#include <assert.h>
#include <queue>
#include <catch2/catch_test_macros.hpp>

#define EXPECTED_SPS30_ADDRESS 0x69

/** Mock SPS-30 Implementation
 *
 * This driver mock is designed to work with the Sensirion SPS-30 driver
 * APIs. It contains implementations that satisfy the SPS-30 driver's APIs,
 * but enable the tester to work with recorded data. It also contains supporting
 * functions that are used exclusively by the test code to support testing,
 * both to check state as well as to modify data expectations.
 *
 * Due to the nature of the driver's implementation, the functions
 * can only be defined once, here in this source file, as they are "hard-coded"
 * APIs with missing implementations rather than function pointers.
 *
 * The "mock" here is simple, and perhaps is better described as a "spy" and/or
 * a "fake". A mocking framework would also work well, if you are
 * into such things. However, this testing scenario did not require that.
 */

/// Contains pointer to tx/rx data and the length of the data,
/// storing it within the TX and RX queues for expectation analysis.
struct ExpectedBuffer
{
	const uint8_t* data = nullptr;
	size_t length = 0;

	ExpectedBuffer(const uint8_t* data, size_t length) : data(data), length(length)
	{
	}
};

/// A queue of the expected TX data to be input when
/// sensirion_i2c_write() is called by the driver
static std::queue<ExpectedBuffer> expected_tx_queue_;
/// A queue of the expected RX data to be returned when
/// sensirion_i2c_read() is called by the driver
static std::queue<ExpectedBuffer> expected_rx_queue_;

/// Used for testing purposes to determine whether or not
/// sensirion_i2c_init() was called by the driver.
static bool i2c_initialized_ = false;
/// Used for testing purposes to determine whether or not
/// sensirion_i2c_release() was called by the driver.
static bool i2c_released_ = false;

void sps30_mock_reset_state()
{
	i2c_initialized_ = false;
	i2c_released_ = false;
}

bool sps30_mock_i2c_initialized()
{
	return i2c_initialized_;
}

bool sps30_mock_i2c_released()
{
	return i2c_released_;
}

void sps30_mock_set_i2c_write_data(const uint8_t* data, size_t length)
{
	assert(data && length);
	expected_tx_queue_.emplace(data, length);
}

void sps30_mock_set_i2c_read_data(const uint8_t* data, size_t length)
{
	assert(data && length);
	expected_rx_queue_.emplace(data, length);
}

/** Select the current i2c bus by index.
 * All following i2c operations will be directed at that bus.
 *
 * THE IMPLEMENTATION IS OPTIONAL ON SINGLE-BUS SETUPS (all sensors on the same
 * bus)
 *
 * This is a required Sensirion I2C driver API.
 *
 * @param bus_idx   Bus index to select
 * @returns         0 on success, an error code otherwise
 */
int16_t sensirion_i2c_select_bus(uint8_t bus_idx)
{
	// We currently aren't doing any testing around this
	// function, as it needs to be invoked by the user.
	// TODO: PJ confirm this.
	(void)bus_idx;
	return 0;
}

/** Initialize all hard- and software components that are needed for the I2C
 * communication.
 *
 * This is a required Sensirion I2C driver API.
 */
void sensirion_i2c_init(void)
{
	// The mock driver is just checking nothing whether this function
	// is called at all. We can then check at the appropriate point.
	i2c_initialized_ = true;
}

/** Release all resources initialized by sensirion_i2c_init().
 *
 * This is a required Sensirion I2C driver API.
 */
void sensirion_i2c_release(void)
{
	// The mock driver is just checking nothing whether this function
	// is called at all. We can then check at the appropriate point.
	i2c_released_ = false;
}

/** Implement "read" for the I2C device
 *
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * For our test implementation
 *
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count)
{
	// Check that we've got an expected value in the queue
	REQUIRE(expected_rx_queue_.empty() == false);

	// Remove the front element from the queue for use here.
	auto expected_data = expected_rx_queue_.front();
	expected_rx_queue_.pop();

	// We make sure the driver supplies the expected address
	CHECK(address == EXPECTED_SPS30_ADDRESS);
	// We make sure data and count are non-zero
	// If not the case, we must fail here to avoid any further
	// memory problems.
	REQUIRE((data && count));
	// We make sure the supplied count field matches the
	// length of the data we are going to return.
	// This uses REQUIRE instead of CHECK to avoid
	// the case where there's a mismatch, and we clobber memory
	// as a result.
	REQUIRE(count == expected_data.length);

	// We'll then memcpy the data to be returned
	memcpy(data, expected_data.data, count);

	return 0;
}

/** Implement "write" for the I2C device
 *
 * Execute one write transaction on the I2C bus, sending a given number of
 * bytes. The bytes in the supplied buffer must be sent to the given address. If
 * the slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count)
{
	// Check that we've got an expected value in the queue
	REQUIRE(expected_tx_queue_.empty() == false);

	// Remove the front element from the queue for use here.
	auto expected_data = expected_tx_queue_.front();
	expected_tx_queue_.pop();

	// We make sure the driver supplies the expected address
	CHECK(address == EXPECTED_SPS30_ADDRESS);
	// We make sure data and count are non-zero
	// If not the case, we must fail here to avoid any further
	// memory problems.
	REQUIRE((data && count));
	// We make sure the supplied coutn field matches the
	// length of the data we are going to return.
	REQUIRE(count == expected_data.length);
	auto match = memcmp(data, expected_data.data, count);
	CHECK(match == 0);

	return 0;
}

/** Sleep for a given number of microseconds.
 *
 * The function should delay the execution for at least the given time,
 * but may also sleep longer.
 *
 * Despite the unit, a <10 millisecond precision is sufficient.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(uint32_t useconds)
{
	// For the test code, we're just going to use the OS's functionality.
	usleep(useconds);
}
