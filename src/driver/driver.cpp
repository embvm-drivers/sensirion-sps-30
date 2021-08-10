#include <driver.hpp>

// TODO: support fixed point and floating point
// TODO: is this another secret we can hide?

#pragma mark - Definitions -

enum sps30_device_status_mask_t
{
	FAN_ERROR = (1 << 4),
	LASER_ERROR = (1 << 5),
	FAN_SPEED_WARNING = (1 << 21)
};
