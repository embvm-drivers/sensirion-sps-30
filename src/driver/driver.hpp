/*
 * Copyright © 2021 Embedded Artistry LLC.
 * See LICENSE file for licensing information.
 */

#ifndef SPS_30_DRIVER_HPP_
#define SPS_30_DRIVER_HPP_

#include <assert.h>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <sps30_transport.hpp>

// TODO: refactor into a .cpp file??

namespace sps30
{
/// The delay between issuing a SPS30Sensor::reset() call and attempting to resume measurements
static constexpr std::chrono::duration<uint32_t, std::micro> RESET_DELAY_USEC =
	std::chrono::microseconds(100000);
/// The interval between measurements must be at least this duration
/// Datasheet specifies 1±0.04s
static constexpr std::chrono::duration<uint32_t, std::micro> MINIMUM_MEASUREMENT_DURATION_USEC =
	std::chrono::seconds(1);
/// Time between starting measurement and stable number concentration readings in the 200-3000
/// #/cm^3 range
static constexpr std::chrono::duration<uint32_t, std::micro>
	NUM_CONCENTRATION_3000_200_SETTLING_TIME_USEC = std::chrono::seconds(8);
/// Time between starting measurement and stable number concentration readings in the 100-200 #/cm^3
/// range
static constexpr std::chrono::duration<uint32_t, std::micro>
	NUM_CONCENTRATION_200_100_SETTLING_TIME_USEC = std::chrono::seconds(8);
/// Time between starting measurement and stable number concentration readings in the 50-100 #/cm^3
/// range
static constexpr std::chrono::duration<uint32_t, std::micro>
	NUM_CONCENTRATION_100_50_SETTLING_TIME_USEC = std::chrono::seconds(8);

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

/** Driver for the Sensirion SPS-30 Particulate Matter Sensor
 *
 * ## Selecting UART vs I2C
 *
 * The SPS-30 Pin 4 must be pulled to ground for the sensor to operate in I2C mode.
 * When left floating, the sensor operates in UART mode.
 * This pin must be set in hardware or configured by your BSP/hardware platform.
 *
 * ## Fan Cleaning
 *
 * Out of the factory, the fan will auto-clean once every week.
 * The counter that tracks run time is reset to 0 when the sensor is switched off.
 * If you switch the sensor off periodicially, make sure to trigger a manual
 * cleaning cycle at least once every week using startFanCleaning();
 *
 * ## Sensor Firmware Bugs
 *
 * Due to a firmware bug on FW<2.2, the reported fan cleaning interval is only
 * updated on sensor restart/reset. If the interval was thus updated after the
 * last reset, the old value is still reported. Power-cycle the sensor or call
 * sps30_reset() first if you need the latest value.
 *
 * We work around this by reading the value on probe(), and then we cache
 * any values that are set manually.
 */
class sensor
{
  public:
	/// Version format for the SPS-30 Sensor Version
	struct version_t
	{
		uint8_t major;
		uint8_t minor;
	};

	// TODO: support fixed point / uint16_t values and floating point
	// TODO: is this another secret we can hide?
	// Perhaps with a template parameter that controsl whether values are float or uint16_t?
	/* Data measurements taken from the SPS-30
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

  private:
	/// The minimum length of a buffer required to hold the serial number string
	static constexpr size_t SPS30_SERIAL_NUM_BUFFER_LEN = 32;

  public:
	sensor(transport& t) : transport_(t) {}
	~sensor() {}

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
	bool probe();

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
	void start();

	// TODO: make match the embvm expectations
	/** Stop SPS-30 device operations
	 *
	 * Stops measurements and puts the SPS-30 back into idle mode.
	 *
	 * @pre Driver has been started and not yet stopped.
	 * @post The sensor is no longer taking measurements and the
	 *  sensor is in idle mode.
	 */
	void stop();

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
	void sleep();

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
	void wake();

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
	void reset();

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
	version_t firmwareVersion() const;

	/** Retrieve the sensor's serial number
	 *
	 * @pre Sensor has been probed.
	 *
	 * @returns A string containing the device serial number.
	 */
	const char* serial() const;

	/** Check if new data is ready
	 *
	 * @pre The device has been started
	 * @post The value of the data ready register was successfully retrieved
	 *
	 * @returns true if new (not yet retrieved) measurements are available.
	 * If true, new data is available with read()
	 */
	bool dataReady();

	/** Read a measurement
	 *
	 * Reads the latest measurement available from the sensor.
	 *
	 * @pre The device has been started
	 * @post Measurements have been successfully read from the device
	 *
	 * @returns A struct that contains the measured values
	 */
	measurement_t read();

	/** Read the current auto-cleaning interval
	 *
	 * Reads the currently configured fan auto-cleaning interval. The reported value
	 * is cached after issuing probe(), and upon any successful autoCleanInterval() calls.
	 *
	 * @precondition The driver has been probed
	 *
	 * @returns interval_seconds The currently configured interval, reported in seconds
	 */
	std::chrono::duration<uint32_t> autoCleanInterval() const;

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
	std::chrono::duration<uint32_t> autoCleanInterval(const std::chrono::seconds interval_seconds);

	/** Immediately trigger the fan cleaning routine
	 *
	 * @pre The device has been started
	 * @post The fan cleaning routine has been started
	 */
	void cleanFan();

  private:
	bool started_ = false;
	bool probed_ = false;
	/// Fan auto-clean interval
	std::chrono::duration<uint32_t> fan_auto_clean_interval_seconds_;
	version_t version_;
	char serial_[SPS30_SERIAL_NUM_BUFFER_LEN] = {0};
	const transport& transport_;
};

}; // end namespace sps30

#endif // SPS_30_DRIVER_HPP_
