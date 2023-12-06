#include "sps30_recorded_data.h"

/** This data was recorded from I2C communication with the SPS30 sensor
 * using the vendor driver. It can be used for testing or simulation purposes.
 *
 * Where relevant, conversions for the raw data has been presented for
 * your convenience.
 *
 * Note that all of the buffers below EXCLUDE the address byte.
 */

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
const char sps30_serial_number_response_string[17] = "7ED625EC2CE81EE8";

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

/** Measured values:
	1.08 pm1.0
	1.43 pm2.5
	1.66 pm4.0
	1.71 pm10.0
	6.74 nc0.5
	8.23 nc1.0
	8.59 nc2.5
	8.66 nc4.5
	8.67 nc10.0
	0.63 typical particle size
*/
const uint8_t sps30_measurement_low_particle_response_2[60] = {
	0x3f, 0x89, 0x58, 0xfe, 0xad, 0x44, 0x3f, 0xb6, 0xb3, 0xf5, 0xc9, 0x21, 0x3f, 0xd4, 0x6a,
	0xef, 0x15, 0xd8, 0x3f, 0xda, 0x75, 0xed, 0xb1, 0x39, 0x40, 0xd7, 0x9b, 0xc6, 0x38, 0x0d,
	0x41, 0x03, 0xaf, 0xb0, 0x11, 0x62, 0x41, 0x09, 0x74, 0x5d, 0x27, 0x37, 0x41, 0x0a, 0x27,
	0x81, 0x11, 0x24, 0x41, 0x0a, 0x27, 0xad, 0xcc, 0x04, 0x3f, 0x21, 0x1d, 0x1e, 0x3c, 0x3a};

/** measured values:
	1.30 pm1.0
	1.38 pm2.5
	1.38 pm4.0
	1.38 pm10.0
	9.08 nc0.5
	10.37 nc1.0
	10.40 nc2.5
	10.40 nc4.5
	10.40 nc10.0
	0.55 typical particle size
*/
const uint8_t sps30_measurement_low_particle_response_3[60] = {
	0x3f, 0xa6, 0xf0, 0xc6, 0xa1, 0xbc, 0x3f, 0xb0, 0x15, 0x5b, 0xfa, 0x25, 0x3f, 0xb0, 0x15,
	0x5b, 0xfa, 0x25, 0x3f, 0xb0, 0x15, 0x5b, 0xf2, 0x9c, 0x41, 0x11, 0x8e, 0x3b, 0x5a, 0x8c,
	0x41, 0x25, 0x8f, 0xfb, 0xbe, 0x23, 0x41, 0x26, 0xdc, 0x60, 0x68, 0xd6, 0x41, 0x26, 0xdc,
	0x6d, 0x87, 0x79, 0x41, 0x26, 0xdc, 0x74, 0xe9, 0x70, 0x3f, 0x0c, 0xd7, 0xc4, 0xf0, 0x2a};

/** measured values:
	0.98 pm1.0
	1.61 pm2.5
	2.08 pm4.0
	2.17 pm10.0
	5.28 nc0.5
	7.07 nc1.0
	7.76 nc2.5
	7.90 nc4.5
	7.92 nc10.0
	0.77 typical particle size
*/
const uint8_t sps30_measurement_low_particle_response_4[60] = {
	0x3f, 0x7a, 0x89, 0x12, 0xb2, 0xeb, 0x3f, 0xce, 0xf2, 0x00, 0x40, 0xbc, 0x40, 0x04, 0xcc,
	0xdb, 0x2e, 0xc6, 0x40, 0x0a, 0xd3, 0xd3, 0xc6, 0x89, 0x40, 0xa8, 0x4d, 0xd6, 0x0a, 0xc4,
	0x40, 0xe2, 0xab, 0x5e, 0x5a, 0xae, 0x40, 0xf8, 0x33, 0x46, 0xe4, 0x57, 0x40, 0xfc, 0xf7,
	0xba, 0x5c, 0xfd, 0x40, 0xfd, 0xc6, 0x5f, 0x4e, 0xdd, 0x3f, 0x45, 0x62, 0x0c, 0xf8, 0xe};
