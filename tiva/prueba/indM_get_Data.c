 //--------------------------------------------------------------------------
//							indM_get_Data
//--------------------------------------------------------------------------

//#include "src-gen/indM.h"
//#include "src-gen/indM_required.h"
//#include "src/sc_timer_service.h"
#include "inc/bsp.h"

    //                      VARIABLES
    //-------------------------------------------------------------------
        IndM    extern  ls                              ;
        int32_t extern data[2];


 void indM_get_Data(const IndM* handle)
 {
	
	//						ROUTINE
	//-------------------------------------------------------------------
	
	//Measure the data
    //Dummy data for tx FIFO, to trigger the CS and CLK.
    SSIDataPut(SSI0_BASE, 0);
    //SSIDataPut(SSI1_BASE, 0);
    // Get value from RX FIFO
    SSIDataGetNonBlocking(SSI0_BASE, &data[0]); //V_L
    SSIDataGetNonBlocking(SSI1_BASE, &data[1]); //V_R
 
 }	//end indM_get_Data
