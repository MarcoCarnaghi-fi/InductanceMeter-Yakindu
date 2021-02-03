#ifndef __BSP_H__
#define __BSP_H__

/* Board Support Package for the EK-TM4C123GXL board */

/*                            INCLUDE'S                                     */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "TM4C123GH6PM.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/fpu.h"
#include "driverlib/mpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
//#include "inc/tm4c_cmsis.h"
#include "driverlib/ssi.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "hw_gpio.h"
//#include "hw_types.hold"
#include "hw_memmap.h"
#include "hw_ssi.h"
#include "src/sc_types.h"
//#include "src/sc_timer_service.c"
#include "src/sc_timer_service.h"
//#include "src-gen/indM.c"
#include "src-gen/indM.h"
#include "src-gen/indM_required.h"
//#include "main.c"
//#include "constantes.h"

/*                            GLOBAL DEFINITIONS                            */

/* system clock setting [Hz] */

    #define SYS_CLOCK_HZ 80000000U
	//Rl_Measurement
		#define NRO_STEP 				4096        // 2^12 (DAC)
		#define NRO_STEP_ADC 			32768
		#define SATURATION_THRESHOLD_R 	26214
		//#define ARRAY_LENGTH 			1000
	//Time_Gain_Set
		#define SATURATION_THRESHOLD    16000       // = 0.8 * 2^16
		#define SATURATION_THRESHOLD2   16000
		//#define FREC_RESAMPLING 1U
		//#define Fsamp 500000U
		
	//IND MEASUREMENT
		#define MAX_SAMPLES             1050       //RECORDAR MODIFICAR CUANDO HAYAMOS LOGRADO HABILITAR OTRO BLOQUE DE RAM
		#define neighbour               100u
		//#define Fs                      500000
		#define SCALE_FACTOR_L          1000000    // for microHy.
		#define SCALE_FACTOR_I          100
		#define SIGNIFICANT_FIGURES     10
		#define TMEAS                   100         // Measurement period
		#define ARRAY_LENGTH            75          // Measured inductance data length
		#define SLIDING_WINDOW_STEP     10

/*                            GLOBAL CONSTANTS                            */
    //RL MEASUREMENT


/*
#define step            0.0141
#define K_LR            1.873251748/1000
#define K_RR            2.65958/1000
#define piR             3.1415926
#define V_MAX_DAC       2.5            // Referes to V_ref of DAC
#define V_MAX_ADC       4.096          // Referes to V_ref of DAC
#define Rs              0.047
#define Gain_OPA227     2
#define V_TH_FET        3.06
#define Vref_ss         0.282          //6
#define V_th_ss         4.7/1000

    //TIME GAIN SET
#define K_RT            2.65958/1000
#define I_wished        20

    //IND MEASUREMENT
#define K_L             1.873251748/1000
#define K_R             1.874890/100
#define pi              3.1415926
#define T               2/1000000
*/
/* on-board LEDs */
#define LED_RED   (1U << 1)
#define LED_BLUE  (1U << 2)
#define LED_GREEN (1U << 3)

/* on-board SW */

#define SW1 (1u << 4)
#define SW2 (1u << 0)

/* J1 Pins */
#define PB5 (1u << 5) // Pin 1.02
#define PB0 (1u << 0) // Pin 1.03
#define PB1 (1u << 1) // Pin 1.04
#define PE4 (1u << 4) // Pin 1.05
#define PE5 (1u << 5) // Pin 1.06
#define PB4 (1u << 4) // Pin 1.07
#define PA5 (1u << 5) // Pin 1.08
#define PA6 (1u << 6) // Pin 1.09
#define PA7 (1u << 7) // Pin 1.10

/* J2 Pins */
#define PB2 (1u << 2)// Pin 2.02
#define PE0 (1u << 0) // Pin 2.03
// Pin 2.04 shared with SW2/Wake
#define PB7 (1u << 7) // Pin 2.06
#define PB6 (1u << 6) // Pin 2.07
#define PA4 (1u << 4) // Pin 2.08
#define PA3 (1u << 3) // Pin 2.09
#define PA2 (1u << 2) // Pin 2.10

