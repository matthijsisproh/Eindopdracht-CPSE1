// Code voor Minder geheugen gebruiken
#include "hwlib.hpp"
#include "spi.hpp"
#include "spi-direct.hpp"
#include "st7789-noncolor.hpp"
namespace target = hwlib::target;

int main( void ){
   
   // wait for the terminal emulator to start up
   hwlib::wait_ms( 2'000 );
   hwlib::cout << "ST7789 demo\n" << hwlib::flush;   
   
   auto _sclk = hwlib::target::pin_out{ hwlib::target::pins::d3 };
   auto sclk = hwlib::invert( _sclk );
   auto mosi = hwlib::target::pin_out{ hwlib::target::pins::d4 };
   
   auto spi  = hwlib::spi_bus_direct{ 
      28, 26 };
      
   auto dc    = hwlib::target::pin_out{ hwlib::target::pins::d6 };
   auto & cs  = hwlib::pin_out_dummy;
   auto blk   = hwlib::target::pin_out{ hwlib::target::pins::d7 };
   auto rst   = hwlib::target::pin_out{ hwlib::target::pins::d5 };
   
   blk.write( 1 );blk.flush();

   auto display     = hwlib::st7789_noncolor( spi, dc, cs, rst );   
   
   for(;;){
	  hwlib::cout << "1: " << hwlib::now_us() / 1'000  << "\n";
      display.clear( hwlib::white );
	  hwlib::cout << "2: " << hwlib::now_us() / 1'000 << "\n";
      display.flush();
	  hwlib::cout << "3: " << hwlib::now_us() / 1'000 << "\n";

      display.clear( hwlib::green );
	  display.flush();

      display.clear( hwlib::blue );
	  display.flush();
     
   }
}
