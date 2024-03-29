#ifndef SPS30_RECORDED_DATA_H
#define SPS30_RECORDED_DATA_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

	/// Recorded I2C command to request device wakeup.
	extern const uint8_t sps30_wakeup_command[2];
	extern const uint8_t sps30_sleep_command[2];
	extern const uint8_t sps30_reset_command[2];

	/// Recorded I2C command to request the firwmare version.
	extern const uint8_t sps30_request_fw_ver[2];
	/// Response payload with version 2.2
	extern const uint8_t sps30_fw_ver_response[3];
#define SPS30_FW_VER_RESPONSE_MAJOR 2
#define SPS30_FW_VER_RESPONSE_MINOR 2

	extern const uint8_t sps30_request_device_status[2];
	extern const uint8_t sps30_device_status_response_1[6];
	extern const uint8_t sps30_device_status_response_2[6];
	extern const uint8_t sps30_request_data_ready[2];
	extern const uint8_t sps30_data_ready_response_1[3];
	extern const uint8_t sps30_data_ready_response_2[3];
	extern const uint8_t sps30_set_fan_auto_cleaning_interval_1[8];
	extern const uint8_t sps30_set_fan_auto_cleaning_interval_2[8];
	extern const uint8_t sps30_request_fan_auto_cleaning_interval[2];
	extern const uint8_t sps30_fan_auto_cleaning_interval_response_1[6];
	extern const uint8_t sps30_fan_auto_cleaning_interval_response_2[6];

	extern const uint8_t sps30_request_start_manual_fan_cleaning[2];

	extern const uint8_t sps30_request_start_measurement[5];
	extern const uint8_t sps30_request_stop_measurement[2];

	/// Recorded I2C command to request the serial number.
	extern const uint8_t sps30_request_serial_number[2];
	/// This is an example recorded response data set
	/// for the serial number request
	extern const uint8_t sps30_serial_number_response[48];
	/// This variable holds the properly decoded serial number for the
	/// response above and can be used for comparisons.
	extern const char sps30_serial_number_response_string[17];

	extern const uint8_t sps30_read_measurement_command[2];
	extern const uint8_t sps30_measurement_low_particle_response_1[60];
	extern const uint8_t sps30_measurement_low_particle_response_2[60];
	extern const uint8_t sps30_measurement_low_particle_response_3[60];
	extern const uint8_t sps30_measurement_mid_particle_response_1[60];
	extern const uint8_t sps30_measurement_mid_particle_response_2[60];
	extern const uint8_t sps30_measurement_zero_particle_response[60];

#ifdef __cplusplus
}
#endif
#endif // SPS30_RECORDED_DATA_H
