
 //--------------------------------------------------------------------------
//							indM_get_Data_Array
//--------------------------------------------------------------------------
//#include "src-gen/indM.h"
//#include "src-gen/indM_required.h"
//#include "src/sc_timer_service.h"
#include "inc/bsp.h"
//#include "main.c"

    //                      VARIABLES
    //-------------------------------------------------------------------
        IndM        extern  ls                              ;
        uint32_t    extern  samp_L[MAX_SAMPLES] ;
        uint32_t    extern  samp_R[MAX_SAMPLES] ;
        int         extern  NRO_SAMPLES         ;
        int                 counter             ; //LOCAL
        //uint32_t  extern  aux                 ;



 void indM_get_Data_Array(const IndM* handle)
 {
  /*
	//						VARIABLES
	//-------------------------------------------------------------------

		uint32_t 	extern 	samp_L[MAX_SAMPLES]	;
		uint32_t 	extern 	samp_R[MAX_SAMPLES]	;
		int 		extern 	NRO_SAMPLES			;
		int 				counter				; //LOCAL
		//uint32_t 	extern 	aux					;
*/
	
	//						ROUTINE
	//-------------------------------------------------------------------
	
	//Counter start 
    counter = NRO_SAMPLES;
    //Turn on the FET.
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_6);

    //5.2 Sampling loop
    uint32_t nx_sample = SysTickValueGet();

    while ( counter != 0){
        if ( SysTickValueGet() <= nx_sample ) {
                            //Get new data
            // Dummy data for tx FIFO, to trigger the CS and CLK.
            SSIDataPut(SSI0_BASE, 0);
            SSIDataGetNonBlocking(SSI0_BASE, &samp_L[NRO_SAMPLES - counter]);
            SSIDataGetNonBlocking(SSI1_BASE, &samp_R[NRO_SAMPLES - counter]);

                            //Update parameters

            nx_sample -= 160u;
            --counter;
        }//end if
    } //end while

    //5.3 Turn off the FET
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_7);
 
 }	//end indM_get_Data_Array
 
