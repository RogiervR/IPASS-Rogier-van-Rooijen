// ==========================================================================
//
// File      : main.cpp
// Part of   : IPASS Rogier van Rooijen 
// Copyright : rogier.vanrooijen@student.hu.nl 2020/2021
//
// (See accompanying file README.md for more licensing information)
//
// ==========================================================================

#include "hwlib.hpp"
#include "PCA9685.hpp"
#include "laser.hpp"
#include "Objectscanner.hpp"

int main() {
	hwlib::wait_ms(100);
	////////////////////////////////////////////////////////////////
	
	auto sda = hwlib::target::pin_oc (hwlib::target::pins::d20);
	auto scl = hwlib::target::pin_oc (hwlib::target::pins::d21);
	
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda (scl, sda);
	auto pca = PCA6985(i2c_bus);
	
	////////////////////////////////////////////////////////////////
	
	auto beeper_pin = hwlib::target::pin_out (hwlib::target::pins::d8);
	auto laser_pin = hwlib::target::pin_out (hwlib::target::pins::d7);
	auto laser = Laser_module(laser_pin);
	
	////////////////////////////////////////////////////////////////
	
	auto servo0_sw1 = hwlib::target::pin_in( hwlib::target::pins::d2);
	auto servo0_sw2 = hwlib::target::pin_in( hwlib::target::pins::d3);
	
	auto servo1_sw1  = hwlib::target::pin_in( hwlib::target::pins::d4);
	auto servo1_sw2  = hwlib::target::pin_in( hwlib::target::pins::d5);
	
	////////////////////////////////////////////////////////////////
	
	auto Trig_pin  = hwlib::target::pin_out( hwlib::target::pins::d10);
	auto Echo_pin  = hwlib::target::pin_in_out( hwlib::target::pins::d11);
	
	Echo_pin.direction_set_input();
	
	auto sr04 = SR04_Distancesensor(Trig_pin , Echo_pin , beeper_pin);
	
	////////////////////////////////////////////////////////////////
	
	auto oled_scl     = hwlib::target::pin_oc( hwlib::target::pins::d12 );
	auto oled_sda     = hwlib::target::pin_oc( hwlib::target::pins::d13 );
	auto oled_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( oled_scl, oled_sda );
	auto oled         = hwlib::glcd_oled( oled_i2c_bus, 0x3c ); 
	auto font         = hwlib::font_default_16x16();
	oled.clear();
	
	////////////////////////////////////////////////////////////////
	uint16_t servo_angle1 = 80;
	uint16_t servo_angle2 = 80;

	// Auto Increment aan: //
	pca.set_Autoincrement(true);
	
	// Prescale Frequentie aanpassen // 
	hwlib::wait_ms(100);
	hwlib::cout << pca.read(PRE_SCALE) << "\n";
	pca.set_frequency(50);
	hwlib::cout << pca.read(PRE_SCALE) << "\n";
	
	hwlib::cout << hwlib::endl;
	
	PCA6985::Pin_to_Register Pin(0);
	hwlib::cout << Pin.Led_on_L << hwlib::endl;
	hwlib::cout << Pin.Led_on_H << hwlib::endl;
	hwlib::cout << Pin.Led_off_L << hwlib::endl;
	hwlib::cout << Pin.Led_off_H << hwlib::endl;

for(;;){
//////////////////////////////////////////////////////////////////////
					// DEMO // 
//	laser.Laser_Beam(true);
//	laser.Laser_Flashing(100);
	::Oled_Interaction(Trig_pin , Echo_pin , oled , font, beeper_pin , laser_pin);
	
	pca.setServo_Angle_Buttons(servo_angle1, servo0_sw1, servo0_sw2);
	pca.setServo_Angle(0, servo_angle1);
	pca.setServo_Angle_Buttons(servo_angle2, servo1_sw1, servo1_sw2);
	pca.setServo_Angle(1, servo_angle2);
	hwlib::cout << servo_angle1 << hwlib::endl;
	hwlib::cout << servo_angle2 << hwlib::endl;
	
//////////////////////////////////////////////////////////////////////
				// Overige Functies // 

//	laser.Laser_Beam(true);
//	laser.Laser_Flashing(100);
//	laser.Laser_LongShort(2000 , 600);

//	pca.setServo_Pulse(0, 30 , 150);
//	pca.setServo_Pulse(0, 150 , 30);
//	pca.setServo_Pulse(1, 30 , 150);
//	pca.setServo_Pulse(1, 150 , 30);

//	sr04.Distancesensor_Alarm(laser_pin);
//	sr04.Beeper_Flashing(100);
//	sr04.Distancesensor_Average();

	
//	pca.setServo_Angle(0, 80);
//	hwlib::wait_ms(100);
//	pca.setServo_Angle(1, 120);
//	hwlib::wait_ms(100);


//	hwlib::wait_ms(100);
//	pca.setServo_Pulse(1, 30 , 150);
//	hwlib::wait_ms(100);

//////////////////////////////////////////////////////////////////////
	}
}