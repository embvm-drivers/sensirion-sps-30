#include "sps30_recorded_data.h"

/**
 *
 * Note that all of the buffers below EXCLUDE the address byte.
 */

#define EXPECTED_SPS30_ADDRESS 0x69

/** On Recorded Data for Vendor's probe() implementation
 *
 * The vendor's SPS30 Driver's probe() function does the following:
 * - Issue a wakeup, which requires the same command to be sent TWICE within 100ms
 * - Read the serial number, but discard the output
 * - Return the value of the return code of serial number read
 * which means you see:
 * Writing 2 bytes to address 0x69
 * [0x11, 0x3]
 * Writing 2 bytes to address 0x69
 * [0x11, 0x3]
 * Writing 2 bytes to address 0x69
 * [0xd0, 0x33]
 * Received 48 bytes
 * [0x37, 0x45, 0x55, 0x44, 0x36, 0xe8, 0x32, 0x35, 0xda, 0x45, 0x43, 0x11, 0x32, 0x43, 0x84, 0x45,
 * 0x38, 0x03, 0x31, 0x45, 0x0f, 0x45, 0x38, 0x03, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81, 0x00, 0x00,
 * 0x81, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81]
 * SPS sensor probing successful
 */

const uint8_t sps30_wakeup_command[2] = {0x11, 0x3};

const uint8_t sps30_request_fw_ver[2] = {0xd1, 0x0};
// FW: 2.2
const uint8_t sps30_fw_ver_response[3] = {0x02, 0x02, 0x3a};

const uint8_t sps30_request_serial_number[2] = {0xd0, 0x33};
// Serial Number: 7ED625EC2CE81EE8
const uint8_t sps30_serial_number_response[48] = {
	0x37, 0x45, 0x55, 0x44, 0x36, 0xe8, 0x32, 0x35, 0xda, 0x45, 0x43, 0x11, 0x32, 0x43, 0x84, 0x45,
	0x38, 0x03, 0x31, 0x45, 0x0f, 0x45, 0x38, 0x03, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81, 0x00, 0x00,
	0x81, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81};

// Command to start measurements (what are the two?)
// Writing 2 bytes to address 0x69
// [0x56, 0x7]
// Writing 5 bytes to address 0x69
// [0x00, 0x10, 0x03, 0x00, 0xac]

const uint8_t sps30_request_data_command[2] = {0x03, 0x0};

/* Measured Values:
	0.16 pm1.0
	0.26 pm2.5
	0.34 pm4.0
	0.35 pm10.0
	0.89 nc0.5
	1.18 nc1.0
	1.29 nc2.5
	1.32 nc4.5
	1.32 nc10.0
	0.72 typical particle size
*/
const uint8_t sps30_measurement_low_particle_response_1[60] = {
	0x3e, 0x26, 0x7e, 0xce, 0x1b, 0xef, 0x3e, 0x87, 0xb3, 0x69, 0x38, 0x6b, 0x3e, 0xad, 0xee,
	0x9f, 0xb5, 0x1f, 0x3e, 0xb5, 0x14, 0x44, 0x30, 0x4e, 0x3f, 0x63, 0x42, 0xe2, 0x81, 0x65,
	0x3f, 0x97, 0x04, 0x9a, 0x61, 0xa8, 0x3f, 0xa5, 0xa3, 0xa3, 0xb3, 0xbf, 0x3f, 0xa8, 0xef,
	0x7d, 0x73, 0x51, 0x3f, 0xa8, 0xef, 0xe7, 0x4e, 0x7b, 0x3f, 0x38, 0xd6, 0x6c, 0x7d, 0xd4};
