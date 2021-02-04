/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco Carnaghi
  * @version Me cago en todo
  * @date    17-December-2020
  * @brief   Default main function.
  ******************************************************************************
*/

//INCLUDES

//#include "src-gen/indM.h"
//#include "src-gen/indM_required.h"
//#include "src/sc_timer_service.h"
#include "inc/bsp.h"
//#include "Entradas.h"
//#include "Salidas.h"
//#include "ICTimer.h"
//#include "SysTick.h"


//DEFINES
#define MAX_TIMERS 4
/*
#define SYS_CLOCK_HZ 80000000U
    //Rl_Measurement
        #define NRO_STEP                4096        // 2^12 (DAC)
        #define NRO_STEP_ADC            32768
        #define SATURATION_THRESHOLD_R  26214
        //#define ARRAY_LENGTH            1000
    //Time_Gain_Set
        #define SATURATION_THRESHOLD    16000       // = 0.8 * 2^16
        #define SATURATION_THRESHOLD2   16000
        //#define FREC_RESAMPLING 1U
        //#define Fsamp 500000U

    //IND MEASUREMENT
        #define MAX_SAMPLES             1050       //RECORDAR MODIFICAR CUANDO HAYAMOS LOGRADO HABILITAR OTRO BLOQUE DE RAM
        #define neighbour               100
        //#define Fs                      500000
        #define SCALE_FACTOR_L          1000000    // for microHy.
        #define SCALE_FACTOR_I          100
        #define SIGNIFICANT_FIGURES     10
        #define TMEAS                   100         // Measurement period
        #define ARRAY_LENGTH            75          // Measured inductance data length
        #define SLIDING_WINDOW_STEP     10
*/
//CONSTANTS
/*                            GLOBAL CONSTANTS                            */

    //RL MEASUREMENT
         float     step         = 0.0141        ;
         float     K_LR         = 1.873251748e-3;
         float     K_RR         = 2.65958e-3    ;
         float     piR          = 3.1415926     ;
         float     V_MAX_DAC    = 2.5           ; // Referes to V_ref of DAC
         float     V_MAX_ADC    = 4.096         ; // Referes to V_ref of DAC
         float     Rs           =  0.047         ;
         float     Gain_OPA227  = 2             ;
         float     V_TH_FET     = 3.06          ;
         float     Vref_ss      = 0.282         ; //6
         float     V_th_ss      = 4.7e-3        ;

    //TIME GAIN SET
         float      K_RT        =   2.65958e-3  ;
         uint32_t   I_wished    =   20u         ;

    //IND MEASUREMENT
         float      K_L         = 1.873251748e-3    ;
         float      K_R         = 1.874890e-2       ;
         float      pi          = 3.1415926         ;
         float      T           = 2e-6              ;

//VARIABLES
	//STATE MACHINE
		IndM            ls                      ;
	
	//Rl MEASUREMENT
		int 	 		Gain_L, Gain_R			; //GLOBAL	
		int32_t	 		offset[2]				; //GLOBAL
		float  	        Vref 			=	0.0	;
		float  	        v_gate[5] 		=	{0}	;
		int32_t  		data[2]					;
		float			Resistance		=	0.0	;
	
	// TIMEGAIN SET
		uint32_t 	n_i 			=	0u	;
		uint32_t 	n_f 			=	0u	;
		int 		NRO_SAMPLES		=	0	;
		
	//UART
		char bufsend[ARRAY_LENGTH*2]	    =	{}		; //GLOBAL
		char bufsend_i[ARRAY_LENGTH*2]   =	{}		; //GLOBAL
        uint32_t        s_index                 =   0u      ;
        uint32_t        a_index                 =   0u      ;
		
	//MAIN MEASUREMENT
		uint32_t  samp_L[MAX_SAMPLES] ;
		uint32_t  samp_R[MAX_SAMPLES] ;

/*
		typedef struct
		{
		    float  Ind[150]                 ;
		    float  Current[150]             ;
		} L_i_pair                          ;

		L_i_pair 		L_i					;
*/



//! We allocate the desired array of timers.
static sc_timer_t timers[MAX_TIMERS];

//! The timers are managed by a timer service. */
static sc_timer_service_t timer_service;

//! callback implementation for the setting up time events
extern void indM_set_timer(IndM* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic)
{
    sc_timer_start(&timer_service, handle, evid, time_ms, periodic);
}

//! callback implementation for canceling time events.
extern void indM_unset_timer(IndM* handle, const sc_eventid evid)
{
    sc_timer_cancel(&timer_service, evid);
}


void main (void)
{
	//Start StateMachine
	indM_init(&ls);
	indM_enter(&ls);
	//Timers start
	sc_timer_service_init(&timer_service, timers, MAX_TIMERS,
					                (sc_raise_time_event_fp) &indM_raise_time_event);
	//Systick Start
	Systick_Configuration(10);
	
	//UART buffer init
      for (a_index = 0; a_index < ARRAY_LENGTH; ++a_index) {

            bufsend[s_index] = (char )(0x00FF & 0xBE);
            bufsend[s_index+1] = (char )(0x00FF & 0xEF);
            bufsend_i[s_index] = (char )(0x00FF & 0xBE);
            bufsend_i[s_index+1] = (char )(0x00FF & 0xEF);

            s_index += 2;
        }


	while (1) 
		{

			indM_set_sp_Vref	(&ls	, &Vref			);
			indM_set_sp_v_gate	(&ls	, &v_gate[0]	);
			indM_set_sp_data	(&ls	, &data[0]		);
			indM_set_sp_R		(&ls	, &Resistance	);
			indM_set_sp_bf_Y    (&ls    , &bufsend[0]   );
			indM_set_sp_bf_i_Y  (&ls    , &bufsend_i[0] );

			indM_run_cycle(&ls);	//StateMachine run
			//Sleep(10);		// Seteo el ciclo RTC en 10 ms.
		}
}

// ----------------------------------------------------------------------------


//--------------------------------------------------------------------------
//
//							Functions
//
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//                          Systick_Configuration
//--------------------------------------------------------------------------

void Systick_Configuration (int period)
{

    //Define the SysTick_Handler
   SysTickIntRegister( (*SysTick_Handler_Hook) );
   //Period is the number of 1us
   //Configure 1us SysTick interruption
   uint32_t count = floor(period * (SYS_CLOCK_HZ/1000000) ) ;
   SysTick_Init(count - 1U);

   //Enable the Systick
   SysTickEnable();
   //Enable the Systick's interruptions
   SysTickIntEnable();

}//end Systick_Configuration

//--------------------------------------------------------------------------
//                          SysTick_Handler_Hook
//--------------------------------------------------------------------------

void SysTick_Handler_Hook (void) // Para colgarse del System Tick
{
   //Timers Updates
   sc_timer_service_proceed(&timer_service, 1); //+1us

}//end SysTick_Handler_Hook



//--------------------------------------------------------------------------
//							indM_systick_Conf
//--------------------------------------------------------------------------

 void indM_systick_Conf(const IndM* handle, const sc_integer period)

 {	
	
	Systick_Configuration (period)	;

 } //endindM_systick_Conf
 
 
 
 
 
 
 //--------------------------------------------------------------------------
//							indM_handler_Disable
//--------------------------------------------------------------------------

 void indM_handler_Disable(const IndM* handle)
 {
	//Enable the Systick
   SysTickDisable();
   //Enable the Systick's interruptions
   SysTickIntDisable();
   
 }//end  indM_handler_Disable
 
 
 
 
 

