/*
 * least_square.c
 *
 *  Created on: 23 feb. 2019
 *      Author: Carnaghi
 *
 *  The purpose of this code is to apply the least square method
 *  in order to obtain the slope that best fit the data sampled.
 *  For subsequent calculations, it's need the di/dt, which will be
 *  approximated by the slope of i considering a certain period of
 *  time to calculate it.
 */

#include "inc/bsp.h"

// a. Definitions
float   A11,A12,A22,B1,B2   ;
float   t, I                ; // auxiliar variables that represent the value in the operation point
float   det                 ; //array's det
float   result              ;
//int     j =0                ;

float least_square (float *sp, uint32_t n, float delta_T){


    // b. Arrays generation
        //  1. A11 = n;
        //  2. A12 = A21 = sum(t)
        //  3. A22 = sum(t^2)
        //  4. B1 = sum(I)
        //  5. B2 = sum(I.*t)
            //A11
            A11 = (float)(n);

            //A12, A21, A22, B1, B2
            A12=0; //inicializacion
            A22=0;
            B1=0;
            B2=0;
            for (int j = 0; j < n ; j++)
            {
                t = (float)(j * delta_T);
                A12 = A12 + t;
                A22 = A22 + (pow(t,2));
                I= *(sp + j); //
                B1= B1 + I;
                B2= B2 + ( I * t );
            } //end for

    // c. Slope calculation

    det =(float)( (A11 * A22) - (A12 * A12) );

    // Not tested modification
    if (det < 0.00001){
        result = 0.0;
    }
    else {
        result = (float)(( (A11 * B2) - (A12 * B1) ) / ( det ) );
    }
    return result;
}//end Least squares




