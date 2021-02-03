  //--------------------------------------------------------------------------
//							indM_control_and_RCalculation
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
        IndM    extern  ls                      ;
        //Configuration
        int     extern  Gain_L, Gain_R          ;   //GLOBAL
        int32_t extern  offset[2]               ;   //GLOBAL
        //Control Loop
        float extern    Vref                    ;
        float extern    v_gate[5]               ;
        uint32_t        v_gate_k                ;   //LOCAL
        int32_t extern  data[2]                 ;
        float volatile  V_L1                    ;   //LOCAL
        float volatile  V_R                     ;   //LOCAL
        float volatile  error[2]                ;   //LOCAL
        float volatile  u[2]                    ; //Control action
        bool            ss          = false     ; //Remains in false until Vref reach the stady state value
        //Resistance calculation
        int             count_prom      =   0   ;   //LOCAL
        float           Resistance_aux  =   0.0 ;   //LOCAL
        float   extern  Resistance              ;
        //float             Resistance[1000]        ;
        //float             Resistance_prom         ;
        //float extern  Resistance_f    =   0.0 ;
        //Run update
        uint32_t        runs_R                  ;   //LOCAL



 void indM_control_and_RCalculation(const IndM* handle, const sc_real* sp_Vref, const sc_real* sp_v_gate, const sc_integer* sp_data, const sc_real* sp_R)
 //void indM_control_and_RCalculation(const IndM* handle, float* sp_Vref, float* sp_v_gate, int* sp_data, float* sp_R);
 {

	//						ROUTINE
	//-------------------------------------------------------------------
	
	// Map data to voltage
	
		if (	*(sp_data) > 32768){
				//*(sp_data) -=65536	;
		        data[0] -= 65536    ;
		}
		if (	*(sp_data+1) > 32768){
		        data[1] -=65536	;
		}

		V_L1 	= (float) (2)*(K_LR/Gain_L) * (	*(sp_data	) 	- offset[0])	;
		V_R 	= (float) (K_RR*Rs/Gain_R) 	* (	*(sp_data+1	) 	- offset[1])	;

	//	Update Vref and steady state condition verification
		if (	*(sp_Vref)	< Vref_ss	){
		        Vref	+=	step;
		}
		else {
			//if ((error[0]< 0.001) && (error[0] > -0.001))
			if (	(	error[0]<V_th_ss	)	&&	(	error[0]>-V_th_ss	)	)
				{
					ss	=	true;
				}
				else{
					ss	=	false;
				}
		} //end if Vref-ramp

	// 				Control action
	//----------------------------------------------	
		
	    //Get the error signal
           error[0] = *(sp_Vref)	-	V_R	;

       //Call to controller's routine
            Control_Res(	&error, &u, &v_gate[0]	);

        //Application of control action
           //With linalization adjustment
            v_gate_k = (uint32_t) floor ( (*(sp_v_gate) + V_TH_FET)/ (V_MAX_DAC * Gain_OPA227) * NRO_STEP);

            v_gate_k &= 0x0FFF; // There're only 12 data bits
            v_gate_k <<= 2;
            SSIDataPut(SSI2_BASE, v_gate_k);
	
	
	// 				Configuration Verification
	//----------------------------------------------	
/*
	    Gains only need to be adjust in case there's a signal close to saturation.
		As the greates voltage over the sensing resistance is known, only saturation
		in winding resistance measurement needs to be contemplated.
*/
        if (	*(sp_data) < 33000	){
			if (	(	*(sp_data)>SATURATION_THRESHOLD_R	)	&&	(	ss!=true	)	) {
				//If Gain_L = 100, it should be drop down to 10.
				if (Gain_L = 100) {
					Gain_L = 10;
					GPIOPinWrite(GPIO_PORTB_BASE, A1_ind | A0_ind, A1_ind);
				}
				else {
					//If Gain_L = 10, it should be drop down to 1.
					if (Gain_L = 10) {
					Gain_L = 1;
					GPIOPinWrite(GPIO_PORTB_BASE, A1_ind | A0_ind, 0);
					}				
				}
				
			}
        }
	
	// 				Resistance Calculation
	//----------------------------------------------	
		//Get runs from StateMachine
		runs_R = indM_get_runs(&ls);
		
		//Rl calculation according to runs_R
	    if (ss){
            if (runs_R < 50000u){
				
				Resistance_aux = (float) ((V_L1*Rs)/ V_R ) ;
            }
            else {
                if (runs_R < 450000){
					
                    if (count_prom <1000)
					{
						count_prom++;
                    }
					
					Resistance_aux	=	(float)	(	(	((V_L1*Rs)/ V_R) + (Resistance_aux*(count_prom-1) ))	/	(count_prom)	);

                }  
            }//end else runs<450000
        }//if ss
	
	
	// Finally, return the Resistance value
	
	Resistance	=	Resistance_aux	;
	
	
}// end indM_control_and_RCalculation
