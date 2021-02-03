# InductanceMeter-Yakindu
C code for Inductance characterization L(i) implemented in a Cortex M4 microcontroller from Texas Instrument 

The main idea is to set a constant voltage over the inductor to generate a current ramp, then by linear regression estimate the inductance value for a given range of current.
This processing routine repeats in severan processing/samples windows.

An additional routine aims to control a constant current over the inductor to measure the DC winding resistance. By knowing this resistances, its possible to modify the voltage 
measured over the inductor to only consider the inductive component (as the inductor involves both, resistive and inductive components).

This version of the application was implemented by used of state chars, in order to achieve a more clear and organized flow. State char code was generated by Yakindu State Char
plugin.

C code for the application, includes, etc are in "tiva" folder.

In particular, "tiva\prueba" folder contains all C code developed for the Main processing of the application.

Also, a basic Python GUI was developed, which code is in "Python" folder
