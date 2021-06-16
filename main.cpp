#include "hwlib.hpp"
#include "PCA9685.hpp"
#include "laser.hpp"

int main() {
    hwlib::wait_ms(2000);
	auto sda = hwlib::target::pin_oc (hwlib::target::pins::d20);
    auto scl = hwlib::target::pin_oc (hwlib::target::pins::d21);
    
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda (scl, sda);
    auto pca = pca6985(i2c_bus);
	
	auto laser = hwlib::target::pin_out (hwlib::target::pins::d8);
	
	auto servo0_sw1 = hwlib::target::pin_in( hwlib::target::pins::d2);
	auto servo0_sw2 = hwlib::target::pin_in( hwlib::target::pins::d3);
	
	auto servo1_sw1  = hwlib::target::pin_in( hwlib::target::pins::d4);
	auto servo1_sw2  = hwlib::target::pin_in( hwlib::target::pins::d5);
	////////////////////////////////////////////////////////////////

//	auto sw1 = & servo0_sw1;
//	auto sw2 = & servo0_sw2;

	uint16_t servo_angle1 = 80;
	uint16_t servo_angle2 = 80;
	// Auto Increment aan: //
	hwlib::wait_ms(2);
	pca.write_8(MODE1, 0x20);
	hwlib::wait_ms(2);
	
	// Prescale Frequentie aanpassen // 
	hwlib::wait_ms(1000);
	hwlib::cout << pca.read(PRE_SCALE) << "\n";
	pca.set_frequency(50);
	hwlib::cout << pca.read(PRE_SCALE) << "\n";
	
	hwlib::cout << hwlib::endl;
	
	
	pca6985::Pin_to_Register Pin(0);
	hwlib::cout << Pin.Led_on_L << hwlib::endl;
	hwlib::cout << Pin.Led_on_H << hwlib::endl;
	hwlib::cout << Pin.Led_off_L << hwlib::endl;
	hwlib::cout << Pin.Led_off_H << hwlib::endl;

for(;;){	
//	auto last = hwlib::now_us();
//	if(hwlib::now_us() > last+1000) {
//		::Laser_Knipper(laser , 100);
//		last = hwlib::now_us();
//	}

	::Laser_Straal(laser , true);
	
	pca.setServo_Angle_Buttons(servo_angle1, servo0_sw1, servo0_sw2);
	pca.setServo_Angle(0, servo_angle1);
	pca.setServo_Angle_Buttons(servo_angle2,servo1_sw1, servo1_sw2);
	pca.setServo_Angle(1, servo_angle2);
	hwlib::cout << servo_angle1 << hwlib::endl;
	hwlib::cout << servo_angle2 << hwlib::endl;



//	::Laser_Knipper(laser , 100);
//	::Laser_LangKort(laser , 2000 , 600);
	
	
//	pca.setServo_Angle(0, 0);
//	hwlib::wait_ms(100);
//	pca.setServo_Angle(1, 90);
//	hwlib::wait_ms(100);
//	pca.setServo_Pulse(0, 0 , 180);
//	hwlib::wait_ms(100);
//	pca.setServo_Pulse(1, 30 , 150);
//	hwlib::wait_ms(100);


}
}