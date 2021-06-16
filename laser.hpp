#ifndef LASER_HPP
#define LASER_HPP

#include "hwlib.hpp"

void Laser_straal(hwlib::target::pin_out & PIN){
	for(;;){
		PIN.write( 1 );
		PIN.flush();
		hwlib::wait_ms( 1000 );
		PIN.write(0);
		PIN.flush();
		hwlib::wait_ms( 1000 );
	}
}

#endif // LASER_HPP