///* J3 Pins */
#define PD0 (1u << 0)// Pin 3.03
#define PD1 (1u << 1)// Pin 3.04
#define PD2 (1u << 2)// Pin 3.05
#define PD3 (1u << 3)// Pin 3.06
#define PE1 (1u << 1)// Pin 3.07
#define PE2 (1u << 2)// Pin 3.08
#define PE3 (1u << 3)// Pin 3.09
#define PF1 (1u << 1)// Pin 3.10

/* J4 Pins */
#define PF2 (1u << 2)// Pin 4.01
#define PF3 (1u << 3)// Pin 4.02
#define PB3 (1u << 3)// Pin 4.03
#define PC4 (1u << 4)// Pin 4.04
#define PC5 (1u << 5)// Pin 4.05
#define PC6 (1u << 6)// Pin 4.06
#define PC7 (1u << 7)// Pin 4.07
#define PD6 (1u << 6)// Pin 4.08
#define PD7 (1u << 7)// Pin 4.09
#define PF4 (1u << 4) // Pin 4.10

#define TEMPREAD_FS 4095.00 // Full scale temperature reading
#define DEBOUNCEDELAY 100000/20 // 50ms debounce (50ms/10us=100000/20)

//AMPS
#define A1_ind (1u << 0)
#define A0_ind (1u << 1)
#define A1_res (1u << 2)
#define A0_res (1u << 3)
//EDI
#define Dpos (1u << 5)
#define Dneg (1u << 6)




/*
	//RL MEASUREMENT
		//const uint32_t	MAX_RUNS 	= 500000u		;
		const float 	step 		= 0.0141		;
		//const SYS_CLOCK_HZ = 80000000; // 80Mhz
		const float 	K_LR 		= 1.873251748e-3;
		const float 	K_RR 		= 2.65958e-3	;
		const float 	piR 		= 3.1415926		;
		const float 	V_MAX_DAC 	= 2.5			; // Referes to V_ref of DAC
		const float 	V_MAX_ADC 	= 4.096			; // Referes to V_ref of DAC
		const float 	Rs 			= 0.047			;
		const float 	Gain_OPA227 = 2 			;
		const float 	V_TH_FET 	= 3.06			;
		//const float Vref_ss = 0.47; //10
		//const float Vref_ss = 0.329; //7
		const float 	Vref_ss 	= 0.282			; //6
		//const float Vref_ss = 0.235; //5
		//const float Vref_ss = 0.1883; //4
		//const float Vref_ss = 0.141; // 3
		//const float Vref_ss = 0.047; //1
		const float 	V_th_ss 	= 4.7e-3		;
		
	//TIME GAIN SET
		const float 	K_RT 		=  	2.65958e-3	;
		const uint32_t 	I_wished 	= 	20u			;
	
	//IND MEASUREMENT
		const float K_L = 1.873251748e-3	;
		const float K_R = 1.874890e-2		;
		const float pi 	= 3.1415926			;
		const float T 	= 2e-6				;

*/

/*                          Data struct                                         */
/*
		typedef struct
		{
		    float  Ind[150]                 ;
		    float  Current[150]             ;
		} L_i_pair;
*/
/*                            Function's prototypes                            */


float least_square (float *sp, uint32_t n, float delta_T);
void Local_Average(float *sp_vector_av, float *sp, uint32_t points, int intervals, int step);
void Control_Res ( float *pe, float *pu, float *pv);
//SysTick
void SysTick_Configuration(int period);
void SysTick_Init(uint32_t ticks);
void SysTick_Handler_Hook(void);


//Interface
uint32_t sw_debounce (uint32_t sw);

void assert_failed (char const *file, int line);
#endif // __BSP_H__


//void Gc(float *sp_vr, float *sp_vg);

//void Res_Med(float Resistance_f);
//float Res_Med(void);
//void Time_Gain_set(uint32_t Fs, uint32_t FREC_RESAMPLING, uint32_t *sp);
//int Time_Gain_set(uint32_t Fs, uint32_t FREC_RESAMPLING, uint32_t sp);

/*                            Constant's Definition                           */
//#define K_L  1.873251748e-3
//const float K_R = 1.874890e-2;
//#define K_R  2.65958e-3
//#define  pi  3.1415926
//#define SATURATION_THRESHOLD 26214


