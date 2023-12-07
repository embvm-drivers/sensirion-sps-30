#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <sps30.h>
#include <sps30_recorded_data.h>
#include "vendor_driver_mock.hpp"
#include <cstdio>

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

TEST_CASE("SPS-30 Data Receive", "[test/vendor_sps30]")
{
	// We set the expected TX data that the driver should send over I2C
	sps30_mock_set_i2c_write_data(sps30_read_measurement_command, sizeof(sps30_read_measurement_command));
	// And then we set the data to be returned by the read command.
	sps30_mock_set_i2c_read_data(sps30_measurement_low_particle_response_1,
								 sizeof(sps30_measurement_low_particle_response_1));

	struct sps30_measurement output;
	auto r = sps30_read_measurement(&output);
	CHECK(r == 0);

	/// Catch2 has special support for floating point comparisons
	/// See here: https://github.com/catchorg/Catch2/blob/devel/docs/comparing-floating-point-numbers.md
	/// We use a ULP strategy. For more info on that, see:
	/// https://embeddedartistry.com/lesson/converting-floating-point-epsilon-comparisons-to-ulp-comparisons/
	CHECK_THAT(output.mc_1p0, Catch::Matchers::WithinULP(0.1628956049680710f, 0));
	CHECK_THAT(output.mc_2p5, Catch::Matchers::WithinULP(0.2644746303558350f, 0));
	CHECK_THAT(output.mc_4p0, Catch::Matchers::WithinULP(0.3391090929508209f, 0));
	CHECK_THAT(output.mc_10p0, Catch::Matchers::WithinULP(0.3540358543395996f, 0));
	CHECK_THAT(output.nc_0p5, Catch::Matchers::WithinULP(0.8901749253273010f, 0));
	CHECK_THAT(output.nc_1p0, Catch::Matchers::WithinULP(1.1843987703323364f, 0));
	CHECK_THAT(output.nc_2p5, Catch::Matchers::WithinULP(1.2940582036972046f, 0));
	CHECK_THAT(output.nc_4p0, Catch::Matchers::WithinULP(1.3163284063339233f, 0));
	CHECK_THAT(output.nc_10p0, Catch::Matchers::WithinULP(1.3195588588714600f, 0));
	CHECK_THAT(output.typical_particle_size, Catch::Matchers::WithinULP(0.7204053997993469f, 0));

	sps30_mock_set_i2c_write_data(sps30_read_measurement_command, sizeof(sps30_read_measurement_command));
	sps30_mock_set_i2c_read_data(sps30_measurement_low_particle_response_2,
								 sizeof(sps30_measurement_low_particle_response_2));

	r = sps30_read_measurement(&output);
	CHECK(r == 0);

	CHECK_THAT(output.mc_1p0, Catch::Matchers::WithinULP(1.0780845880508423f, 0));
	CHECK_THAT(output.mc_2p5, Catch::Matchers::WithinULP(1.4293757677078247f, 0));
	CHECK_THAT(output.mc_4p0, Catch::Matchers::WithinULP(1.6635462045669556f, 0));
	CHECK_THAT(output.mc_10p0, Catch::Matchers::WithinULP(1.7103787660598755f, 0));
	CHECK_THAT(output.nc_0p5, Catch::Matchers::WithinULP(6.7429466247558594f, 0));
	CHECK_THAT(output.nc_1p0, Catch::Matchers::WithinULP(8.2304849624633789f, 0));
	CHECK_THAT(output.nc_2p5, Catch::Matchers::WithinULP(8.5852422714233398f, 0));
	CHECK_THAT(output.nc_4p0, Catch::Matchers::WithinULP(8.6565103530883789f, 0));
	CHECK_THAT(output.nc_10p0, Catch::Matchers::WithinULP(8.6674308776855469f, 0));
	CHECK_THAT(output.typical_particle_size, Catch::Matchers::WithinULP(0.6293675899505615f, 0));

}
