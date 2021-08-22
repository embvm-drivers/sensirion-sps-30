#include <driver.hpp>

using namespace sps30;

#if 0
SPEED: Fan speed out of range
0: Fan speed is ok.
1: Fan speed is too high or too low.
During the first 3 seconds after starting the measurement (fan start-up) the fan speed is not checked.
The fan speed is also not checked during the auto cleaning procedure.
Apart from the two exceptions mentioned above, the fan speed is checked once per second in the measurement mode. If it is out of range twice in succession, the SPEED-bit is set.
At very high or low ambient temperatures, the fan may take longer to reach its target speed after start-up. In this case, the bit will be set. As soon as the target speed is reached, this bit is cleared automatically.
If this bit is constantly set, this indicates a problem with the power supply or that the fan is no longer working properly.
LASER: Laser failure
0: Laser current is ok.
1: Laser is switched on and current is out of range.
The laser current is checked once per second in the measurement mode. If it is out of range twice in succession, the LASER-bit is set.
If the laser current is back within limits, this bit will be cleared automatically.
A laser failure can occur at very high temperatures outside of specifications or when the laser module is defective.
FAN: Fan failure, fan is mechanically blocked or broken.
0: Fan works as expected.
1: Fan is switched on, but the measured fan speed is 0 RPM.
The fan is checked once per second in the measurement mode. If 0 RPM is measured twice in succession, the FAN bit is set.
The FAN-bit will not be cleared automatically.
A fan failure can occur if the fan is mechanically blocked or broken.
#endif
enum device_status_mask_t
{
	/// The fan is switched on but not running
	FAN_ERROR = (1 << 4),
	/// The laser is currently out of range
	LASER_ERROR = (1 << 5),
	/// The fan speed is out of range
	FAN_SPEED_WARNING = (1 << 21)
};

void readSerial_(const transport& t, char* const serial_buffer, size_t max_len)
{
	auto status = t.read(transport::command_t::SPS30_CMD_GET_SERIAL,
						 reinterpret_cast<uint8_t*>(serial_buffer), max_len);

	assert(status == transport::status_t::OK);
}

void readFirmwareVersion_(const transport& t, sensor::version_t& v)
{
	uint16_t reported_value;
	auto status = t.read(transport::command_t::SPS30_CMD_GET_FIRMWARE_VERSION,
						 reinterpret_cast<uint8_t*>(&reported_value), sizeof(uint16_t));
	assert(status == transport::status_t::OK);

	v.major = (reported_value & 0xff00) >> 8;
	v.minor = (reported_value & 0x00ff);
}

