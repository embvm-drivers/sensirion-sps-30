#include <catch2/catch_test_macros.hpp>
#include <sps30.h>
#include <sps30_recorded_data.h>
#include "vendor_driver_mock.hpp"

TEST_CASE("SPS-30 Firmware Version", "[test/vendor_sps30]")
{
	uint8_t major_version;
	uint8_t minor_version;

	// We set the expected TX data that the driver should send over I2C
	sps30_mock_set_i2c_write_data(sps30_request_fw_ver, sizeof(sps30_request_fw_ver));
	// And then we set the data to be returned by the read command.
	sps30_mock_set_i2c_read_data(sps30_fw_ver_response, sizeof(sps30_fw_ver_response));

	// Now that we've set the expected TX/RX data, we run the desired API
	auto r = sps30_read_firmware_version(&major_version, &minor_version);

	// And check the outputs of the function, making sure the returned data
	// is correctly interpreted from the input data.
	CHECK(r == 0);
	CHECK(major_version == SPS30_FW_VER_RESPONSE_MAJOR);
	CHECK(minor_version == SPS30_FW_VER_RESPONSE_MINOR);
}

TEST_CASE("SPS-30 Serial Number", "[test/vendor_sps30]")
{
	char serial_num[SPS30_MAX_SERIAL_LEN];

	// We set the expected TX data that the driver should send over I2C
	sps30_mock_set_i2c_write_data(sps30_request_serial_number, sizeof(sps30_request_serial_number));
	// And then we set the data to be returned by the read command.
	sps30_mock_set_i2c_read_data(sps30_serial_number_response,
								 sizeof(sps30_serial_number_response));

	// Now that we've set the expected TX/RX data, we run the desired API
	auto r = sps30_get_serial(serial_num);

	// And check the outputs of the function, making sure the returned data
	// is correctly interpreted from the input data.
	CHECK(r == 0);
	CHECK(0 == strncmp(serial_num, sps30_serial_number_response_string,
					   sizeof(sps30_serial_number_response_string)));
}

TEST_CASE("SPS-30 Start Manual Fan Cleaning", "[test/vendor_sps30]")
{
	// We set the expected TX data that the driver should send over
	// I2C for this command.
	sps30_mock_set_i2c_write_data(sps30_request_start_manual_fan_cleaning,
								  sizeof(sps30_request_start_manual_fan_cleaning));

	auto r = sps30_start_manual_fan_cleaning();
	CHECK(r == 0);
}

TEST_CASE("SPS-30 Request Start Measurement", "[test/vendor_sps30]")
{
	// We set the expected TX data that the driver should send over
	// I2C for this command.
	sps30_mock_set_i2c_write_data(sps30_request_start_measurement,
								  sizeof(sps30_request_start_measurement));

	auto r = sps30_start_measurement();
	CHECK(r == 0);
}
