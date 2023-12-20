#ifndef SPS30_VENDOR_DRIVER_MOCK_H_
#define SPS30_VENDOR_DRIVER_MOCK_H_

#include <stdint.h>
#include <stdbool.h>

/** This function resets the mock state to a known initial setup.
 *
 * This function provides a way for the tester to reset state
 * to a known starting point prior to executing a new test setup,
 * such as clearing any data in the TX/RX queues.
 */
void sps30_mock_reset_state(void);

/** Set data buffer for the next write command.
 *
 * This will set the data that will be used to compare with the next expected
 * I2C write, to make sure the driver is sending the expected values
 * over the bus.
 *
 * This function only sets a single write expectation, so calling it
 * multiple times will reset the expected value.
 *
 * @param[in] data Pointer to the comparison data buffer.
 * @param[in] length The size of the data buffer.
 */
void sps30_mock_set_i2c_write_data(const uint8_t* data, size_t length);

/** Set data buffer for the next read command.
 *
 * This will set the data that will be returned to the driver from the next
 * I2C read command. Then, you can check whether or not the driver implemented
 * the expected behavior.
 *
 * This function only sets a single read expectation, so calling it
 * multiple times will reset the returned value.
 *
 * @param[in] data Pointer to the data buffer to return
 * @param[in] length The size of the data buffer.
 */
void sps30_mock_set_i2c_read_data(const uint8_t* data, size_t length);

#endif // SPS30_VENDOR_DRIVER_MOCK_H_
