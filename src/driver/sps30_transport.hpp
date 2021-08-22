#ifndef SPS30_TRANSPORT_INTERFACE_HPP_
#define SPS30_TRANSPORT_INTERFACE_HPP_

#include <stddef.h>
#include <stdint.h>

namespace sps30
{
// TODO: refactor this class for being asynchronous
// TODO: ultimately use CRTP here?
// https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/ OR:
// https://www.fluentcpp.com/2020/09/11/replacing-crtp-static-polymorphism-with-concepts/
class transport
{
  public:
	/// Possible transport function status return values
	enum status_t
	{
		/// Operation was successful
		OK = 0,
		/// An error occurred while transmitting or receiving data on the hardware bus
		BUS_ERROR,
		/// The driver's state is not valid for this operation
		/// E.g., trying to call read() when the driver that has not been started
		INVALID_STATE,
		/// The device firmware does not support the specified command
		SENSOR_FIRWMARE_DOES_NOT_SUPPORT_COMMAND,
		/// The action you are attempting is not currently implemented
		NOT_IMPLEMENTED,
		/// An error occurred, but its details are not specified
		UNKNOWN_ERROR,
	};

	/// Generic representation of commands that can be issued across the SPS-30
	/// transport layer.
	enum command_t
	{
		SPS30_CMD_START_MEASUREMENT = 0,
		SPS30_CMD_START_MEASUREMENT_ARG,
		SPS30_CMD_STOP_MEASUREMENT,
		SPS30_CMD_READ_MEASUREMENT,
		SPS30_CMD_GET_DATA_READY,
		SPS30_CMD_AUTOCLEAN_INTERVAL,
		SPS30_CMD_GET_FIRMWARE_VERSION,
		SPS30_CMD_GET_SERIAL,
		SPS30_CMD_RESET,
		SPS30_CMD_SLEEP,
		SPS30_CMD_READ_DEVICE_STATUS_REG,
		SPS30_CMD_START_MANUAL_FAN_CLEANING,
		SPS30_CMD_WAKE_UP,
	};

  public:
	/** Read data over the transport
	 *
	 * @param [out] data Pointer to the buffer where the data should be stored
	 * @param []
	 *
	 * @returns a status_t value indiating the state of the transfer
	 */
	status_t read(const command_t command, uint8_t* const data, const size_t length) const;

	/** Write data over the transport
	 *
	 * @returns a status_t value indiating the state of the transfer
	 */
	status_t write(const command_t command, const uint8_t* const data, const size_t length) const;

	/** Send and recieve data over the transport
	 *
	 * @returns a status_t value indiating the state of the transfer
	 */
	status_t transcieve(const command_t command, const uint8_t* const tx_data,
						const size_t tx_length, uint8_t* const rx_data,
						const size_t rx_length) const;
};

}; // end namespace sps30

#endif // SPS30_TRANSPORT_INTERFACE_HPP_
