#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "math.h"
#include "driver.h"
#include "device.h"
#include "i2c-dev.h"

#define MPU6050_POLLING_MS 100

class MPU6050 : public Device {
	private:
		enum {
			s_detect, 
			s_read, 
			s_error, 
			s_wait
		} state;
		TickType_t tickcnt, polling_tickcnt;
		
		int _acc[4] = { 0, 0, 0, 0};
		int _gyro[4] = { 0, 0, 0, 0};
		float _temp = 0;

		bool _set_acc_new_range_flag = false;
		bool _set_gyro_new_range_flag = false;
		int _acc_range = 2;
		int _gyro_range = 250;
		
	public:
		// constructor
		MPU6050(int bus_ch, int dev_addr);
		// override
		void init(void);
		void process(Driver *drv);
		int prop_count(void);
		bool prop_name(int index, char *name);
		bool prop_unit(int index, char *unit);
		bool prop_attr(int index, char *attr);
		bool prop_read(int index, char *value);
		bool prop_write(int index, char *value);
		
		// method
		float get_acceleration(uint8_t axis) ;
		float get_gyroscope(uint8_t axis) ;
		float get_temperature() ;

		void set_gyro_range(uint16_t range) ;
		void set_acc_range(uint8_t range) ;
		
};

#endif
