#include "hwlib.hpp"

namespace hwlib {
    
class servo_bb : public background {
private:
   pin_out & pin;
   uint_fast64_t pulse;
   uint_fast64_t next_run;
   
public:
   servo_bb( pin_out & pin ):
       pin( pin ),
       pulse( 1'500 ),
       next_run( now_us() )
   {
      pin.write( 0 );
      pin.flush();
   }        
    
   void write( int n ){
      pulse = 1'000 + ( 1'000 * n ) / 100;
   }
       
   void work() override {
      if( hwlib::now_us() < next_run ){
         return;
      }       
      next_run += 20'000;       
      
      pin.write( 1 );
      pin.flush();
      
      // don't switch context, this delay must be accurate
      wait_us_busy( pulse );
      
      pin.write( 0 );
      pin.flush();
   }      
};

} // hwlib

int main( void ){
     
   namespace target = hwlib::target;
   
   auto p1 = hwlib::target::pin_out{ hwlib::target::pins::d2 };
   auto p2 = hwlib::target::pin_out{ hwlib::target::pins::d3 };
   auto s1 = hwlib::servo_bb( p1 );
   auto s2 = hwlib::servo_bb( p2 );
   
   for(;;){
//	   s1.write(160);
//	   s2.write(160);
//	   hwlib::wait_ms(1000);
      for( int n = 0; n < 100 ; ++n ){
         s1.write( n );
         s2.write( n );
         hwlib::wait_ms( 10 );
      }        
      for( int n = 100; n > 0 ; --n ){
         s1.write( n );
         s2.write( n );
         hwlib::wait_ms( 10 );
      }        
   }
}