// Code De bestaande driver
#include "hwlib.hpp"

namespace target = hwlib::target;

int main( void ){
   
   // wait for the terminal emulator to start up
   hwlib::wait_ms( 2'000 );
   hwlib::cout << "ST7789 demo\n" << hwlib::flush;   
   
   auto _sclk = hwlib::target::pin_out{ hwlib::target::pins::d3 };
   auto sclk = hwlib::invert( _sclk );
   auto mosi = hwlib::target::pin_out{ hwlib::target::pins::d4 };
   
   auto spi  = hwlib::spi_bus_bit_banged_sclk_mosi_miso{ 
      sclk, mosi, hwlib::pin_in_dummy };
      
   auto dc    = hwlib::target::pin_out{ hwlib::target::pins::d6 };
   auto & cs  = hwlib::pin_out_dummy;
   auto blk   = hwlib::target::pin_out{ hwlib::target::pins::d7 };
   auto rst   = hwlib::target::pin_out{ hwlib::target::pins::d5 };
   
   blk.write( 1 );blk.flush();

   auto display     = hwlib::st7789_spi_dc_cs_rst( spi, dc, cs, rst );   
   
   for(;;){
	  hwlib::cout << "1: " << hwlib::now_us() / 1'000  << "\n";
      display.clear( hwlib::red );
	  hwlib::cout << "2: " << hwlib::now_us() / 1'000 << "\n";
      display.flush();
	  hwlib::cout << "3: " << hwlib::now_us() / 1'000 << "\n";

      display.clear( hwlib::green );
	  display.flush();

      display.clear( hwlib::blue );
	  display.flush();
     
   }
}
