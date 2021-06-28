// ==========================================================================
//
// File      : laser.hpp
// Part of   : IPASS Rogier van Rooijen 
// Copyright : rogier.vanrooijen@student.hu.nl 2020/2021
//
// (See accompanying file README.md for more licensing information)
//
// ==========================================================================

#ifndef LASER_HPP
#define LASER_HPP

#include "hwlib.hpp"

/// @file

/// \brief
/// Laser module class
/// \details
/// This is an class for the Laser Module.
/// It has several different functions to make the Laser module light up.
class Laser_module{
protected:
    hwlib::target::pin_out & laser_pin;
public:
   /// \brief
   /// constructor from the given laser_pin
   /// \details
   /// This constructor initializes the given laser_pin
   /// It doens't work without a given laser_pin to reference to.
    Laser_module(hwlib::target::pin_out & laser_pin): 
	laser_pin(laser_pin)
    {}
	
   /// \brief
   /// Function to turn the laser on or off
   /// \details
   /// This function makes the laser go on or off given the parameter.
   /// If the parameter is equal to true the laser_pin will be set to high and the laser will turn on.
   /// When the laser_pin is equal to false the laser_pin will be set to low and the laser will turn off.
	void Laser_Beam(bool State){
		if(State == false){
			laser_pin.write( 0 );
			laser_pin.flush();
		}
		else if(State == true){
			laser_pin.write( 1 );
			laser_pin.flush();
		}
	}
	
   /// \brief
   /// Function to make the laser flash with a given delay
   /// \details
   /// This function makes the laser turn on and turn it off after a certain delay given the parameter.
   /// The parameter decides the delay in milliseconds between turning the laser on or off.
   /// When the laser_pin is set to high the laser will be on and when the laser_pin is set to low the laser will be off.
	void Laser_Flashing(int delay_ms){
		laser_pin.write( 1 );
		laser_pin.flush();
		hwlib::wait_ms( delay_ms );
		laser_pin.write(0);
		laser_pin.flush();
		hwlib::wait_ms( delay_ms );
	}
   /// \brief
   /// Function to make the laser flash with two seperate delays
   /// \details
   /// This function makes the laser turn on after a certain delay given the first parameter and turn it off after a certain delay given the second parameter.
   /// The two parameters decide the delay in milliseconds between turning the laser on and the delay between turning the laser off.
   /// When the laser_pin is set to high the laser will be on and when the laser_pin is set to low the laser will be off.
	void Laser_LongShort(int delay_long_ms, int delay_short_ms){
		laser_pin.write( 1 );
		laser_pin.flush();
		hwlib::wait_ms( delay_long_ms );
		laser_pin.write(0);
		laser_pin.flush();
		hwlib::wait_ms( delay_short_ms );
	}
};
#endif // LASER_HPP