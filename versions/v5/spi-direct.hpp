#pragma once
#include "hwlib.hpp"
namespace hwlib {

// bit-banged SPI bus implementation
//
// This class implements a bit-banged master interface to a SPI bus.

class spi_bus_direct : public spi_bus {
private:
	uint8_t sclk_pin;
	uint8_t mosi_pin;
	static constexpr uint8_t pin_number_sclk = 28;
	static constexpr uint8_t pin_number_mosi = 26;
	
	//Aangepaste functie
	void write_and_read( const size_t n, const uint8_t data_out[], uint8_t data_in[] ) override {
		uint_fast8_t d = *data_out++; 
		 
         for( uint_fast8_t j = 0; j < 8; ++j ){
			if((d & 0x80) != 0){
				PIOC->PIO_SODR = 0x01 << pin_number_mosi; // Set pin HIGH
			}
			else{
				PIOC->PIO_CODR = 0x01 << pin_number_mosi; // Set pin LOW
			}
			
			PIOC->PIO_CODR = 0x01 << pin_number_sclk; // Set pin LOW(because inverted HIGH)
			d = d << 1;
			PIOC->PIO_SODR = 0x01 << pin_number_sclk; // Set pin HIGH(because inverted LOW)
         }
	   
   }      

public:
   spi_bus_direct( 
      uint8_t sclk_pin, 
      uint8_t mosi_pin 
   ):
      sclk_pin( sclk_pin ), 
      mosi_pin( mosi_pin )
	  {
		PMC->PMC_PCER0 = 1 << ID_PIOC;// enable all clock to Port C
		PIOC->PIO_SODR = 0x01 << pin_number_sclk; // Set pin LOW(because inverted HIGH)
   }
   
   //Note: SCLK and MOSI pins must be on port C!
   
}; // class spi_bus_direct  

}; // namespace hwlib