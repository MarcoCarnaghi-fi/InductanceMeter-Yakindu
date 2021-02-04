//--------------------------------------------------------------------------
//							indM_uART_routine
//--------------------------------------------------------------------------


//#include "src-gen/indM.h"
//#include "src-gen/indM_required.h"
//#include "src/sc_timer_service.h"
#include "inc/bsp.h"
//#include "main.c"


    //                      VARIABLES
    //-------------------------------------------------------------------
         IndM    extern  ls                                             ;
         char            command                            =   0       ; //LOCAL
         uint32_t        send_index                         =   0u      ; //LOCAL
         char extern     bufsend[ARRAY_LENGTH*2]                        ; //GLOBAL
         char extern     bufsend_i[ARRAY_LENGTH*2]                      ; //GLOBAL



//void indM_uART_routine(const IndM* handle,const char *sp_bf,const char *sp_bf_i)
void indM_uART_routine(const IndM* handle)
 {
	
	//						ROUTINE
	//-------------------------------------------------------------------
    //pointer
    char*  sp_bf    = indM_get_sp_bf_Y( &ls );
    char*   sp_bf_i = indM_get_sp_bf_i_Y( &ls );
	
	if (UARTCharsAvail(UART0_BASE)) {
	   command = UARTCharGet(UART0_BASE);
	   if(command == 's') {
		   send_index = 0u;
		  while (send_index < 2*ARRAY_LENGTH) {
			   UARTCharPut(UART0_BASE, *(sp_bf + send_index));
			   send_index++;
		   }
		   send_index = 0u;
		  while (send_index < 2*ARRAY_LENGTH) {
			  UARTCharPut(UART0_BASE, *(sp_bf_i + send_index));
			  send_index++;
		  }
		  UARTCharPut(UART0_BASE, 0xFF); //Sends FF to indicate end of data send
		  UARTCharPut(UART0_BASE, 0xFF);
	   } else if (command == 'c') {
		   command = UARTCharGet(UART0_BASE);
		   if (command == 'b') {
			   indM_raise_play(&ls);

		   } else if (command == 'g') {
			   indM_raise_stop(&ls);
	            //Enable the Systick
	               SysTickDisable();

	               //Enable the Systick's interruptions
	               SysTickIntDisable();
	           }
	       }
	    }
	}// end     indM_uART_routine

