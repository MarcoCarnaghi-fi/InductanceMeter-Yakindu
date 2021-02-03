 //--------------------------------------------------------------------------
//							indM_res_Init
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
        //Configuration
         int     extern  Gain_L, Gain_R          ; //GLOBAL
         int32_t extern  offset[2]               ; //GLOBAL
         //Control Loop
         float  extern      Vref                    ;
         float  extern      v_gate[5]               ;
         uint32_t           v_gate_k                ; // LOCAL
         int32_t extern     data[2]                 ;
         //Runs update
         //uint32_t      runs_R      = 0         ;
//         int                i                       ;

 bool indM_res_Init(const IndM* handle, const sc_real* sp_Vref, const sc_real* sp_v_gate, const sc_integer* sp_data)
 {
	
	//						ROUTINE
	//-------------------------------------------------------------------
	
	//Turn off the TBJ.
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1, GPIO_PIN_1 | GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7,GPIO_PIN_7);
    
	//Indicate the Mode has iniciated(green) and the configuration is in process(red).
    GPIOPinWrite(GPIO_PORTF_AHB_BASE, LED_RED | LED_BLUE | LED_GREEN, LED_GREEN | LED_RED);
	
	// Gain Configuration
	//-------------------
/*	
			Two gains to set:
			a) The amp which measure the Res voltage
			b) The amp which measure the Ind voltage
			For a) we set a constant gain. Because it's known the range of voltage over the resistance
			For b) we first set the maximum gain, then the routine itself will modify it if the measure overflows
			the ADC range of voltage
*/
	GPIOPinWrite(GPIO_PORTB_BASE, A1_res | A0_res, A1_res | A0_res);
    Gain_R = 10;
    GPIOPinWrite(GPIO_PORTB_BASE, A1_ind | A0_ind, A0_ind);
    Gain_L = 100;

	
	// Input signal and Control action
	//--------------------------------
	
/*	
		The process consist on:
			1) Set a ramp to take softly the system to the wished tension in stady state.
			2) In steady state voltage value, keep it constant and the measurement starts.
*/
	//	Set the TBJ.
		GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1, 0);

    //	Initiate the Vref
		Vref 		= step;
		v_gate[1] 	= step;

    //	With linealization adjustment
		v_gate_k = (uint32_t) floor ( (*(sp_v_gate+1) + V_TH_FET)/ (V_MAX_DAC * Gain_OPA227) * NRO_STEP);


    //	Write the value in the DAC
		SSIDataPut(SSI2_BASE, 0)	; // Dummy data for initiate the CLK
		v_gate_k &= 0x0FFF			; // 12 data bits
		v_gate_k <<= 2				;
		SSIDataPut(SSI2_BASE, v_gate_k);

		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6, GPIO_PIN_6);
	
	//	Clean SSI buffers
		while(SSIDataGetNonBlocking(SSI0_BASE, data[0]		)){}
        while(SSIDataGetNonBlocking(SSI1_BASE, data[1]	    )){}
		//Read turn off tension --> offset
			for (int i = 0; i<10 ; i++ ){
                    // Dummy data for tx FIFO, to trigger the CS and CLK.
                    SSIDataPut(SSI0_BASE, 0);
                    // Get value from RX FIFO
                       // SSIDataGet(SSI0_BASE, &offset[0]);
                       // SSIDataGet(SSI1_BASE, &offset[1]);
            }

            if ( offset[0] > 32768){
                offset[0] -=65536;
            }
            if ( offset[1] > 32768){
                offset[1] -=65536;
            }
			
	//	Indicate the configuration is over.
		GPIOPinWrite(GPIO_PORTF_AHB_BASE, LED_RED | LED_BLUE | LED_GREEN, LED_GREEN);
		
		//indM_set_f_res_configured(&ls, true);
		
		return true;
		
 }// end indM_res_Init
