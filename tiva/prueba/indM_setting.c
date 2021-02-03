//--------------------------------------------------------------------------
//							indM_setting
//--------------------------------------------------------------------------

//#include "src-gen/indM.h"
//#include "src-gen/indM_required.h"
//#include "src/sc_timer_service.h"
#include "inc/bsp.h"
//#include "main.c"

    //                      CONSTANTES
    //-------------------------------------------------------------------
        float       extern    K_RT        ;
        uint32_t    extern    I_wished    ;

    //                      VARIABLES
    //-------------------------------------------------------------------
        IndM        extern      ls                   ;
        int         extern      Gain_L, Gain_R      ;   //Global
        uint32_t    extern      n_i                 ;   //Global
        uint32_t    extern      n_f                 ;   //Global
        uint32_t    volatile    aux_1       =   1u  ;   //local
        uint32_t    volatile    aux_0       =   1u  ;   //local
        uint32_t                puntero     =   0u  ;   //local
        float                   aux_f[1000] =   {0} ;   //local

        bool                    l_f_ireach  =   false;  //local
        bool                    l_WrongSetting= false;  //local
        bool                    l_gain_set  =   false;  //local



 void indM_setting(const IndM* handle, const sc_boolean gain_set, const sc_boolean f_ireach, const sc_boolean WrongSetting)	
{

	//						ROUTINE
	//-------------------------------------------------------------------

	//Turn on the FET.
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_6);
	
	//Get New Data
	// Dummy data for tx FIFO, to trigger the CS and CLK.
	SSIDataPut(SSI0_BASE, 0);
	// Get value from RX FIFO
	SSIDataGetNonBlocking(SSI0_BASE, &aux_0);
	SSIDataGetNonBlocking(SSI1_BASE, &aux_1);
	
	//Error verification
	++puntero;
	aux_f[puntero] = (float) ((K_RT/Gain_R) * aux_1); // aux_1 = V_r(uint32) ; aux_f = I_r(float)
	if (	(aux_f[puntero] > (aux_f[puntero-2]*1.1)	)	&&	(puntero > 20)){
			aux_f[puntero] = 1.1*aux_f[puntero-2]	;
	}
	
	//				Configuration Check
	
	//Data must be evaluated if positive
	if ( aux_0 < 33000  &&  aux_1 < 33000){
		
			
			l_gain_set = true; //if no other condition is satisfied, gain is set
			
		//Define if there was saturation in the measurement
			// Saturation in inductor measurement
				if ( aux_0 >  SATURATION_THRESHOLD2){
					//Configurate Gain_L and Gain_R
					if (Gain_L == 1000)
					{
						Gain_L = 100;
						GPIOPinWrite(GPIOB_BASE, A1_ind | A0_ind, A0_ind);
						//Check for compatible state in Gain_R
						if ( (Gain_R == 5 ) || ( Gain_R == 1 ) ){
							Gain_R = 2;
							GPIOPinWrite(GPIOB_BASE, A1_res | A0_res, A0_res);
						}
					}
					
					if (Gain_L == 100)
					{
						Gain_L = 10;
						GPIOPinWrite(GPIOB_BASE, A1_ind | A0_ind, A1_ind);

						if ( (Gain_R == 5 ) || ( Gain_R == 1 ) ){
							Gain_R = 2;
							GPIOPinWrite(GPIOB_BASE, A1_res | A0_res, A0_res);
						}
					}

					l_gain_set = false;
				}
			// Saturation in resistance measurement
				if ( aux_1 >  SATURATION_THRESHOLD){ 
					//Configurate Gain_L and Gain_R
					if (Gain_R == 10)
					{
						Gain_R = 5;
						GPIOPinWrite(GPIOB_BASE, A1_res | A0_res, A1_res);
						//The only Gain_L compatible is 1000, in other case, set Gain_R = 2.
						if (Gain_L != 1000 ){
							Gain_R = 2;
							GPIOPinWrite(GPIOB_BASE, A1_res | A0_res, A0_res);
						}
							
						
					}
						
					if (Gain_R == 5)
					{
						Gain_R = 2;
						GPIOPinWrite(GPIOB_BASE, A1_res | A0_res, A0_res);
					}
					else 
					{	//Only its posible to set Gain_R=1 if Gain_L =1000. In other case, it's better to let Gain_R=2
						if (Gain_L == 1000){
							Gain_R = 1;
							GPIOPinWrite(GPIOB_BASE, A1_res | A0_res, 0);
						}
						
					}
					
					l_gain_set = false;
				}
				
		//Define if current has already reached to the wished current.
				
			
			if (	l_gain_set	 ){
				if ( (aux_f[puntero]) >= (float)(I_wished) ) {
						
						n_f 		= SysTickValueGet()	;
						l_f_ireach 	= true				;				
						GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_7); //turn off the FET
					} 
 
			}
			else {  
				l_gain_set = false	; //I_wished wasn't reach yet				
			}
			

	}//end if-positive state
	else{
		l_WrongSetting = true;
	}	// wrong setting case
	
	//Update StateMachine flags
		indM_set_wrongSetting	(	&ls	, l_WrongSetting);
		indM_set_f_ireach		(	&ls	, l_f_ireach	);
		indM_set_gain_set		(	&ls	, l_gain_set	);
	

}// end indM_setting
