#ifndef PCA9685_HPP
#define PCA9685_HPP


#include "hwlib.hpp"


#define CHIP_ADDRESS	0x40

#define MODE1			0x00
#define MODE2			0x01

#define PRE_SCALE		0xFE

#define LED0_ON_L 		0x06
#define LED0_ON_H 		0x07
#define LED0_OFF_L		0x08
#define LED0_OFF_H		0x09

#define LED1_ON_L		0x0A
#define LED1_ON_H 		0x0B
#define LED1_OFF_L		0x0C
#define LED1_OFF_H		0x0D


class pca6985{
private:
    hwlib::i2c_bus & bus;
	uint_fast8_t address;

public:
    pca6985(hwlib::i2c_bus & bus ,uint_fast8_t address=CHIP_ADDRESS): 
	bus(bus),
	address(address)
    {}
	
	union {
		uint16_t Full_byte;
		uint8_t Half_byte[2];
	} byte16_combine;
	
	struct byte16_Halfs { 
		uint8_t L_byte;
		uint8_t H_byte;
		};
		
	struct Pin_to_Register { 
		uint8_t pin;
		uint8_t Led_on_L  = LED0_ON_L + (4 * pin);
		uint8_t Led_on_H  = LED0_ON_H + (4 * pin);
		uint8_t Led_off_L = LED0_OFF_L + (4 * pin);
		uint8_t Led_off_H = LED0_OFF_H + (4 * pin);
		
		Pin_to_Register(uint8_t pin = 0): 
		pin(pin)
		{}
		};

	uint_fast16_t read(uint8_t address){
		uint8_t data = 0;
		bus.write(CHIP_ADDRESS).write(address);
		bus.read(CHIP_ADDRESS).read(data);
		return data;
	}
	
	byte16_Halfs write_halfbyte(uint16_t full_byte){
		byte16_combine.Full_byte = full_byte;
		uint8_t L_byte = byte16_combine.Half_byte[0];
		uint8_t H_byte = byte16_combine.Half_byte[1];
		byte16_Halfs byte1 { 
			L_byte, 
			H_byte 
			};
		return byte1;
		
	}
	
	void write_16(uint_fast8_t REGISTER_L, uint_fast8_t REGISTER_H, uint_fast8_t input_L, uint_fast8_t input_H){
		{
		auto transactie = bus.write(CHIP_ADDRESS);
		transactie.write(REGISTER_L);
		transactie.write(input_L);
		}
		hwlib::wait_ms(2);
		{
		auto transactie2 = bus.write(CHIP_ADDRESS);
		transactie2.write(REGISTER_H);
		transactie2.write(input_H);
		}
	}
	
	void write_8(uint_fast8_t REGISTER, uint_fast8_t input){
	auto transactie = bus.write(CHIP_ADDRESS);
	transactie.write(REGISTER);
	transactie.write(input);
	}
	
	void set_frequency(uint16_t freq){
		if (freq < 24) {
			freq = 24;
		} 
		else if (freq > 1526) {
			freq = 1526;
		}
		float prescaleval = (25000000 / (4096 * freq)) - 1;
		
		write_8(MODE1, 0x10);
		hwlib::wait_ms(2);
		write_8(PRE_SCALE, prescaleval);
		hwlib::wait_ms(2);
		write_8(MODE1, 0x00);
	}
	
	/// Sets an Angle for the Servo, Doesn't work with with negative values! ///
	void setServo_Angle(uint8_t pin, uint16_t angle) {
		if (angle < 0) {
			angle = 0;
			hwlib::cout << angle;
		}
		if (angle > 180) {
			angle = 180;
		}
		angle *= 2.28;
		angle += 102;
		
		pca6985::byte16_Halfs half_byte = write_halfbyte(angle);
		pca6985::Pin_to_Register Pin(pin);
		
		write_8(Pin.Led_on_L, 0x00); // HIGH value
		write_8(Pin.Led_on_H, 0x00); // HIGH value
		write_8(Pin.Led_off_L, half_byte.L_byte); // HIGH value
		write_8(Pin.Led_off_H, half_byte.H_byte); // HIGH value
		
	}
	
	/// Set a Pulse on the servo with a range from 0-180
	void setServo_Pulse(uint8_t pin, uint16_t Min_range , uint16_t Max_range) {
		if (Min_range < 0) {
			Min_range = 0;
		}
		if (Max_range > 180) {
			Max_range = 180;
		}
		Min_range *= 2.28;
		Min_range += 102;
		Max_range *= 2.28;
		Max_range += 102;
		
		hwlib::cout << Min_range << hwlib::endl;
		hwlib::cout << Max_range << hwlib::endl;
		
		for(int i=Min_range ; i < Max_range; i+=2){
			pca6985::byte16_Halfs half_byte = write_halfbyte(i);
			pca6985::Pin_to_Register Pin(pin);
			
			write_8(Pin.Led_on_L, 0x00); // HIGH value
			write_8(Pin.Led_on_H, 0x00); // HIGH value
			write_8(Pin.Led_off_L, half_byte.L_byte); // HIGH value
			write_8(Pin.Led_off_H, half_byte.H_byte); // HIGH value
			
			hwlib::wait_ms(10);
		}
	
	}
};

#endif // PCA9685_HPP