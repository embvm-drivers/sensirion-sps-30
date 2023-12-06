#ifndef SPS30_RECORDED_DATA_H
#define SPS30_RECORDED_DATA_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

	/// Recorded I2C command to request device wakeup.
	extern const uint8_t sps30_wakeup_command[2];

	/// Recorded I2C command to request the firwmare version.
	extern const uint8_t sps30_request_fw_ver[2];
	/// Response payload with version 2.2
	extern const uint8_t sps30_fw_ver_response[3];
#define SPS30_FW_VER_RESPONSE_MAJOR 2
#define SPS30_FW_VER_RESPONSE_MINOR 2

	/// Recorded I2C command to request the serial number.
	extern const uint8_t sps30_request_serial_number[2];
	/// This is an example recorded response data set
	/// for the serial number request
	extern const uint8_t sps30_serial_number_response[48];
	/// This variable holds the properly decoded serial number for the
	/// response above and can be used for comparisons.
	extern const char sps30_serial_number_response_string[17];

	extern const uint8_t sps30_request_data_command[2];
	extern const uint8_t sps30_measurement_low_particle_response_1[60];
	extern const uint8_t sps30_measurement_low_particle_response_2[60];
	extern const uint8_t sps30_measurement_low_particle_response_3[60];
	extern const uint8_t sps30_measurement_low_particle_response_4[60];

#ifdef __cplusplus
}
#endif
#endif // SPS30_RECORDED_DATA_H
