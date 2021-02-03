//--------------------------------------------------------------------------
//							indM_safe_Close
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
        IndM    extern ls               ;
        int     extern Gain_L, Gain_R   ; // GLOBAL
        int     extern NRO_SAMPLES      ; // GLOBAL


 bool indM_safe_Close(const IndM* handle)
 {
	
	//						ROUTINE
	//-------------------------------------------------------------------

	//Turn off the FET
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_7);
	
	//Number of samples
	NRO_SAMPLES = 1	;
	indM_set_n_samples(&ls, NRO_SAMPLES);
	//Safe gain configuration.
	Gain_L 		= 10;
	GPIOPinWrite(GPIOB_BASE, A1_ind | A0_ind, A1_ind);
	Gain_R 		= 2	;
	GPIOPinWrite(GPIOB_BASE, A1_res | A0_res, A0_res);
	
	// Configuration is over --> raise condition	
	//indM_raise_conf_Ready(&ls);
	return (bool) true;
	
}	//end indM_safe_Close
