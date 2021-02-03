//--------------------------------------------------------------------------
//							indM_port_configuration
//--------------------------------------------------------------------------

//#include "src-gen/indM.h"
//#include "src-gen/indM_required.h"
//#include "src/sc_timer_service.h"
#include "inc/bsp.h"

    //                      VARIABLES
    //-------------------------------------------------------------------
        IndM        extern      ls                  ;


bool indM_port_configuration(const IndM* handle)
{
    /*
                            CLK configuration
    */
    /*We first need to configure the clock source of the system to generate a signal
    for synchronization. The configuration below consist on:
		- Use de internal oscillator of 16Mhz.
		- Use the PLL to work with higher frecuencies.
		- Use the divider 2.5 to generate a clk signal of 80Mhz (which is the greater one) */
        SysCtlClockSet(SYSCTL_SYSDIV_2_5| SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);


    /*
                            PORT CONFIGURATION
    */

    // 			Switches 
    //Sw1
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        // Enable port F
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);  // Init PF4 as input
        GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //Sw2
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        // Enable port F
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);  // Init PF0 as input
        GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

        GPIOF->DIR = 0u; // Clear direction register
        GPIOF->DIR |= ( LED_RED | LED_BLUE | LED_GREEN); // Set outputs
    
	
	
	//          			ADC SPI COMMUNICATION

        uint32_t sysclk = SysCtlClockGet();
    //  SPI interface

    //1. Enable the peripherals
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0); //Enable the periphera of SSI for use.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Enable the GPIOA
        //ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_5);


    //2. Associate the pins of port A with the signals of SSI module.
        GPIOPinConfigure(GPIO_PA2_SSI0CLK);
        GPIOPinConfigure(GPIO_PA3_SSI0FSS);
        GPIOPinConfigure(GPIO_PA4_SSI0RX);
        GPIOPinConfigure(GPIO_PA5_SSI0TX);
        GPIOPinTypeSSI(GPIOA_BASE, GPIO_PIN_5 |GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);

    //3. Configure and enable the ssi port for sPI master mode.
