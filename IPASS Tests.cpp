// ==========================================================================
//
// File      : IPASS Tests.hpp
// Part of   : IPASS Rogier van Rooijen 
// Copyright : rogier.vanrooijen@student.hu.nl 2020/2021
//
// (See accompanying file README.md for more licensing information)
//
// ==========================================================================
#include "hwlib.hpp"
#include <iostream>
#include <bitset>

#define LED0_ON_L 		0x06
#define LED0_ON_H 		0x07
#define LED0_OFF_L		0x08
#define LED0_OFF_H		0x09

union {
		uint16_t Full_byte;
		uint8_t Half_byte[2];
		} byte16_combine;
	
	struct byte16_Halfs { 
		uint8_t L_byte;
		uint8_t H_byte;
		};
		
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
	
	float set_frequency(uint16_t freq){
		if (freq < 24) {
			freq = 24;
		} 
		else if (freq > 1526) {
			freq = 1526;
		}
		float prescaleval = (25000000 / (4096 * freq)) - 1;
		
		return prescaleval;
	}
	
	uint16_t setServo_Angle(uint16_t angle) {
		// Sets an Angle for the Servo, Doesn't work with with negative values! //
		if (angle < 0) {
			angle = 0;
		}
		if (angle > 180) {
			angle = 180;
		}
		angle *= 2.28;
		angle += 102;
		
		return angle;
	}
	
	float Distancesensor(int start ,int end){

		
		int microsec = end - start;
		float sec = microsec / 1000000.0;
		float distance_cm = (sec * 34300) / 2;
		return distance_cm;
	}
int main(){
	std::cout << std::endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////
	int testwaarde_halfbyte = 1500;
	byte16_Halfs half_byte = write_halfbyte(testwaarde_halfbyte);
	std::cout << "** Hier is een Test voor een Full 16 bits waarde splitsen in twee 8 bits bytes **" << std::endl;
	std::cout << "De full byte waarde meegegeven is: " << std::bitset<16>(testwaarde_halfbyte) << " ookwel in decimaal: " << testwaarde_halfbyte << std::endl;
	std::cout << "16 Bits Higher part = " << std::bitset<8>(half_byte.H_byte)  << std::endl;
    std::cout << "16 Bits Lower part = " << std::bitset<8>(half_byte.L_byte)  << std::endl;
	std::cout << std::bitset<8>(half_byte.H_byte) << " " << std::bitset<8>(half_byte.L_byte) << " maakt samen dus samen de decimale waarde: " << testwaarde_halfbyte << std::endl;
	std::cout << std::endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////
	int testwaarde_PintoRegister = 5;
	Pin_to_Register Pin(testwaarde_PintoRegister);
	std::cout << "** Hier is een Test voor Pin nummer meegeven en shiften naar de juiste registers **" << std::endl;
	std::cout << "Dit zijn de waardes van de LED_ON_L , LED_ON_H , LED_OFF_L en LED_OFF_H van de registers voor LED0 (PIN 0) (voor de bewerking): " << std::endl;
	std::cout << std::bitset<8>(LED0_ON_L) << "    " << std::bitset<8>(LED0_ON_H) << "    " << std::bitset<8>(LED0_OFF_L) << "    " << std::bitset<8>(LED0_OFF_H) << std::endl;
	std::cout << "Als er een Pin wordt meegegeven aan Pin_to_Register zullen deze registers omgerekend worden naar de juiste waarde voor dit LED nummer" << std::endl;
	std::cout << "Hier de waarde van de pin: " << testwaarde_PintoRegister << " deze eerder genoemde registers zullen dan ook met: " << testwaarde_PintoRegister << " vermenigvuldigd worden." << std::endl;
	std::cout << "Als volgt zijn dit de waardes van de LED_ON_L , LED_ON_H , LED_OFF_L en LED_OFF_H registers na de bewerking: " << std::endl;
	std::cout << std::bitset<8>(Pin.Led_on_L) << "    " << std::bitset<8>(Pin.Led_on_H) << "    " << std::bitset<8>(Pin.Led_off_L) << "    " << std::bitset<8>(Pin.Led_off_H) << std::endl;
	std::cout << "Deze byte waardes corresponderen dan ook met LED" << testwaarde_PintoRegister << " in de Datasheet." << std::endl;
	std::cout << std::endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////
	uint16_t test_frequentie = 50;
	set_frequency(test_frequentie);
	std::cout << "** Hier is een Test voor de PWM frequentie instellen voor de PCA9685 **" << std::endl;
	std::cout << "De meegegeven frequentie is: " << test_frequentie << std::endl;
	std::cout << "Om de Prescale value uit te rekenen (gevraagde waarde voor PRE_SCALE Register) gebruiken we deze formule: " << std::endl;
	std::cout << "Prescale value = (25000000 / (4096 * frequentie)) - 1" << std::endl;
	std::cout << "Als we de meegegeven frequentie: " << test_frequentie << "in de formule verwerken krijgen we:" << std::endl;
	std::cout << "Prescale value = (25000000 / (4096 * " << test_frequentie << " )) - 1 = " << set_frequency(test_frequentie) << std::endl;
	std::cout << std::endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////
	uint16_t test_angle = 90;
	std::cout << "** Hier is een Test voor de Servo angle instellen voor een bepaalde pin **" << std::endl;
	std::cout << "De meegegeven angle is: " << test_angle << std::endl;
	std::cout << "Om de juiste angle waarde te krijgen voor de registers wordt deze waarde met 2.28 vermenigvuldigd" << std::endl;
	std::cout << "Ook wordt de waarde +102 gedaan om de juiste startwaarde te krijgen" << std::endl;
	std::cout << "De Servo Angle die wordt meegegeven aan de functie staat dan gelijk aan: " << setServo_Angle(test_angle) << std::endl;
	std::cout << std::endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////
	int start = 288506;
	int end = 288941;
	std::cout << "** Hier is een Test voor Afstand berekenen doormiddel van ultrasone pulsen **" << std::endl;
	std::cout << "Eerst pakt de functie de start tijd van wanneer de echo pin laag is bijv: " << start << std::endl;
	std::cout << "Vervolgens pakt de functie de end tijd van wanneer de echo pin hoog wordt bijv: " << end << std::endl;
	std::cout << "De tijd in microseconde tussen deze twee pulsen is dan: " << (end-start) << std::endl;
	std::cout << "De tijd in seconde is dan gelijk aan: " << float((end-start) / 1000000.0) << std::endl;
	std::cout << "De afstand in cm tussen de afstandssensor en het object wordt dan uitgerekend met deze formule: " << std::endl;
	std::cout << "distance_cm = (sec * 34300) / 2 " << ", Als we hier onze berekende tijd in verwerken krijgen we deze afstand in cm" << std::endl;
	std::cout << "distance_cm = (sec * 34300) / 2 = " << Distancesensor(start , end) << std::endl;
	std::cout << std::endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////
	}