// ==========================================================================
//
// File      : PCA9685.hpp
// Part of   : IPASS Rogier van Rooijen 
// Copyright : rogier.vanrooijen@student.hu.nl 2020/2021
//
// (See accompanying file README.md for more licensing information)
//
// ==========================================================================

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


/// @file

/// \brief
/// PCA6985 servo controller class
/// \details
/// This is an class for the PCA6985 chip from Adafruit.
/// It works with I2C to move servo's and can also be used to light up a different amount of leds.
/// The I2C works with given Bus and Address.
/// It has its own read function and 2 write functions for 16 bits values and 8 bit values.
/// The other functions are different ways to manipulate the servo motors. 
/// please refer to the Datasheet for more information about the registers and ways to connect the chip:
/// https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf
class PCA6985{
private:
    hwlib::i2c_bus & bus;
	uint_fast8_t address;

public:
   /// \brief
   /// constructor from the bus and the address
   /// \details
   /// This constructor initializes a I2C bus and I2C address.
   /// The I2C address has the standard chip address as default value.
   /// However the I2C bus has no default value!
    PCA6985(hwlib::i2c_bus & bus ,uint_fast8_t address=CHIP_ADDRESS): 
	bus(bus),
	address(address)
    {}
	
   /// \brief
   /// union to split up a full byte
   /// \details
   /// This union splits up a full 16 bits value up into two 8 bits bytes and stores it in a variable.
   /// The two Half_byte's can be combined into the Full_byte variable by using combine.
	union {
		uint16_t Full_byte;
		uint8_t Half_byte[2];
	} byte16_combine;
	
   /// \brief
   /// struct for two halfs of 16 bit value
   /// \details
   /// This struct makes it possible to store the lower and higher byte of the 16 bit value into seperate variables.
   /// The lower and higher byte can be written seperatly by using the write_halfbyte function.
	struct byte16_Halfs { 
		uint8_t L_byte;
		uint8_t H_byte;
		};
		
   /// \brief
   /// struct to convert a given pin number on the chip to the right I2C registers for the pin
   /// \details
   /// This struct takes a given uint8_t pin and converts is to the needed ON_L , ON_H , OFF_L and OFF_H registers of the pin.
   /// It does this by taking the LED0 registers and adding 4 mutiplied by the given pin to shift to needed LED.
   /// If no given pin, the default pin number will be 0. 
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

   /// \brief
   /// read function to read out data on the I2C bus
   /// \details
   /// This uint_fast16_t read function takes a given register address.
   /// For this address it writes the given address to the bus.
   /// And then reads the data on that address and returns that value.
   /// It implements the made I2C bus for the chip.
	uint_fast16_t read(uint8_t address){
		uint8_t data = 0;
		bus.write(CHIP_ADDRESS).write(address);
		bus.read(CHIP_ADDRESS).read(data);
		return data;
	}
	
   /// \brief
   /// write function to write given full_byte to two half_bytes registers
   /// \details
   /// This byte16_Halfs write_halfbyte function takes a given full_byte value.
   /// It uses the byte16_combine union to store the full_byte.
   /// Then it splits the full_byte value into a L(ow)_byte and H(igh)_byte variable.
   /// These variables are put in the byte16_Halfs struct so they are easily used in other funtions.
   /// The byte16_Halfs struct with the two halfs is returned.
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

   /// \brief
   /// write function to write one 16 bits input to two registers
   /// \details
   /// This void write_16 function takes given Low and High registers and the input values for those registers.
   /// For the Low register address it writes the given Low 8 bits input to the I2C bus.
   /// After that it waits 2 milliseconds.
   /// Then for the High register address it writes the given 8 bits High input to the I2C bus.
   /// Together these two 8 bits values can be seen as a 16 bits value.
   /// It implements the made I2C bus for the chip to make the needed transaction.
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
	
   /// \brief
   /// write function to write one 8 bits input to a register
   /// \details
   /// This void write_8 function takes given 8 bits register and the input value for that register.
   /// For the register address it writes the given 8 bits input to the I2C bus.
   /// It implements the made I2C bus for the chip to make the needed transaction.
	void write_8(uint_fast8_t REGISTER, uint_fast8_t input){
	auto transactie = bus.write(CHIP_ADDRESS);
	transactie.write(REGISTER);
	transactie.write(input);
	}
	
   /// \brief
   /// Function to turn Auto increment on or off
   /// \details
   /// This function takes a boolean value.
   /// If the value is true, Auto increment is turned on by writing 0x20 to MODE1.
   /// Else 0x00 is written to MODE1.
   /// It implements the write_8 function to write the needed values.
	void set_Autoincrement(bool state){
		if(state == true){
			hwlib::wait_ms(2);
			write_8(MODE1, 0x20);
			hwlib::wait_ms(2);
		}
		else{
			hwlib::wait_ms(2);
			write_8(MODE1, 0x00);
			hwlib::wait_ms(2);
		}
	}
	
