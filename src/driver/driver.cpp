#include <driver.hpp>
#include <chrono>

/// The delay between issuing a SPS30Sensor::reset() call and attempting to resume measurements
static constexpr std::chrono::duration<uint32_t, std::micro> RESET_DELAY_USEC = std::chrono::microseconds(100000);
/// The interval between measurements must be at least this duration
/// Datasheet specifies 1±0.04s
static constexpr std::chrono::duration<uint32_t, std::micro> MINIMUM_MEASUREMENT_DURATION_USEC = std::chrono::seconds(1);
/// Time between starting measurement and stable number concentration readings in the 200-3000 #/cm^3 range
static constexpr std::chrono::duration<uint32_t, std::micro> NUM_CONCENTRATION_3000_200_SETTLING_TIME_USEC = std::chrono::seconds(8);
/// Time between starting measurement and stable number concentration readings in the 100-200 #/cm^3 range
static constexpr std::chrono::duration<uint32_t, std::micro> NUM_CONCENTRATION_200_100_SETTLING_TIME_USEC = std::chrono::seconds(8);
/// Time between starting measurement and stable number concentration readings in the 50-100 #/cm^3 range
static constexpr std::chrono::duration<uint32_t, std::micro> NUM_CONCENTRATION_100_50_SETTLING_TIME_USEC = std::chrono::seconds(8);

#pragma mark - Definitions -

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
enum sps30_device_status_mask_t
{
	/// The fan is switched on but not running
	FAN_ERROR = (1 << 4),
	/// The laser is currently out of range
	LASER_ERROR = (1 << 5),
	/// The fan speed is out of range
	FAN_SPEED_WARNING = (1 << 21)
};

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
