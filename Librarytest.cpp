	pca.write_16(LED0_ON_L, LED0_ON_H, 0x00,  0x00); // HIGH value
	hwlib::cout << pca.read(LED0_ON_L) << "\t" <<  pca.read(LED0_ON_H) << "\n";
	pca.write_16(LED0_OFF_L, LED0_OFF_H, 0x00,  0x00); // LOW value
	hwlib::cout << pca.read(LED0_OFF_L) << "\t" <<  pca.read(LED0_OFF_H) << "\n";

	void set_pwm(uint8_t pin, uint16_t led_on, uint16_t led_off){
	if (pin < 1) {
        pin = 1;
    } 
	else if (pin > 16) {
        pin = 16;
    }
    pin = pin - 1;

    size_t buffer[4];

    buffer[0] = led_on;
    buffer[1] = led_on >> 8;
    buffer[2] = led_off;
    buffer[3] = led_off >> 8;
//	
//	uint8_t vier = 0x04;
//	uint8_t test = (vier * pin);
	
	transactie.write(LED0_ON_L + 4* pin , buffer);
	}
	


	
	void set_ServoPulseRange(uint16_t min_pulse_width, uint16_t max_pulse_width, uint16_t max_servo_degree){
		float min;
		float max;
		min = (2.46 * min_pulse_width) / 10.0 - 1;
		max = (2.46 * max_pulse_width) / 10.0 - 1;
		_min_pulse_width = min;
		_max_pulse_width = max;
		_max_servo_degree = max_servo_degree;
	}
	
	void setAngle(uint8_t pin, uint16_t angle) {
		if (angle > _max_servo_degree) {
			angle = _max_servo_degree;
		}
		set_pwm(pin, 0, angle * ((_max_pulse_width - _min_pulse_width) / _max_servo_degree) + _min_pulse_width);
	}
	
	
	
	
//	hwlib::cout << pca.read(PRE_SCALE) << "\n";
//	pca.write_8(MODE1, 0x10);
//	pca.write_8(PRE_SCALE, 0x51);
//	pca.write_8(MODE1, 0x00);
//	hwlib::cout << pca.read(PRE_SCALE) << "\n";

//	void set_frequency(uint16_t freq){
//		if (freq < 24) {
//			freq = 24;
//		} 
//		else if (freq > 1526) {
//			freq = 1526;
//		}
//		float prescaleval = (25000000 / 4096) / freq - 1;
//		
//		transactie.write(MODE1, 0x10);
//		hwlib::wait_ms(2);
//		transactie.write(PRE_SCALE, prescaleval);
//		hwlib::wait_ms(2);
//		transactie.write(MODE1, 0x00);
//	}