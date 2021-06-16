#ifndef LASER_HPP
#define LASER_HPP

#include "hwlib.hpp"

void Laser_Straal(hwlib::target::pin_out & PIN , bool State){
	if(State == false){
		PIN.write( 0 );
		PIN.flush();
	}
	else if(State == true){
		PIN.write( 1 );
		PIN.flush();
	}
}

void Laser_Knipper(hwlib::target::pin_out & PIN, int delay_ms){
	PIN.write( 1 );
	PIN.flush();
	hwlib::wait_ms( delay_ms );
	PIN.write(0);
	PIN.flush();
	hwlib::wait_ms( delay_ms );
}

void Laser_LangKort(hwlib::target::pin_out & PIN, int delay_long_ms, int delay_short_ms){
	PIN.write( 1 );
	PIN.flush();
	hwlib::wait_ms( delay_long_ms );
	PIN.write(0);
	PIN.flush();
	hwlib::wait_ms( delay_short_ms );
}
#endif // LASER_HPP