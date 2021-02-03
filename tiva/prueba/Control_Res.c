/*
 * Control_Res.c
 *
 *  Created on: 3 mar. 2019
 *      Author: Carnaghi
 *      Version: 1.0
 */

/*This routine applies the equation that defines the behavior of the
 * controller for the Resistance measurement loop.
 * It's objetive is to generate the control action for the constant
 * current that must flow through the inductor for the measurement.

The controller's equation is:
           kc       (s/zc + 1)
Gc(s) = ------ . --------------
           s        (s/p + 1)

which can be divided into two expressions:
          u(s)     kc                   kc.T. (1 + z^-1)
Gc1(s) = ----- = ------ ->  Gc1(z) = -------------------
          e(s)     s                       1 - z^-1

then:
        u(k) = (kc.T) (e(k) + e(k-1)) + u(k-1)
        u(k) = N1 (e(k) + e(k-1)) + u(k-1)

            v_gate(s)       (s/zc + 1)                1 +( Q - e^(-pT)) z^-1
Gc2(s) = ------------- = --------------- -> Gc2(z) = -----------------------
               u(s)          (s/p + 1)                  1 - e^(-pT) z^-1

Luego:
        v_gate(k) = u(k) + ( Q - e^(-pT)) u(k-1) + e^(-pT) v_gate(k-1)
        v_gate(k) = N21 u(k) + N22 u(k-1) + D2 v_gate(k-1)


where Q = (p-zc) / (zc.p)
being:
1. zc : the zero of the controller.
2. p : the pole of the controller.

*/

#include "inc/bsp.h"

const float N1 = 0.002; // N1=100T = 100 * 0.5e-5 = 1e-3
const float N21 =  0.032;
const float N22 = -7.798634e-04;
const float D2 = 3.9641429e-3;



void Control_Res ( float *pe, float *pu, float *pv)
{
    //u(k) = N1 e(k-1) + u(k-1)
    *pu = N1 *( *pe) + N1 *( *(pe + 1)) + *(pu + 1);
    //*pu =  N1 *( *(pe + 1)) + *(pu + 1);
    //v_gate(k) = u(k) + N2 u(k-1) + D2 v_gate(k-1)
    *pv = N21*(*pu) + N22 *( *(pu + 1)) + D2 *( *(pv + 1));

    //Actualizo valores
    *(pe + 1) = *(pe);
    *(pu + 1) = *(pu);
    *(pv + 1) = *(pv);

}


