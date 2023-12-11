/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h> // printf

#include "sps30.h"

/**
 * TO USE CONSOLE OUTPUT (printf) PLEASE ADAPT TO YOUR PLATFORM:
 * #define printf(...)
 */

int main(void)
{
	struct sps30_measurement m;
	int16_t ret;

	/* Initialize I2C bus */
	sensirion_i2c_init();

	/* Busy loop for initialization, because the main loop does not work without
	 * a sensor.
	 */
	while(sps30_probe() != 0)
	{
		printf("SPS sensor probing failed\n");
		sensirion_sleep_usec(1000000); /* wait 1s */
	}
	printf("SPS sensor probing successful\n");

	uint8_t fw_major;
	uint8_t fw_minor;
	ret = sps30_read_firmware_version(&fw_major, &fw_minor);
	if(ret)
	{
		printf("error reading firmware version\n");
	}
	else
	{
		printf("FW: %u.%u\n", fw_major, fw_minor);
	}

	char serial_number[SPS30_MAX_SERIAL_LEN];
	ret = sps30_get_serial(serial_number);
	if(ret)
	{
		printf("error reading serial number\n");
	}
	else
	{
		printf("Serial Number: %s\n", serial_number);
	}

	printf("Getting Fan Auto Cleaning Interval\n");
	uint32_t interval;
	sps30_get_fan_auto_cleaning_interval(&interval);
	printf("Interval: %d s\n", interval);

	printf("Getting Fan Auto Cleaning Interval in Days\n");
	uint8_t interval_days;
	sps30_get_fan_auto_cleaning_interval_days(&interval_days);
	printf("Interval: %d days\n", interval_days);

	printf("Setting Fan Auto Cleaning Interval\n");
	interval = 39288;
	sps30_set_fan_auto_cleaning_interval(interval);

	printf("Getting Fan Auto Cleaning Interval\n");
	interval = 0;
	sps30_get_fan_auto_cleaning_interval(&interval);
	printf("Interval: %d s\n", interval);

	printf("Getting Fan Auto Cleaning Interval in Days\n");
	sps30_get_fan_auto_cleaning_interval_days(&interval_days);
	printf("Interval: %d days\n", interval_days);

	printf("Setting Fan Auto Cleaning Interval in Days\n");
	interval_days = 2;
	sps30_set_fan_auto_cleaning_interval_days(interval_days);

	printf("Getting Fan Auto Cleaning Interval in Days\n");
	sps30_get_fan_auto_cleaning_interval_days(&interval_days);
	printf("Interval: %d days\n", interval_days);

	printf("Issuing manual fan cleaning command\n");
	sps30_start_manual_fan_cleaning();
	// 10s wait is what I empirically measured here.
	sensirion_sleep_usec(SPS30_MANUAL_FAN_CLEANING_DURATION);
	printf("Fan cleaning completed\n");

	uint32_t device_status_flags;
	printf("Reading device status register\n");
	ret = sps30_read_device_status_register(&device_status_flags);
	if(ret < 0)
		printf("error reading device status register\n");
	else
		printf("device status register: 0x%x\n", device_status_flags);

	ret = sps30_start_measurement();
	if(ret < 0)
		printf("error starting measurement\n");
	printf("measurements started\n");

	printf("Reading device status register\n");
	ret = sps30_read_device_status_register(&device_status_flags);
	if(ret < 0)
		printf("error reading device status register\n");
	else
		printf("device status register: 0x%x\n", device_status_flags);

	int i = 5;

	while(--i)
	{
		uint16_t data_ready = false;

		while(!data_ready)
		{
			ret = sps30_read_data_ready(&data_ready);
			if(ret)
			{
				printf("Error getting data ready: %d\n", ret);
			}
			else
			{
				printf("Data ready: %d\n", data_ready);
			}

			// We want to intentionally trigger a short sleep here
			// so we can see various data ready outputs.
			sensirion_sleep_usec(SPS30_MEASUREMENT_DURATION_USEC >> 1);
		}

		// Data ready, let's get some measurements
		ret = sps30_read_measurement(&m);
		if(ret < 0)
		{
			printf("error reading measurement\n");
		}
		else
		{
#ifdef LOW_PRECISION_PRINTING
			printf("measured values:\n"
				   "\t%0.2f pm1.0\n"
				   "\t%0.2f pm2.5\n"
				   "\t%0.2f pm4.0\n"
				   "\t%0.2f pm10.0\n"
				   "\t%0.2f nc0.5\n"
				   "\t%0.2f nc1.0\n"
				   "\t%0.2f nc2.5\n"
				   "\t%0.2f nc4.5\n"
				   "\t%0.2f nc10.0\n"
				   "\t%0.2f typical particle size\n\n",
				   (double)m.mc_1p0, (double)m.mc_2p5, (double)m.mc_4p0, (double)m.mc_10p0,
				   (double)m.nc_0p5, (double)m.nc_1p0, (double)m.nc_2p5, (double)m.nc_4p0,
				   (double)m.nc_10p0, (double)m.typical_particle_size);
#else
			printf("measured values:\n"
				   "\t%0.16f pm1.0\n"
				   "\t%0.16f pm2.5\n"
				   "\t%0.16f pm4.0\n"
				   "\t%0.16f pm10.0\n"
				   "\t%0.16f nc0.5\n"
				   "\t%0.16f nc1.0\n"
				   "\t%0.16f nc2.5\n"
				   "\t%0.16f nc4.5\n"
				   "\t%0.16f nc10.0\n"
				   "\t%0.16f typical particle size\n\n",
				   (double)m.mc_1p0, (double)m.mc_2p5, (double)m.mc_4p0, (double)m.mc_10p0,
				   (double)m.nc_0p5, (double)m.nc_1p0, (double)m.nc_2p5, (double)m.nc_4p0,
				   (double)m.nc_10p0, (double)m.typical_particle_size);
#endif
		}
	}

	printf("Issuing sleep command, expected to fail\n");
	ret = sps30_sleep();
	printf("sleep returned: %d\n", ret);

	printf("Stopping Measurements\n");
	sps30_stop_measurement();

	printf("Reading device status register\n");
	ret = sps30_read_device_status_register(&device_status_flags);
	if(ret < 0)
		printf("error reading device status register\n");
	else
		printf("device status register: 0x%x\n", device_status_flags);

	printf("Resetting Sensor\n");
	sps30_reset();

	printf("Issuing sleep command, expected to succeed\n");
	ret = sps30_sleep();
	printf("sleep returned: %d\n", ret);

	sensirion_sleep_usec(SPS30_RESET_DELAY_USEC);

	printf("Issuign wakup command\n");
	ret = sps30_wake_up();
	printf("wakeup returned: %d\n", ret);

	return 0;
}