   /// \brief
   /// sets the frequency for the chip
   /// \details
   /// This void set_frequency function takes a given uint16_t frequency value.
   /// It checks if this value is between a minimal of 24 and a maximum of 1526.
   /// If the given value is out of this range it sets it two the minimal of 24 or the maximum of 1526.
   /// Then the prescale value is calculates by taking 25000000 and dividing it by 4096 times the given frequency minus 1.
   /// The MODE1 register is put to sleep so the prescale can be set to the right value.
   /// It implements the write_8 function to write the needed values.
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
	
   /// \brief
   /// sets the given angle for the given servo pin
   /// \details
   /// This void setServo_Angle function takes a given uint8_t pin value and a uint16_t angle value from 0-180.
   /// It checks if the angle is in the range of 0 to 180.
   /// If not it corrects it to 0 if under and to 180 if above, It doens't work with negative values!
   /// The angle is multiplied by 2.28 and 102 is summed up to it so the right value will be written to the register.
   /// Then a byte16_Halfs value is made to write the angle to.
   //  Also a Pin_to_Register value is made to write to the right registers given the pin value.
   /// It implements the write_8 function to write the L(ow)_byte and H(igh)_byte values to the right pin.
	
	void setServo_Angle(uint8_t pin, uint16_t angle) {
		// Sets an Angle for the Servo, Doesn't work with with negative values! //
		if (angle < 0) {
			angle = 0;
		}
		if (angle > 180) {
			angle = 180;
		}
		angle *= 2.28;
		angle += 102;
		
		PCA6985::byte16_Halfs half_byte = write_halfbyte(angle);
		PCA6985::Pin_to_Register Pin(pin);
		
		write_8(Pin.Led_on_L, 0x00); // HIGH value
		write_8(Pin.Led_on_H, 0x00); // HIGH value
		write_8(Pin.Led_off_L, half_byte.L_byte); // HIGH value
		write_8(Pin.Led_off_H, half_byte.H_byte); // HIGH value
		
	}
	
   /// \brief
   /// sets the angle by button input for the given servo pin
   /// \details
   /// This void setServo_Angle_Buttons function takes a global servo_angle and two pin_in input switches.
   /// First the switches are refreshed so the input value is set to 0.
   /// Then the switches are read, if value of switch1 is equal to 1 it turns the servo_angle up by 10.
   /// If value of switch2 is equal to 1 it turns the servo_angle down by 10.
   /// Then it waits for 100 milliseconds so the input doesn't get interrupted. 
   /// The switches need to be connected with Pull-down resistors to read the right values!
   /// It implements the global servo_angle which is 80 given to the function.
	void setServo_Angle_Buttons(uint16_t & servo_angle, hwlib::target::pin_in & sw1 ,hwlib::target::pin_in & sw2) {
		hwlib::wait_ms(10);
		sw1.refresh();
		sw2.refresh();
		hwlib::wait_ms(10);
		
		if(sw1.read() == 1){
			if(servo_angle <= 180){
				servo_angle += 10;
			}
		}
		if(sw2.read() == 1){
			if(servo_angle >= 10){
				servo_angle -= 10;
			}
		}
	}
	
   /// \brief
   /// sets the servo to do a pulse in a given range
   /// \details
   /// This void setServo_Pulse function takes a given uint8_t pin value, and a uint16_t Min and Max range.
   /// It checks if the Min_range is above 0 and the Max range is under 180.
   /// If not it corrects the Min_range to 0 and the Max_range to 180 if above, It doens't work with negative values!
   /// The angle is multiplied by 2.28 and 102 is summed up to it so the right value will be written to the register.
   /// Then a byte16_Halfs value is made to write the angle to.
   /// Also a Pin_to_Register value is made to write to the right registers given the pin value.
   /// For this Min_range to Max_range a servo pulse is given so it sweeps one time.
   /// It implements the write_8 function to write the L(ow)_byte and H(igh)_byte values to the right pin.
	void setServo_Pulse(uint8_t pin, uint16_t Min_range , uint16_t Max_range) {
			// Set a Pulse on the servo with a range from 0-180 //
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
			PCA6985::byte16_Halfs half_byte = write_halfbyte(i);
			PCA6985::Pin_to_Register Pin(pin);
			
			write_8(Pin.Led_on_L, 0x00); // HIGH value
			write_8(Pin.Led_on_H, 0x00); // HIGH value
			write_8(Pin.Led_off_L, half_byte.L_byte); // HIGH value
			write_8(Pin.Led_off_H, half_byte.H_byte); // HIGH value
			
			hwlib::wait_ms(10);
		}
	
	}
};

#endif // PCA9685_HPP