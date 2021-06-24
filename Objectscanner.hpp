#ifndef OBJECTSCANNER_HPP
#define OBJECTSCANNER_HPP

#include "hwlib.hpp"
#include "laser.hpp"

/// @file

/// \brief
/// SR04 Distance sensor class
/// \details
/// This is an class for the SR04 ultrasone distance sensor
/// The sensor detects object by sending and recieving ultrasonic sound pulses.
/// It has several different functions to calculate distance and to use that distance for detecting objects.
class SR04_Distancesensor{
protected:
    hwlib::target::pin_out & Trig_pin;
	hwlib::target::pin_in_out & Echo_pin;
	hwlib::target::pin_out & beeper_pin;
public:
   /// \brief
   /// constructor made from the needed Trig and Echo pins for the distance sensor and a pin for a optional sound beeper.
   /// \details
   /// This constructor initializes the given Trig , Echo and beeper pins.
   /// It doens't work without a given Trig , Echo and beeper to reference to.
    SR04_Distancesensor(hwlib::target::pin_out & Trig_pin, hwlib::target::pin_in_out & Echo_pin , hwlib::target::pin_out & beeper_pin): 
	Trig_pin(Trig_pin),
	Echo_pin(Echo_pin),
	beeper_pin(beeper_pin)
    {}
	
   /// \brief
   /// Function to calculate distance using ultrasone pulses.
   /// \details
   /// This function makes a ultrasone pulse on the Trig pin. 
   /// With that pulse it can calculate the time it takes getting back to the Echo pin.
   /// That time can be used to calculate the measured distance in cm.
   /// This function caculates the distance once per invocation.
	float Distancesensor(){
		Trig_pin.write(1);
		Trig_pin.flush();
		hwlib::wait_ms(1);
		Trig_pin.write(0);
		Trig_pin.flush(); 
		
		while(Echo_pin.read() == 0){
		}
		auto start = hwlib::now_us();
		while(Echo_pin.read() == 1){
		}
		auto end = hwlib::now_us();
		
		int microsec = end - start;
		float sec = microsec / 1000000.0;
		float distance_cm = (sec * 34300) / 2;
		return distance_cm;
	}
	
   /// \brief
   /// Function to calculate average distance of mutiple measurements.
   /// \details
   /// This function calls the Distancesensor() function to calculate one instance of distance.
   /// It sums the instance of distance to a total distance.
   /// The distance is caculated 10 times and then a average distance of those calculations is returned.
	float Distancesensor_Average(){
		float total = 0.0;
		float average = 0.0;
		int count = 10;
		for (int i = 0; i < count; i++){
			total += Distancesensor();
			hwlib::wait_ms(100);
		}
		average = total / count;
		hwlib::cout << int(average) << hwlib::endl;
		return average;
	}
	
   /// \brief
   /// Function for a motion alarm with a beeper and a laser module.
   /// \details
   /// This function calls the Distancesensor_average() function to calculate the average distance.
   /// If the object in front of the sensor is closer than 7 cm,
   /// the Beeper_Flashing() and Laser_Flashing() function will be called with a delay of 100 milliseconds between flash.
	void Distancesensor_Alarm(hwlib::target::pin_out & Laser_pin){
		auto laser = Laser_module(Laser_pin);
		float distance = Distancesensor_Average();
		if(distance < 7){
			Beeper_Flashing(100);
			laser.Laser_Flashing(100);
		}
	}
	
   /// \brief
   /// Function to make the sound beeper flash with a given delay
   /// \details
   /// This function makes the sound beeper turn on and turn it off after a certain delay given the parameter.
   /// The parameter decides the delay in milliseconds between turning the beeper on or off.
   /// When the beeper_pin is set to high the beeper will be on and when the beeper_pin is set to low the beeper will be off.
	void Beeper_Flashing(int delay_ms){
		beeper_pin.write( 1 );
		beeper_pin.flush();
		hwlib::wait_ms( delay_ms );
		beeper_pin.write(0);
		beeper_pin.flush();
		hwlib::wait_ms( delay_ms );
	}
};


/// \brief
/// Function to make an oled detection screen for the distancesensor
/// \details
/// This function makes a notification apear on the oled screen when a object is in range of the distancesensor. 
/// The function calls the SR04_Distancesensor() to calculate the average distance to the object.
/// if the object is closer than 15 cm the oled screen will say "OBJECT DETECTED".
/// Else if the object is further away then 15 cm the oled screen will say "NO OBJECT DETECTED".
/// On top of the oled printing out if a object is detected it also calls the Beeper_Flashing() and Laser_Flashing() function with a delay of 100 milliseconds.
void Oled_Interaction(hwlib::target::pin_out & Trig_pin, hwlib::target::pin_in_out & Echo_pin , hwlib::glcd_oled & oled , hwlib::font_default_16x16 & font , hwlib::target::pin_out & beeper_pin , hwlib::target::pin_out & laser_pin){
	auto laser = Laser_module(laser_pin);
	auto Distancesensor = SR04_Distancesensor(Trig_pin , Echo_pin , beeper_pin);
	float distance = Distancesensor.Distancesensor_Average();
	auto display = hwlib::terminal_from( oled, font );
	if(distance < 15){
		  display 
			 << "\f" << " OBJECT "
			 << "\n" << "DETECTED!"
			 << hwlib::flush;   
		Distancesensor.Beeper_Flashing(100);
		laser.Laser_Flashing(100);
	}
	else{
		  display 
			 << "\f" << "   NO  " 
			 << "\n" << " OBJECT "
			 << "\n" << "DETECTED!"
			 << hwlib::flush;  
	}
}





#endif // OBJECTSCANNER_HPP