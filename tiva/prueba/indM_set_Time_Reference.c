
//--------------------------------------------------------------------------
//							indM_set_Time_Reference
//--------------------------------------------------------------------------

//#include "src-gen/indM.h"
//#include "src-gen/indM_required.h"
//#include "src/sc_timer_service.h"
#include "inc/bsp.h"


    //                      VARIABLES
    //-------------------------------------------------------------------
        IndM        extern ls               ;
        uint32_t    extern  n_i             ;

 void indM_set_Time_Reference(const IndM* handle, const sc_integer ni)	
{
	//						ROUTINE
	//-------------------------------------------------------------------

	//Initialize reference
	n_i = SysTickValueGet();
	
	//Update StateMachine var
	indM_set_n_i(&ls, n_i);
	
} // end indM_set_Time_Reference
