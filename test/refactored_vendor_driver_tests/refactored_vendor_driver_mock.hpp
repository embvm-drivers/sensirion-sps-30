#ifndef SPS30_VENDOR_DRIVER_MOCK_H_
#define SPS30_VENDOR_DRIVER_MOCK_H_

#include <stdint.h>
#include <stdbool.h>

/** This function resets the mock state to a known initial setup.
 *
 * Sometimes, we may run multiple test scenarios that require
 * resetting the internal state of the mock, such as testing whether
 * different init/de-init paths properly initialize and release the
 * I2C bus.
 *
 * This function provides a way for the tester to reset state
 * to a known starting point prior to executing a new test setup.
 */
void sps30_mock_reset_state(void);

/** Check whether the I2C bus initialization function was called.
 *
 * @note Right now, this is just a boolean flag. If you wanted to check for the
 * number of times a function was invoked, you could use a numeric type
 * instead and set a counter.
 *
 * @returns true if the function was called, false otherwise.
 */
bool sps30_mock_i2c_initialized(void);

/** Check whether the I2C bus release function was called.
 *
 * @note Right now, this is just a boolean flag. If you wanted to check for the
 * number of times a function was invoked, you could use a numeric type
 * instead and set a counter.
 *
 * @returns true if the function was called, false otherwise.
 */
bool sps30_mock_i2c_released(void);

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
