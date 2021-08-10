/*
 * Copyright © 2021 Embedded Artistry LLC.
 * See LICENSE file for licensing information.
 */

#ifndef SPS_30_DRIVER_HPP_
#define SPS_30_DRIVER_HPP_

#include <cstdint>
#include <cstddef>

// For now, we want to develop this class
// for I2C. Eventually we will refactor it so we can
// configure the specific transport (and it's necessary
// function pointers) on boot

// TODO: a secret we can control is whether or not the device is powered.
// Is this a separate abstraction? sps_30_power_control
// Or is it part of the transport? (mixed responsibilities)
// TODO: make this work in an async style... currently synchronous
// TODO: register a read callback
// TODO: what other calls should be async?
// TODO: should we just assert if there's a failure below? i.e., all APIs assume success?
// e.g., getFanAutoCleanInterval -> better return as an integer instead of an inout...
// or a std::pair?
// TODO: think about measurements - how can I request/report specific types of data?
// e.g., we can think about this sensor as a composite sensor supporting:
// PM1.0, PM2.5, PM4, PM10 concentrations
// PM1.0, PM2.5, PM4, PM10 counts
// Average particle size detector

/// The minimum length of a buffer required to hold the serial number string
constexpr size_t SPS30_SERIAL_NUM_BUFFER_LEN = 32;

/**
 *
 * Note that SPS-30 Pin 4 must be pulled to ground for the sensor to operate in i2c
 * i2c mode. When left floating, the sensor operates in UART mode.
 * This pin must be set in hardware or configured by your BSP/hardware platform.
 */
class SPS30Sensor
{
  public:
	/// Possible function status return values
	enum status_t {
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

	// TODO: support fixed point and floating point
	// TODO: is this another secret we can hide?
    /*
    *
    * @note Precision for mass concentration PM1 and PM2.5:
    *   [0, 100] μg/m^3 ±10 μg/m^3
    *   (100, 1000] μg/m^3 ±10% m.v.
    *
    * @note Precision for mass concentration PM4 and PM10:
    *   [0, 100] μg/m^3 ±25 μg/m^3
    *   (100, 1000] μg/m^3 ±25% m.v.
    *
    * @note Maximum-long-term mass concentration precision limit drift:
    *   [0, 100] μg/m^3 ±1.25 μg/m^3 / year
    *   (100, 1000] μg/m^3 ±1.25% m.v. / year
    *
    * @note Number concentration precision1 for PM0.5, PM1 and PM2.5:
    *   [0, 1000] #/cm^3 ±100 #/cm^3
    *   (1000, 3000] μg/m^3 ±10% m.v.
    *
    * @note Number concentration precision1 for PM4, PM10:
    *   [0, 1000] #/cm^3 ±250 #/cm^3
    *   (1000, 3000] μg/m^3 ±25% m.v.
    *
    * @note Maximum long-term number concentration precision limit drift:
    *   [0, 1000] #/cm^3 ±12.5 #/cm^3 / yr
    *   (1000, 3000] μg/m^3 ±1.25% m.v. /yr
    *
    */
	struct measurement_t
	{
        /// Mass concentration of particles in the range of 0.3 μm to 1.0 μm,
        /// reported in μg/m^3
		float mc_1p0;
        /// Mass concentration of particles in the range of 0.3 μm to 2.5 μm,
        /// reported in μg/m^3
		float mc_2p5;
        /// Mass concentration of particles in the range of 0.3 μm to 4.0 μm,
        /// reported in μg/m^3
		float mc_4p0;
        /// Mass concentration of particles in the range of 0.3 μm to 10.0 μm,
        /// reported in μg/m^3
		float mc_10p0;
        /// Number concentration of particles in the 0.3μm to 0.5μm range,
        /// reported in number per cm^3
		float nc_0p5;
        /// Number concentration of particles in the 0.3μm to 1.0μm range,
        /// reported in number per cm^3
		float nc_1p0;
        /// Number concentration of particles in the 0.3μm to 2.5μm range,
        /// reported in number per cm^3
		float nc_2p5;
        /// Number concentration of particles in the 0.3μm to 4.0μm range,
        /// reported in number per cm^3
		float nc_4p0;
        /// Number concentration of particles in the 0.3μm to 10.0μm range,
        /// reported in number per cm^3
		float nc_10p0;
        /// Typical particle size in μm
		float typical_particle_size;
	};

	public : SPS30Sensor();
	~SPS30Sensor();

	/** Check if SPS-30 sensor is available on the specified transport
	 *
	 * @returns true if the sensor is detected, false otherwise
	 */
	bool probe();