void readFanAutoCleanInterval_(const transport& t, std::chrono::duration<uint32_t>& d)
{
	uint32_t value;
	auto status = t.read(transport::command_t::SPS30_CMD_AUTOCLEAN_INTERVAL,
						 reinterpret_cast<uint8_t*>(&value), sizeof(uint32_t));
	assert(status == transport::status_t::OK);
	// TODO: endianness swap? handle in transport?
#if 0
uint32_t sensirion_bytes_to_uint32_t(const uint8_t* bytes) {
    return (uint32_t)bytes[0] << 24 | (uint32_t)bytes[1] << 16 |
           (uint32_t)bytes[2] << 8 | (uint32_t)bytes[3];
#endif
	d = std::chrono::duration<uint32_t>(value);
}

#if 0
	/** Read the Device Status Register
	 *
	 * Reads the Device Status Registerm which reveals info, warnings and errors
	 * about the sensor's current operational state. Note that the flags are
	 * self-clearing, i.e. they reset to 0 when the condition is resolved.
	 *
	 * @note This command only works on firmware 2.2 or more recent.
	 *
	 * @param [out] device_status_flags Memory where the register value will be written to
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t readStatusRegister(uint32_t& device_status_flags);
#endif

#if 0
6.3.1 Start Measurement (0x0010)
Starts the measurement. After power up, the module is in Idle-Mode. Before any measurement values can be read, the Measurement-Mode needs to be started using this command.
Transfer Type: Set Pointer & Write Data Pointer Address: 0x0010
Write Data:
0 Measurement Output Format
0x03: Big-endian IEEE754 float values
0x05: Big-endian unsigned 16-bit integer values
1 dummy byte, insert 0x00
2 Checksum for bytes 0, 1
#endif

/** Check if SPS-30 sensor is available, and if so, pre-load values
 *
 * This call checks to see whether or not the SPS-30 is available on the bus.
 * If so, we will pre-fetch the following information:
 *
 * - Firmware version
 * - Sensor serial number
 * - Auto-cleaning interval
 *
 * @postcondition The firmware version, serial number, and auto-clean interval will be
 * cached and available to the user.
 *
 * @returns true if the sensor is detected, false otherwise
 */
bool sensor::probe()
{
	// TODO: try to wake up, but ignore failure if it is not in sleep mode?

	// TODO: how to detect if the device isn't present? transport-check API?

	// As part of probing, we read and cache the following information
	readSerial_(transport_, serial_, SPS30_SERIAL_NUM_BUFFER_LEN);
	readFirmwareVersion_(transport_, version_);
	readFanAutoCleanInterval_(transport_, fan_auto_clean_interval_seconds_);

	probed_ = true;

	return probed_;
}

// TODO: make match the embvm expectations
/** Initialize the SPS-30 senso
 *
 * Initializes the SPS-30 and places it into an operational state, where it is
 * taking measurements
 *
 * TODO: does this just call probe()? then we eliminate the probe public interface?
 *
 * @post Device is initialized and taking measurements.
 *
 * @note Once the driver is started, measurements are retrievable once per second
 * via read().
 */
void sensor::start()
{
	assert(0);
}

// TODO: make match the embvm expectations
/** Stop SPS-30 device operations
 *
 * Stops measurements and puts the SPS-30 back into idle mode.
 *
 * @pre Driver has been started and not yet stopped.
 * @post The sensor is no longer taking measurements and the
 *  sensor is in idle mode.
 */
void sensor::stop()
{
	assert(0);
}

/** Send a stopped sensor to sleep
 *
 * The sensor will reduce its power consumption to a minimum, but must be woken
 * up again with wake() prior to resuming operations.
 *
 * @pre Device is stopped
 * @post Device has successfully been issued a command to enter sleep mode
 * @sideeffect Device is placed into sleep mode
 *
 * @note This command only works on firmware 2.0 or newer
 */
void sensor::sleep()
{
	assert(0);
}

/** Wake up the sensor from sleep mode
 *
 * Use this command to wake up the sensor from sleep mode into idle mode.
 *
 * @pre Device is stopped
 * @pre Device is in sleep mode
 * @post The device has been issued the wakeup sequence
 * @post Device is in idle mode and can be started.
 *
 * @note This command only works on firmware 2.0 or newer
 */
void sensor::wake()
{
	assert(0);
}

/** Reset the sensor
 *
 * Resets the sensor, placing it back into the state it was in before the
 * reset instruction was issued. The caller should
 *
 * @post The sensor has been issued a restart command.
 * @sideeffect The sensor reboots to the same state as before the reset but takes a few
 * seconds to resume measurements. A delay of SPS30_RESET_DELAY_USEC microseconds will
 * occur before sensor interactions and data can resume. Interactions with the sensor before
 * this duration has elapsed are likely to fail.
 *
 * @note During reset, the interface-select configuration is reinterpreted, thus Pin 4
 *  must remain in the selected state during the reset period.
 */
void sensor::reset()
{
	assert(0);
}

/** Read the sensor firmware version
 *
 * Reads the firmware version reported by the sensor.
 *
 * @pre Sensor has been probed.
 *
 * @returns An sps30::version_t structure instance containing the major and minor version
 * reported by firmware.
 *
 */
sensor::version_t sensor::firmwareVersion() const
{
	assert(probed_);

	return version_;
}

/** Retrieve the sensor's serial number
 *
 * @pre Sensor has been probed.
 *
 * @returns A string containing the device serial number.
 */
const char* sensor::serial() const
{
	assert(probed_);

	return serial_;
}

/** Check if new data is ready
 *
 * @pre The device has been started
 * @post The value of the data ready register was successfully retrieved
 *
 * @returns true if new (not yet retrieved) measurements are available.
 * If true, new data is available with read()
 */
bool sensor::dataReady()
{
	assert(0);
	return false;
}

/** Read a measurement
 *
 * Reads the latest measurement available from the sensor.
 *
 * @pre The device has been started
 * @post Measurements have been successfully read from the device
 *
 * @returns A struct that contains the measured values
 */
sensor::measurement_t sensor::read()
{
	assert(0);
	return {};
}

/** Read the current auto-cleaning interval
 *
 * Reads the currently configured fan auto-cleaning interval. The reported value
 * is cached after issuing probe(), and upon any successful autoCleanInterval() calls.
 *
 * @precondition The driver has been probed
 *
 * @returns interval_seconds The currently configured interval, reported in seconds
 */
std::chrono::duration<uint32_t> sensor::autoCleanInterval() const
{
	return fan_auto_clean_interval_seconds_;
}

/** Set the fan auto-cleaning interval
 *
 * Set (or disable) the fan auto-cleaning interval. The default interval is 604,800 seconds
 * (168 hours or 1 week), with a tolerance of ±3%. Once programmed, the interval value
 * is stored permanently in non-volatile memory.
 *
 * @note The counter is reset to 0 when the sensor is switched off. If you switch the sensor
 * off periodicially, make sure to trigger a manual cleaning cycle once every week.
 *
 * @param [in] interval_seconds The interval (in seconds) between fan auto-cleaning events
 *  @note 0 will disable auto-cleaning
 *
 * @returns interval_seconds The currently configured interval, reported in seconds.
 *  A value that does not match the requested interval indicates that an error occurred
 *  when setting the value.
 */
std::chrono::duration<uint32_t>
	sensor::autoCleanInterval(const std::chrono::seconds interval_seconds)
{
	assert(0);
	return std::chrono::duration<uint32_t>(0);
}

/** Immediately trigger the fan cleaning routine
 *
 * @pre The device has been started
 * @post The fan cleaning routine has been started
 */
void sensor::cleanFan()
{
	assert(0);
}
