#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <sps30.h>
#include <sps30_recorded_data.h>
#include "refactored_vendor_driver_mock.hpp"
#include <cstdio>

TEST_CASE("Refactored SPS-30 I2C Interactions", "[test/refactored_sps30]")
{
	// This function is run before every SECTION below, ensuring common
	// starting state for each operation.
	sps30_mock_reset_state();

	SECTION("SPS-30 Probe")
	{
		// The probe function requires that several TX-only commands
		// are executed before an RX command. So we set those expecations
		// as a chain in this function.
		sps30_mock_set_i2c_write_data(sps30_wakeup_command, sizeof(sps30_wakeup_command));
		sps30_mock_set_i2c_write_data(sps30_wakeup_command, sizeof(sps30_wakeup_command));
		sps30_mock_set_i2c_write_data(sps30_request_serial_number,
									  sizeof(sps30_request_serial_number));
		// And we just need to specify the one RX data piece: a serial number
		sps30_mock_set_i2c_read_data(sps30_serial_number_response,
									 sizeof(sps30_serial_number_response));

		// Now that we've set the expected TX/RX data, we run the desired API
		auto r = sps30_probe();
		CHECK(r == 0);
	}

	SECTION("SPS-30 Reset")
	{
		sps30_mock_set_i2c_write_data(sps30_reset_command, sizeof(sps30_reset_command));
		auto r = sps30_reset();
		CHECK(r == 0);
	}

	SECTION("SPS-30 Sleep")
	{
		sps30_mock_set_i2c_write_data(sps30_sleep_command, sizeof(sps30_sleep_command));
		auto r = sps30_sleep();
		CHECK(r == 0);
	}

	SECTION("SPS-30 Wakeup")
	{
		// The driver must sent the wakup command *twice*
		sps30_mock_set_i2c_write_data(sps30_wakeup_command, sizeof(sps30_wakeup_command));
		sps30_mock_set_i2c_write_data(sps30_wakeup_command, sizeof(sps30_wakeup_command));

		auto r = sps30_wake_up();
		CHECK(r == 0);
	}

	SECTION("SPS-30 Request Device Status")
	{
		uint32_t device_status_flags;

		sps30_mock_set_i2c_write_data(sps30_request_device_status,
									  sizeof(sps30_request_device_status));
		sps30_mock_set_i2c_read_data(sps30_device_status_response_1,
									 sizeof(sps30_device_status_response_1));
		auto r = sps30_read_device_status_register(&device_status_flags);
		CHECK(r == 0);
		CHECK(device_status_flags == 0);

		sps30_mock_set_i2c_write_data(sps30_request_device_status,
									  sizeof(sps30_request_device_status));
		sps30_mock_set_i2c_read_data(sps30_device_status_response_2,
									 sizeof(sps30_device_status_response_2));
		r = sps30_read_device_status_register(&device_status_flags);
		CHECK(r == 0);
		CHECK(device_status_flags == 0x100000);
	}

	SECTION("SPS-30 Firmware Version")
	{
		uint8_t major_version;
		uint8_t minor_version;

		sps30_mock_reset_state();

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

	SECTION("SPS-30 Serial Number")
	{
		char serial_num[SPS30_MAX_SERIAL_LEN];

		// We set the expected TX data that the driver should send over I2C
		sps30_mock_set_i2c_write_data(sps30_request_serial_number,
									  sizeof(sps30_request_serial_number));
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

	SECTION("SPS-30 Set Fan Auto-Cleaning Interval")
	{
		// We set the expected TX data that the driver should send over
		// I2C for this command.
		sps30_mock_set_i2c_write_data(sps30_set_fan_auto_cleaning_interval_1,
									  sizeof(sps30_set_fan_auto_cleaning_interval_1));

		auto r = sps30_set_fan_auto_cleaning_interval(39288);
		CHECK(r == 0);

		sps30_mock_set_i2c_write_data(sps30_set_fan_auto_cleaning_interval_2,
									  sizeof(sps30_set_fan_auto_cleaning_interval_2));

		r = sps30_set_fan_auto_cleaning_interval(172800);
		CHECK(r == 0);
	}

	SECTION("SPS-30 Get Fan Auto-Cleaning Interval")
	{
		uint32_t interval;

		sps30_mock_set_i2c_write_data(sps30_request_fan_auto_cleaning_interval,
									  sizeof(sps30_request_fan_auto_cleaning_interval));
		sps30_mock_set_i2c_read_data(sps30_fan_auto_cleaning_interval_response_1,
									 sizeof(sps30_fan_auto_cleaning_interval_response_1));
		auto r = sps30_get_fan_auto_cleaning_interval(&interval);
		CHECK(r == 0);
		CHECK(interval == 172800);

		sps30_mock_set_i2c_write_data(sps30_request_fan_auto_cleaning_interval,
									  sizeof(sps30_request_fan_auto_cleaning_interval));
		sps30_mock_set_i2c_read_data(sps30_fan_auto_cleaning_interval_response_2,
									 sizeof(sps30_fan_auto_cleaning_interval_response_2));
		r = sps30_get_fan_auto_cleaning_interval(&interval);
		CHECK(r == 0);
		CHECK(interval == 39288);
	}

	SECTION("SPS-30 Set Fan Auto-Cleaning Interval in Days")
	{
		sps30_mock_set_i2c_write_data(sps30_set_fan_auto_cleaning_interval_2,
									  sizeof(sps30_set_fan_auto_cleaning_interval_2));
		auto r = sps30_set_fan_auto_cleaning_interval_days(2);
		CHECK(r == 0);
	}

	SECTION("SPS-30 Get Fan Auto-Cleaning Interval in Days")
	{
		uint8_t interval;

		sps30_mock_set_i2c_write_data(sps30_request_fan_auto_cleaning_interval,
									  sizeof(sps30_request_fan_auto_cleaning_interval));
		sps30_mock_set_i2c_read_data(sps30_fan_auto_cleaning_interval_response_1,
									 sizeof(sps30_fan_auto_cleaning_interval_response_1));
		auto r = sps30_get_fan_auto_cleaning_interval_days(&interval);
		CHECK(r == 0);
		CHECK(interval == 2);

		sps30_mock_set_i2c_write_data(sps30_request_fan_auto_cleaning_interval,
									  sizeof(sps30_request_fan_auto_cleaning_interval));
		sps30_mock_set_i2c_read_data(sps30_fan_auto_cleaning_interval_response_2,
									 sizeof(sps30_fan_auto_cleaning_interval_response_2));
		r = sps30_get_fan_auto_cleaning_interval_days(&interval);
		CHECK(r == 0);
		CHECK(interval == 0);
	}

	SECTION("SPS-30 Start Manual Fan Cleaning")
	{
		// We set the expected TX data that the driver should send over
		// I2C for this command.
		sps30_mock_set_i2c_write_data(sps30_request_start_manual_fan_cleaning,
									  sizeof(sps30_request_start_manual_fan_cleaning));

		auto r = sps30_start_manual_fan_cleaning();
		CHECK(r == 0);
	}

	SECTION("SPS-30 Request Start Measurement")
	{
		// We set the expected TX data that the driver should send over
		// I2C for this command.
		sps30_mock_set_i2c_write_data(sps30_request_start_measurement,
									  sizeof(sps30_request_start_measurement));

		auto r = sps30_start_measurement();
		CHECK(r == 0);
	}

	SECTION("SPS-30 Request Stop Measurement")
	{
		// We set the expected TX data that the driver should send over
		// I2C for this command.
		sps30_mock_set_i2c_write_data(sps30_request_stop_measurement,
									  sizeof(sps30_request_stop_measurement));

		auto r = sps30_stop_measurement();
		CHECK(r == 0);
	}

	SECTION("SPS-30 Request Data Ready")
	{
		uint16_t data_ready;

		sps30_mock_set_i2c_write_data(sps30_request_data_ready, sizeof(sps30_request_data_ready));
		sps30_mock_set_i2c_read_data(sps30_data_ready_response_1,
									 sizeof(sps30_data_ready_response_1));
		auto r = sps30_read_data_ready(&data_ready);
		CHECK(r == 0);
		CHECK(data_ready == 0);

		sps30_mock_set_i2c_write_data(sps30_request_data_ready, sizeof(sps30_request_data_ready));
		sps30_mock_set_i2c_read_data(sps30_data_ready_response_2,
									 sizeof(sps30_data_ready_response_2));
		r = sps30_read_data_ready(&data_ready);
		CHECK(r == 0);
		CHECK(data_ready == 1);
	}

	SECTION("SPS-30 Data Receive")
	{
		// We set the expected TX data that the driver should send over I2C
		sps30_mock_set_i2c_write_data(sps30_read_measurement_command,
									  sizeof(sps30_read_measurement_command));
		// And then we set the data to be returned by the read command.
		sps30_mock_set_i2c_read_data(sps30_measurement_low_particle_response_1,
									 sizeof(sps30_measurement_low_particle_response_1));

		struct sps30_measurement output;
		auto r = sps30_read_measurement(&output);
		CHECK(r == 0);

		/// Catch2 has special support for floating point comparisons
		/// See here:
		/// https://github.com/catchorg/Catch2/blob/devel/docs/comparing-floating-point-numbers.md
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
		CHECK_THAT(output.typical_particle_size,
				   Catch::Matchers::WithinULP(0.7204053997993469f, 0));

		sps30_mock_set_i2c_write_data(sps30_read_measurement_command,
									  sizeof(sps30_read_measurement_command));
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
		CHECK_THAT(output.typical_particle_size,
				   Catch::Matchers::WithinULP(0.6293675899505615f, 0));

		sps30_mock_set_i2c_write_data(sps30_read_measurement_command,
									  sizeof(sps30_read_measurement_command));
		sps30_mock_set_i2c_read_data(sps30_measurement_low_particle_response_3,
									 sizeof(sps30_measurement_low_particle_response_3));

		r = sps30_read_measurement(&output);
		CHECK(r == 0);
		CHECK_THAT(output.mc_1p0, Catch::Matchers::WithinULP(6.4453110694885254f, 0));
		CHECK_THAT(output.mc_2p5, Catch::Matchers::WithinULP(7.2207860946655273f, 0));
		CHECK_THAT(output.mc_4p0, Catch::Matchers::WithinULP(7.5486493110656738f, 0));
		CHECK_THAT(output.mc_10p0, Catch::Matchers::WithinULP(7.6142153739929199f, 0));
		CHECK_THAT(output.nc_0p5, Catch::Matchers::WithinULP(43.8269462585449219f, 0));
		CHECK_THAT(output.nc_1p0, Catch::Matchers::WithinULP(50.8229331970214844f, 0));
		CHECK_THAT(output.nc_2p5, Catch::Matchers::WithinULP(51.4127349853515625f, 0));
		CHECK_THAT(output.nc_4p0, Catch::Matchers::WithinULP(51.5246505737304688f, 0));
		CHECK_THAT(output.nc_10p0, Catch::Matchers::WithinULP(51.5467681884765625f, 0));
		CHECK_THAT(output.typical_particle_size,
				   Catch::Matchers::WithinULP(0.6570276618003845f, 0));

		sps30_mock_set_i2c_write_data(sps30_read_measurement_command,
									  sizeof(sps30_read_measurement_command));
		sps30_mock_set_i2c_read_data(sps30_measurement_mid_particle_response_1,
									 sizeof(sps30_measurement_mid_particle_response_1));

		r = sps30_read_measurement(&output);
		CHECK(r == 0);
		CHECK_THAT(output.mc_1p0, Catch::Matchers::WithinULP(8.3007287979125977f, 0));
		CHECK_THAT(output.mc_2p5, Catch::Matchers::WithinULP(12.5388498306274414f, 0));
		CHECK_THAT(output.mc_4p0, Catch::Matchers::WithinULP(15.5828180313110352f, 0));
		CHECK_THAT(output.mc_10p0, Catch::Matchers::WithinULP(16.1916084289550781f, 0));
		CHECK_THAT(output.nc_0p5, Catch::Matchers::WithinULP(47.8495903015136719f, 0));
		CHECK_THAT(output.nc_1p0, Catch::Matchers::WithinULP(61.4989509582519531f, 0));
		CHECK_THAT(output.nc_2p5, Catch::Matchers::WithinULP(66.0026779174804688f, 0));
		CHECK_THAT(output.nc_4p0, Catch::Matchers::WithinULP(66.9150390625000000f, 0));
		CHECK_THAT(output.nc_10p0, Catch::Matchers::WithinULP(67.0490875244140625f, 0));
		CHECK_THAT(output.typical_particle_size,
				   Catch::Matchers::WithinULP(0.8576955795288086f, 0));

		sps30_mock_set_i2c_write_data(sps30_read_measurement_command,
									  sizeof(sps30_read_measurement_command));
		sps30_mock_set_i2c_read_data(sps30_measurement_mid_particle_response_2,
									 sizeof(sps30_measurement_mid_particle_response_2));

		r = sps30_read_measurement(&output);
		CHECK(r == 0);
		CHECK_THAT(output.mc_1p0, Catch::Matchers::WithinULP(8.4688024520874023f, 0));
		CHECK_THAT(output.mc_2p5, Catch::Matchers::WithinULP(19.5325355529785156f, 0));
		CHECK_THAT(output.mc_4p0, Catch::Matchers::WithinULP(28.0928020477294922f, 0));
		CHECK_THAT(output.mc_10p0, Catch::Matchers::WithinULP(29.8048496246337891f, 0));
		CHECK_THAT(output.nc_0p5, Catch::Matchers::WithinULP(30.9383926391601562f, 0));
		CHECK_THAT(output.nc_1p0, Catch::Matchers::WithinULP(54.5167579650878906f, 0));
		CHECK_THAT(output.nc_2p5, Catch::Matchers::WithinULP(66.9015884399414062f, 0));
		CHECK_THAT(output.nc_4p0, Catch::Matchers::WithinULP(69.4307708740234375f, 0));
		CHECK_THAT(output.nc_10p0, Catch::Matchers::WithinULP(69.7871704101562500f, 0));
		CHECK_THAT(output.typical_particle_size,
				   Catch::Matchers::WithinULP(1.1527029275894165f, 0));

		sps30_mock_set_i2c_write_data(sps30_read_measurement_command,
									  sizeof(sps30_read_measurement_command));
		sps30_mock_set_i2c_read_data(sps30_measurement_zero_particle_response,
									 sizeof(sps30_measurement_zero_particle_response));

		r = sps30_read_measurement(&output);
		CHECK(r == 0);
		CHECK_THAT(output.mc_1p0, Catch::Matchers::WithinULP(0.0f, 0));
		CHECK_THAT(output.mc_2p5, Catch::Matchers::WithinULP(0.0f, 0));
		CHECK_THAT(output.mc_4p0, Catch::Matchers::WithinULP(0.0f, 0));
		CHECK_THAT(output.mc_10p0, Catch::Matchers::WithinULP(0.0f, 0));
		CHECK_THAT(output.nc_0p5, Catch::Matchers::WithinULP(0.0f, 0));
		CHECK_THAT(output.nc_1p0, Catch::Matchers::WithinULP(0.0f, 0));
		CHECK_THAT(output.nc_2p5, Catch::Matchers::WithinULP(0.0f, 0));
		CHECK_THAT(output.nc_4p0, Catch::Matchers::WithinULP(0.0f, 0));
		CHECK_THAT(output.nc_10p0, Catch::Matchers::WithinULP(0.0f, 0));
		CHECK_THAT(output.typical_particle_size,
				   Catch::Matchers::WithinULP(1.6299999952316284f, 0));
	}
}
