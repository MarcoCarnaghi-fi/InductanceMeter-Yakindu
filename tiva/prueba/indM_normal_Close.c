//--------------------------------------------------------------------------
//							indM_normal_Close
//--------------------------------------------------------------------------

//#include "src-gen/indM.h"
//#include "src-gen/indM_required.h"
//#include "src/sc_timer_service.h"
#include "inc/bsp.h"


    //                      CONSTANTES
    //-------------------------------------------------------------------
        float       extern    K_RT        ;
        uint32_t    extern    I_wished    ;

    //                      VARIABLES
    //-------------------------------------------------------------------
        IndM        extern      ls                  ;
        int         extern      NRO_SAMPLES         ;
        uint32_t    extern      n_i                 ;
        uint32_t    extern      n_f                 ;

 
 bool indM_normal_Close(const IndM* handle, const sc_integer ni, const sc_integer nf)
 {
	//						ROUTINE
	//-------------------------------------------------------------------
	
	//Turn off the FET
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_7);
	
	//Number of samples
		NRO_SAMPLES = (int) floor	(  (n_i - n_f)/160 	);
		NRO_SAMPLES = (int) floor	(	1.2*NRO_SAMPLES	);
		indM_set_n_samples(&ls, NRO_SAMPLES);
	// Configuration is over --> raise condition	
	//indM_raise_conf_Ready(&ls);
		return (bool) true;
	
	
 } // end indM_normal_Close
