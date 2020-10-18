#include "hwlib.hpp"

namespace hwlib {

class st7789_matthijs {
public:
   //Commands
   enum class commands : uint8_t {
      NOP        = 0x00,
      SWRESET    = 0x01,
      RDDID      = 0x04,
      RDDST      = 0x09,
      RDDPM      = 0x0A,
      RDDMADCTL  = 0x0B,
      RDDIM      = 0x0D,
      RDDSEM     = 0x0E,
      RDDSDR     = 0x0F,

      SLPIN      = 0x10,
      SLPOUT     = 0x11,
      PTLON      = 0x12,
      NORON      = 0x13,

      INVOFF     = 0x20,
      INVON      = 0x21,
      GAMSET     = 0x26,
      DISPOFF    = 0x28,
      DISPON     = 0x29,
      CASET      = 0x2A,
      RASET      = 0x2B,
      RAMWR      = 0x2C,
      RAMRD      = 0x2E,

      PTLAR      = 0x30,
      VSCRDEF    = 0x33,
      TEOFF      = 0x34,
      TEON       = 0x35,
      MADCTL     = 0x36,
      COLMOD     = 0x3A
   };   
};

template<typename T, unsigned int N>
class my_array{
public:
	T operator[](int i) const{
		return a[i];
	}
	
private:
	T a[N];

};

template< unsigned int N>
class my_array<bool, N>{
public:
	bool operator[](int i)const{
		return (a[i/ 8] >> (i%8)) & 0x01;	
	}
	bool get(int i)const{
		return((a[i / 8] >> (i%8)) & 0x01) != 0;
	}
	
	void set(int i, bool b){
		if(b){
			a[i / 8] |= (0x01 << (i%8));	
		}
		else{
			a[i / 8] &= ~(0x01 << (i%8));	
		}
	}
	
	class reference{
	private:
		my_array & subject;
		int n;
	public:
		reference(my_array & subject, int n): subject(subject), n(n){}
		
		reference & operator=(bool b){
			subject.set(n, b);
			return *this;
		}
		
		operator bool(){
			return subject.get(n);	
		}
	
	};

	reference operator[](int i){
		return reference(*this, i);
	}
private:
	unsigned char a[(N+ 7) / 8];
	
};

class st7789_noncolor : public st7789_matthijs, public window {
private:

   static auto constexpr wsize = xy( 240, 240 );
   
   my_array<bool, wsize.x * wsize.y> buffer; 
   
   void write_implementation( 
      xy pos, 
      color col
   ) override {
		if(col == white){
		   buffer[ pos.x + wsize.x * pos.y ] = 
           1;
		}
		else{
			buffer[ pos.x + wsize.x * pos.y ] = 
           0;
		}
   }      

   spi_bus & bus;
   pin_out & dc;
   pin_out & cs;
   pin_out & rst;
   
public:

   void command( 
      commands c 
   ){
      dc.write( 0 ); dc.flush();
      auto transaction = bus.transaction( cs );
      transaction.write( static_cast< uint8_t >( c ) );      
      dc.write( 1 ); dc.flush();
   }

   void command( 
      commands  c,
      uint8_t   d0      
   ){
      dc.write( 0 ); dc.flush();
      auto transaction = bus.transaction( cs );
      transaction.write( static_cast< uint8_t >( c ) );
      dc.write( 1 ); dc.flush();
      transaction.write( d0 );           
   } 
   
   void command( 
      commands c, 
      uint8_t  d0,
      uint8_t  d1,
      uint8_t  d2,
      uint8_t  d3
   ){
      dc.write( 0 ); dc.flush();
      auto transaction = bus.transaction( cs );
      transaction.write( static_cast< uint8_t >( c ) );      
      dc.write( 1 ); dc.flush();
      transaction.write( d0 );    
      transaction.write( d1 );    
      transaction.write( d2 );    
      transaction.write( d3 );    
   } 	
   
   st7789_noncolor( 
      spi_bus & bus, 
      pin_out & dc, 
      pin_out & cs, 
      pin_out & rst
   ):
      window( wsize, white, black ),   
      bus( bus ), 
      dc( dc ), 
      cs( cs ), 
      rst( rst )
   {       
	  // Init
      rst.write( 0 );rst.flush();
      wait_ms( 200 );
      rst.write( 1 );rst.flush();
      wait_ms( 200 );
      
      command( commands::SWRESET );
      wait_ms( 150 );
      
      command( commands::SLPOUT );		 
      wait_ms( 10 );
      
      command( commands::COLMOD, 0x66 );
      wait_ms( 10 );
      
      command( commands::MADCTL, 0x10 );                      
      command( commands::CASET, 0, 0, wsize.x >> 8, wsize.x & 0xFF );
      command( commands::RASET, 0, 0, wsize.y >> 8, wsize.y & 0xFF );
      
      command( commands::INVON );
      wait_ms( 10 );
      command( commands::NORON );
      wait_ms( 100 );
      command( commands::DISPON );
      wait_ms( 100 );
   }     

   void flush() override {
      dc.write( 0 ); dc.flush();
      auto transaction = bus.transaction( cs );
      transaction.write( static_cast< uint8_t >( commands::RAMWR ) );     
      dc.write( 1 ); dc.flush();
      for( int i = 0; i < 240 * 240; ++i ){
		if( buffer[ i ] == true ){
			transaction.write(0xFF);
			transaction.write(0xFF);
			transaction.write(0xFF);
		}
		else{
			transaction.write(0x00);
			transaction.write(0x00);
			transaction.write(0x00);
		}
		 
      }   
   }     
        
};

}; // namespace hwlib