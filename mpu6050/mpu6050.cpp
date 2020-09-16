#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "kidbright32.h"
#include "MPU6050.h"
#include "esp_log.h"

MPU6050::MPU6050(int bus_ch, int dev_addr) {
	channel = bus_ch;
	address = dev_addr;
	polling_ms = MPU6050_POLLING_MS;
}

void i2c_setClock(uint32_t clock) {
	// Reset speed of I2C
	i2c_config_t conf;

	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = CHAIN_SDA_GPIO;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = CHAIN_SCL_GPIO;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = clock;

	i2c_param_config(I2C_NUM_1, &conf);
}

void MPU6050::init(void) {
	esp_log_level_set("*", ESP_LOG_VERBOSE);
	i2c_setClock(100E3);

	state = s_detect;
}

int MPU6050::prop_count(void) {
	return 0;
}

bool MPU6050::prop_name(int index, char *name) {
	// not supported
	return false;
}

bool MPU6050::prop_unit(int index, char *unit) {
	// not supported
	return false;
}

bool MPU6050::prop_attr(int index, char *attr) {
	// not supported
	return false;
}

bool MPU6050::prop_read(int index, char *value) {
	// not supported
	return false;
}

bool MPU6050::prop_write(int index, char *value) {
	// not supported
	return false;
}

void MPU6050::process(Driver *drv) {
	I2CDev *i2c = (I2CDev *)drv;
	
	switch (state) {
		case s_detect:
			// detect i2c device
			if (i2c->detect(channel, address) == ESP_OK) {	
				uint8_t buff[2] = { 
					0x6B,
					0x00
				};

				if (i2c->write(channel, address, buff, 2) != ESP_OK) {
					state = s_error;
					break;
				}

				_set_acc_new_range_flag = true;
				_set_gyro_new_range_flag = true;
				
				tickcnt = get_tickcnt() + 10000;
				state = s_read;
			} else {
				state = s_error;
			}
			break;
		
		case s_read: {
			if (!is_tickcnt_elapsed(tickcnt, polling_ms)) {
				break;
			}
			tickcnt = get_tickcnt();

			if (_set_acc_new_range_flag) {
				uint8_t buff[2] = { 
					0x1C,  // ACCEL_CONFIG
					0 << 3 // 2g
				};

				// MPU-6000-Register-Map1.pdf page.15
				if (_acc_range == 2)  buff[1] = 0 << 3;
				if (_acc_range == 4)  buff[1] = 1 << 3;
				if (_acc_range == 8)  buff[1] = 2 << 3;
				if (_acc_range == 16) buff[1] = 3 << 3;


				if (i2c->write(channel, address, buff, 2) != ESP_OK) {
					state = s_error;
					break;
				}
			}
			
			if (_set_gyro_new_range_flag) {
				uint8_t buff[2] = { 
					0x1B,  // GYRO_CONFIG
					0 << 3 // 250 */sec
				};

				// MPU-6000-Register-Map1.pdf page.14
				if (_gyro_range == 250)  buff[1] = 0 << 3;
				if (_gyro_range == 500)  buff[1] = 1 << 3;
				if (_gyro_range == 1000) buff[1] = 2 << 3;
				if (_gyro_range == 2000) buff[1] = 3 << 3;

				if (i2c->write(channel, address, buff, 2) != ESP_OK) {
					state = s_error;
					break;
				}
			}
			
			uint8_t reg = 0x3B; // 
			uint8_t buffer[14];
			if (i2c->read(channel, address, &reg, 1, buffer, 14) == ESP_OK) {
				for (int i=0;i<3;i++) {
					int16_t raw = (buffer[(i * 2) + 0] << 8) | buffer[(i * 2) + 1];

					// MPU-6000-Register-Map1.pdf page.29
					if (_acc_range == 2)  _acc[i] = (raw / 16384.0) * 1000.0;
					if (_acc_range == 4)  _acc[i] = (raw / 8192.0) * 1000.0;
					if (_acc_range == 8)  _acc[i] = (raw / 4096.0) * 1000.0;
					if (_acc_range == 16) _acc[i] = (raw / 2048.0) * 1000.0;
					ESP_LOGI("MPU-6050", "Acc RAW: %x", buffer[(i * 2) + 1]);
				}
				_acc[3] = sqrt((_acc[0] * _acc[0]) + (_acc[1] * _acc[1]) + (_acc[2] * _acc[2]));


				_temp = ((int16_t)((buffer[6] << 8) | buffer[7])) / 340.00 + 36.53; // MPU-6000-Register-Map1.pdf page.30

				for (int i=0;i<3;i++) {
					int16_t raw = (buffer[(i * 2) + 8] << 8) | buffer[(i * 2) + 9];

					// MPU-6000-Register-Map1.pdf page.31
					if (_gyro_range == 250)  _gyro[i] = raw / 131.0;
					if (_gyro_range == 500)  _gyro[i] = raw / 65.5;
					if (_gyro_range == 1000) _gyro[i] = raw / 32.8;
					if (_gyro_range == 2000) _gyro[i] = raw / 16.4;
					ESP_LOGI("MPU-6050", "Gyro RAW: %d", raw);
				}
				_gyro[3] = sqrt((_gyro[0] * _gyro[0]) + (_gyro[1] * _gyro[1]) + (_gyro[2] * _gyro[2]));
			
				// set initialized flag
				initialized = true;
					
				// clear error flag
				error = false;
			} else {
				state = s_error;
			}
			
			break;
		}

		case s_error:
			// set error flag
			error = true;
			// clear initialized flag
			initialized = false;
			// get current tickcnt
			tickcnt = get_tickcnt();
			// goto wait and retry with detect state
			state = s_wait;
			break;

		case s_wait:
			// wait polling_ms timeout
			if (is_tickcnt_elapsed(tickcnt, polling_ms)) {
				state = s_detect;
			}
			break;
	}
}

float MPU6050::get_acceleration(uint8_t axis) {
	if (error) {
		return 99999;
	}
	
	return _acc[axis - 1];
}

float MPU6050::get_gyroscope(uint8_t axis) {
	if (error) {
		return 99999;
	}
	
	return _gyro[axis - 1];
}

float MPU6050::get_temperature() {
	if (error) {
		return 99999;
	}

	return _temp;
}

void MPU6050::set_acc_range(uint8_t range) {
	if (range == 2 || range == 4 || range == 8 || range == 16) {
		_set_acc_new_range_flag = true;
		_acc_range = range;
	}
}

void MPU6050::set_gyro_range(uint16_t range) {
	if (range == 250 || range == 500 || range == 1000 || range == 2000) {
		_set_gyro_new_range_flag = true;
		_gyro_range = range;
	}
}


