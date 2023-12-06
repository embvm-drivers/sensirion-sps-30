#ifndef SPS30_RECORDED_DATA_H
#define SPS30_RECORDED_DATA_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

	extern const uint8_t sps30_wakeup_command[2];
	extern const uint8_t sps30_request_fw_ver[2];
	extern const uint8_t sps30_fw_ver_response[3];
	extern const uint8_t sps30_request_serial_number[2];
	extern const uint8_t sps30_serial_number_response[48];
	extern const uint8_t sps30_request_data_command[2];
	extern const uint8_t sps30_measurement_low_particle_response_1[60];

#ifdef __cplusplus
}
#endif

#endif // SPS30_RECORDED_DATA_H