	// TODO: make match the embvm expectations
	/** Initialize the SPS-30 senso
	 *
	 * Initializes the SPS-30 and places it into an operational state, where it is
	 * taking measurements
	 *
	 * @post Device is initialized and taking measurements.
	 *
	 * @note Once the driver is started, measurements are retrievable once per second
	 * via read().
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t start();

	// TODO: make match the embvm expectations
	/** Stop SPS-30 device operations
	 *
	 * Stops measurements and puts the SPS-30 back into idle mode.
	 *
	 * @pre Driver has been started and not yet stopped.
	 * @post The sensor is no longer taking measurements and the
	 *  sensor is in idle mode.
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t stop();

	/** Read the sensor firmware version
	 *
	 * Reads the firmware version reported by the sensor
	 *
	 * @param [out] major:  Placeholder for the reported major version number
	 * @param [out] minor:  Placeholder for the reported minor version numbe
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t firmwareVersion(uint8_t& major, uint8_t& minor);

	/** Retrieve the sensor's serial number
	 *
	 * @pre sizeof(serial buffer) >= SPS30_SERIAL_NUM_BUFFER_LEN bytes
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 *  @note that serial data must be discarded with the return value is not status_t::OK
	 */
	status_t serial(char* serial, size_t serial_buffer_size);

	/** Check if new data is ready
	 *
	 * @pre The device has been started
	 *
	 * @param [out] data_ready A flag indicating whether new (not yet retrieved) measurements are
	 * available. If true, new data is available with read()
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t dataReady(bool& data_ready);

	/** Read a measurement
	 *
	 * Reads the latest measurement available from the sensor.
	 *
	 * @pre The device has been started
	 *
	 * @param [out] measurement A struct that will contain the measured values
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t read(measurement_t& measurement);

	/** Read the current auto-cleaning interval
	 *
	 * Reads the currently configured fan auto-cleaning interval
	 *
	 * @note Due to a firmware bug on FW<2.2, the reported interval is only
	 * updated on sensor restart/reset. If the interval was thus updated after the
	 * last reset, the old value is still reported. Power-cycle the sensor or call
	 * sps30_reset() first if you need the latest value.
	 *
	 * @param [out] interval_seconds Placeholder for the currently configured interval,
	 *  reported in sconds
	 * @returns status_t::OK on success, or an appropriate error value.
	 *  @note interval_seconds must be discarded when the return code is
	 * non-zero.
	 */
	status_t getFanAutoCleanInterval(uint32_t& interval_seconds);

	/** Set the fan auto-cleaning interval
	 *
	 * Set (or disable) the fan auto-cleaning interval.
	 *
	 * @param [in] interval_seconds The interval (in seconds) between fan auto-cleaning events
	 *  @note 0 will disable auto-cleaning
	 *
	 * @returns status_t::OK on success, or an appropriate error value.
	 */
	status_t setFanAutoCleanInterval(const uint32_t interval_seconds);

	/** Set the auto-cleaning interval in days
	 *
	 * This is a convenience function to read the current auto-cleaning interval in days rather than
	 * in seconds.
	 *
	 * @note The value is a simple estimate with the fractional piece dropped. It is not rounded
	 *  nicely. If a more precise answer is required, use getFanAutoCleanInterval().
	 *
	 * @note Please see the note for getFanAutoCleanInterval() on sensor firmware bugs.
	 *
	 * Note that interval_days must be discarded when the return code is non-zero.

	 *
	 * @param [out] interval_days Placeholder for the currently configured interval, reported
	 *  (roughly) in days
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t getFanAutoCleanIntervalInDays(uint8_t& interval_days);

	/** Set the auto-cleaning interval in days
	 *
	 * This is a convenience function to set the auto-cleaning interval in days rather than in
	 * seconds
	 *
	 * @param [in] interval_days Value in days specifying the interval for auto-cleaning.
	 *  @note A value of 0 disables auto-cleaning
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t setFanAutoCleanIntervalInDays(const uint8_t interval_days);

	/** Immediately trigger the fan cleaning routine
	 *
	 * @pre The device has been started
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t startManualFanClean();

	/** Reset the sensor
	 *
	 * Resets the sensor, placing it back into the state it was in before the
	 * reset instruction was issued. The caller should
	 *
	 * @sideeffect The sensor reboots to the same state as before the reset but takes a few
	 * seconds to resume measurements.
	 *
	 * The caller should wait at least SPS30_RESET_DELAY_USEC microseconds before
	 * interacting with the sensor again in order for the sensor to restart.
	 * Interactions with the sensor before this delay might fail.
	 *
	 * Note that the interface-select configuration is reinterpreted, thus Pin 4
	 * must be pulled to ground during the reset period for the sensor to remain in
	 * i2c mode.
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t reset();

	/** Send a stopped sensor to sleep
	 *
	 * The sensor will reduce its power consumption to a minimum, but must be woken
	 * up again with wake() prior to resuming operations.
	 *
	 * @pre Device is stopped
	 * @sideeffect Device is placed into sleep mode
	 *
	 * @note This command only works on firmware 2.0 or newer
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t sleep();

	/** Wake up the sensor from sleep mode
	 *
	 * Use this command to wake up the sensor from sleep mode into idle mode.
	 *
	 * @pre Device is stopped
	 * @pre Device is in sleep mode
	 * @post Device is in idle mode and can be started.
	 *
	 * @note This command only works on firmware 2.0 or newer
	 *
	 * @returns status_t::OK on success, or an error indicating the source of the failure
	 */
	status_t wake();

  private:
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

  private:
	bool started_ = false;
};

#endif // SPS_30_DRIVER_HPP_
