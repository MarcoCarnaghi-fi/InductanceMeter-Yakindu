/*
 * Local_Averange.c
 *
 *  Created on: 23 feb. 2019
 *      Author: Carnaghi
 *      Version: 3.4
 *
 * This routine divides the input array into the number of intervals specified.
 * Then calculates the average value for that interval and stores it in the output
 * array.
 *
 * The only modification respect to the previous version is the 50-samples step,
 *instead of the 100-samples step.
 *
 */

#include "inc/bsp.h"
//uint32_t sp_aux;
//int j=0;
//int k=0;

void Local_Average(float *sp_vector_av, float *sp,
                    uint32_t points, int intervals, int step) {

// The function stores in *Sp_vector_av the "intervals" averanged
// values of "points" values of *sp

    // 0. Definitions

    //sp_aux = sp;

    // 1. Calculation

    for (int j = 0 ; j < intervals ; j++){

        // Averange the k-interval
        // For that:

        // a.the algorith accumulates in the position j
        // of the averenged vector the k values of the original
        // data vector that are included in the j-interval

        for (int k = 0 ; k < points ; k++){
            *(sp_vector_av + j) += *(sp + k + j*(step));
        }//end for - k

        //b. divide into the number of points
        *(sp_vector_av + j ) /= points;

        //And, update the pointer to the next interval
        //sp_aux += points;
    }//end for - j

}//end Local_Averange