/* 
			a) Use SSI0,
			b) System Clock supply
			c) Freescale mode SP1=1 and SPH=0 (accord to Analog devices datasheet)
			d) Master mode
			e) 1Mhz to 5Mhz (max)
			f) 16 bits word length
*/
        SSIConfigSetExpClk(SSI0_BASE, sysclk, SSI_FRF_MOTO_MODE_3,SSI_MODE_MASTER, 20000000, 16); // 1Mhz
        //SSIConfigSetExpClk(SSI0_BASE, sysclk, SSI_FRF_MOTO_MODE_2,SSI_MODE_MASTER, 3000000, 16); // 3Mhz
        //SSIConfigSetExpClk(SSI0_BASE, sysclk, SSI_FRF_MOTO_MODE_2,SSI_MODE_MASTER, 5000000, 16); // 5Mhz

        SSIEnable(SSI0_BASE); // Enable the SSI0 module.

    //4. As we need to read 2 data stream from the ADC. I'll initiate another SSI module
    //associated to the same signals but reading a different one.

        //4.1 Enable the peripeheral
            SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1); //Enable the periphera of SSI for use.
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); //Enable the GPIOA
           // ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
            GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_3);
        //4.2 Associate the pins of port A with the signals of SSI module.
            GPIOPinConfigure(GPIO_PD0_SSI1CLK);
            GPIOPinConfigure(GPIO_PD1_SSI1FSS);
            GPIOPinConfigure(GPIO_PD2_SSI1RX); //We change the pin that represent the data input
            GPIOPinConfigure(GPIO_PD3_SSI1TX );
            GPIOPinTypeSSI(GPIOD_BASE, GPIO_PIN_0 |GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_2);
        //4.3 Configure and enable the ssi port for sPI master mode.
            //SSIConfigSetExpClk(SSI1_BASE, sysclk, SSI_FRF_MOTO_MODE_3,SSI_MODE_MASTER, 20000000, 16); // 1Mhz

            SSIConfigSetExpClk(SSI1_BASE, sysclk, SSI_FRF_MOTO_MODE_3,SSI_MODE_SLAVE, 20000000, 16); // 1Mhz
            //SSIConfigSetExpClk(SSI1_BASE, sysclk, SSI_FRF_MOTO_MODE_2,SSI_MODE_MASTER, 3000000, 16); // 3Mhz
            //SSIConfigSetExpClk(SSI1_BASE, sysclk, SSI_FRF_MOTO_MODE_2,SSI_MODE_MASTER, 5000000, 16); // 5Mhz

            SSIEnable(SSI1_BASE); // Enable the SSI0 module.




    //     				Amplifier's Gain Configuration

    // First, enable the Port B
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        //ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //Second, define 4 output pins to configurate both amplifiers
/*
			1. A1_ind = GPIO_PIN_0
			2. A0_ind = GPIO_PIN_1
			3. A1_res = GPIO_PIN_2
			4. A0_res = GPIO_PIN_3
*/
        GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_0 );



		
		
    //      			Linear Driver Configuration
	
    /*The configuration of the linear driver is quiet similar to the one of the ADC.
    This is because, this configuration implice a SPI comunication with the DAC device and
    an output signal with should enable and disable the TBJ conected to the DAC output*/

    //1. Enable the port B and SSI module
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        //ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        //ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2); //Enable the periphera of SSI for use.
        
		//1.1 Enable an output pin for the TBJ signal.
            GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_1 | GPIO_PIN_2);
			
    //2. Associate the pins of port B with the signals of SSI module.
        GPIOPinConfigure(GPIO_PB4_SSI2CLK);
        GPIOPinConfigure(GPIO_PB5_SSI2FSS);
        GPIOPinConfigure(GPIO_PB6_SSI2RX);
        GPIOPinConfigure(GPIO_PB7_SSI2TX);
        GPIOPinTypeSSI(GPIOB_BASE, GPIO_PIN_4 |GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    //3. Configure and enable the ssi port for sPI master mode.
/* 
			a) Use SSI2,
			b) System Clock supply
			c) Freescale mode SPO=0 and SPH=1 (accord to Analog devices datasheet)
			d) Master mode
			e) 1Mhz to 5Mhz (max)
			f) 12 bits word length --> 12 data + 4 senseless bits
*/
        SSIConfigSetExpClk(SSI2_BASE, sysclk, SSI_FRF_MOTO_MODE_1,SSI_MODE_MASTER, 1000000, 16); // 1Mhz
        //SSIConfigSetExpClk(SSI2_BASE, sysclk, SSI_FRF_MOTO_MODE_1,SSI_MODE_MASTER, 1000000, 12); // 1Mhz
        //SSIConfigSetExpClk(SSI0_BASE, sysclk, SSI_FRF_MOTO_MODE_1,SSI_MODE_MASTER, 3000000, 12); // 3Mhz
        //SSIConfigSetExpClk(SSI0_BASE, sysclk, SSI_FRF_MOTO_MODE_1,SSI_MODE_MASTER, 5000000, 12); // 5Mhz

        SSIEnable(SSI2_BASE); // Enable the SSI2 module.



    //      			Switched driver Configuration (EDI)
	
    //1. Enable the port D --> was already enable 
        //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        //ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    //2. Enable 2 output pins to define the differential input signal for 1EDI driver
        /*
        1. Dpos = GPIO_PIN_6
        2. Dneg = GPIO_PIN_7
        */
        HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= GPIO_PIN_7;
        GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    
    //                      LEDS Configuration
    

        SYSCTL->RCGCGPIO |= (1U << 5); 						// Enables the run GPIOF
        SYSCTL->GPIOHBCTL |= (1u << 5); 					// Enables the AHB de GPIOF
        GPIOF_AHB->DIR |= (LED_RED | LED_BLUE | LED_GREEN);
        GPIOF_AHB->DEN |= (LED_RED | LED_BLUE | LED_GREEN);

    
    //                        UART Configuration
    
    //1. Enable the portA --> which was already enable above.
        //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //2. Configure the pin muxing for UART0 functions on port A0 and A1.
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
    //3. Enable the UART module
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    //4. Configurate the Clk source for the UART module
        UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);// Use the internal 16MHz oscillator as the UART clock source.
    //5. Load the recommended configuration
        GPIOPinTypeUART(GPIOA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    //6. Define the communication parameters
/*
				a) Port nro 0
				b) Bound rate 115200
				c) Clk Frecuency 16Mhz (this's from the source defined above)
*/
        UARTStdioConfig(0, 115200, 16000000);


        /*Auxiliar*/
        GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_6);


    
    //                      FPU Configuration
    
        FPULazyStackingEnable();
        FPUEnable();

    
    //                      MPU Configuration
    
     //Another 32Kb memory block is going to be enable to store data



        MPURegionSet(1, 0x20008000,
                      MPU_RGN_SIZE_32K |
                      MPU_RGN_PERM_NOEXEC |
                      MPU_RGN_PERM_PRV_RW_USR_RW |
                      MPU_RGN_ENABLE);

        MPUEnable( MPU_CONFIG_PRIV_DEFAULT);
        MPURegionEnable(1);
		
		
	// Configuration is over --> raise condition	
	
		//indM_raise_conf_Ready(&ls);
		//indM_set_conf_Ready(&ls, true);
		
		return true;
		
 } // end indM_port_configuration
