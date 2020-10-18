#pragma once
#include "hwlib.hpp"
namespace hwlib {

class spi_bus_matthijs : public spi_bus {
private:

   pin_direct_from_out_t sclk;
   pin_direct_from_out_t mosi;
   pin_direct_from_in_t  miso;
   
   // very crude;
   // delay should be a constructor parameter
   void HWLIB_INLINE wait_half_period(){
      wait_us( 1 );      
   }
   
   //Aangepaste functie
   void write_and_read( 
      const size_t n, 
      const uint8_t data_out[], 
      uint8_t data_in[] 
   ) override {
         uint_fast8_t d = *data_out++;
             
         for( uint_fast8_t j = 0; j < 8; ++j ){
            mosi.write( ( d & 0x80 ) != 0 );
            sclk.write( 1 );
            d = d << 1;
            sclk.write( 0 );              
         }
	   
   }      

public:
   spi_bus_matthijs( 
      pin_out & _sclk, 
      pin_out & _mosi, 
      pin_in  & _miso 
   ):
      //sclk( direct( _sclk ) ), 
      //mosi( direct( _mosi ) ), 
      //miso( direct( _miso ) )
      
      sclk( _sclk ), 
      mosi( _mosi ), 
      miso( _miso )   {
      sclk.write( 0 );
   }
   
};
 
}; // namespace hwlib