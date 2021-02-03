 //--------------------------------------------------------------------------
//							indM_res_Closing
//--------------------------------------------------------------------------

//#include "src-gen/indM.h"
//#include "src-gen/indM_required.h"
//#include "src/sc_timer_service.h"
#include "inc/bsp.h"
//#include "main.c"


    //                      CONSTANTES
    //-------------------------------------------------------------------
         float  extern   step        ;
         float  extern   K_LR        ;
         float  extern   K_RR        ;
         float  extern   piR         ;
         float  extern   V_MAX_DAC   ; // Referes to V_ref of DAC
         float  extern   V_MAX_ADC   ; // Referes to V_ref of DAC
         float  extern   Rs          ;
         float  extern   Gain_OPA227 ;
         float  extern   V_TH_FET    ;
         float  extern   Vref_ss     ; //6
         float  extern   V_th_ss     ;

    //                      VARIABLES
    //-------------------------------------------------------------------
        IndM        extern      ls                  ;


 bool indM_res_Closing(const IndM* handle, const sc_real* sp_v_gate)
{

     uint32_t         v_gate2_k            ;   //LOCAL
	//						ROUTINE
	//-------------------------------------------------------------------

	
	//Get previous value
	v_gate2_k = (uint32_t) floor ( ( *(sp_v_gate) + V_TH_FET)/ (V_MAX_DAC * Gain_OPA227) * NRO_STEP);

	
	// Discharging TBJ driver
	while ( v_gate2_k > 10u){

		v_gate2_k >>= 3		;
		v_gate2_k &= 0x0FFF	; // There're only 12 data bits
		v_gate2_k <<= 2		;
		SSIDataPut(SSI2_BASE, v_gate2_k);
		int count = 0		;
		while (count < 1000){
			count++;
		}
	}
	
	// Configuration is over --> raise condition	

	//indM_raise_conf_Ready(&ls);
	
	return true;
 
} // indM_res_Closing	
