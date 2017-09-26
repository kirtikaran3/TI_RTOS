/*
 * ======== hal.h ========
 *
 * Device and board specific pins need to be configured here
 *
 */

#ifndef HAL_H_
#define HAL_H_

//***********************************************************************
// Only one of the following defines should be enabled. The symbols
// __MSP430F5529, __MSP430FR4133, __MSP430F5529 and __MSP432P401R__ are 
// defined by each of the processors respective header files (shipped 
// with the MSP430 or MSP432 Code Generation Tools
//***********************************************************************

#ifdef __MSP430F5529
    #define BUTTON1_PORT    GPIO_PORT_P2
    #define BUTTON1_PIN     GPIO_PIN1
    #define BUTTON1_VECTOR  PORT2_VECTOR

    #define BUTTON2_PORT    GPIO_PORT_P1
    #define BUTTON2_PIN     GPIO_PIN1
    #define BUTTON2_VECTOR  PORT1_VECTOR

    #define LED1_PORT       GPIO_PORT_P1
    #define LED1_PIN        GPIO_PIN0

    #define LED2_PORT       GPIO_PORT_P4
    #define LED2_PIN        GPIO_PIN7

    #define LFXT_DO_SETUP   1
    #define LFXTIN_PORT     GPIO_PORT_P5
    #define LFXTIN_PIN      GPIO_PIN4

    #define LFXTOUT_PORT    GPIO_PORT_P5
    #define LFXTOUT_PIN     GPIO_PIN5

//    #define HFXT_DO_SETUP   1
    #define HFXTIN_PORT     GPIO_PORT_P5
    #define HFXTIN_PIN      GPIO_PIN2

    #define HFXTOUT_PORT    GPIO_PORT_P5
    #define HFXTOUT_PIN     GPIO_PIN3

    #define TIMER_BASE      TIMER_A0_BASE
    #define TIMER_PORT      GPIO_PORT_P1
    #define TIMER_PIN       GPIO_PIN3
    #define TIMER_VECTOR0   TIMER0_A0_VECTOR
    #define TIMER_VECTOR1   TIMER0_A1_VECTOR
    #define TIMER_IV        TA0IV

    #define UNLOCK_PINS
#endif

#ifdef __MSP430FR4133
    #define BUTTON1_PORT    GPIO_PORT_P1
    #define BUTTON1_PIN     GPIO_PIN2
    #define BUTTON1_VECTOR  PORT1_VECTOR

    #define BUTTON2_PORT    GPIO_PORT_P2
    #define BUTTON2_PIN     GPIO_PIN6
    #define BUTTON2_VECTOR  PORT2_VECTOR

    #define LED1_PORT       GPIO_PORT_P1
    #define LED1_PIN        GPIO_PIN0

    #define LED2_PORT       GPIO_PORT_P4
    #define LED2_PIN        GPIO_PIN0

    #define LFXT_DO_SETUP   1
    #define LFXTIN_PORT     GPIO_PORT_P4
    #define LFXTIN_PIN      GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION

    #define LFXTOUT_PORT    GPIO_PORT_P4
    #define LFXTOUT_PIN     GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION

//    #define HFXT_DO_SETUP   1
    #define HFXTIN_PORT     
    #define HFXTIN_PIN      

    #define HFXTOUT_PORT    
    #define HFXTOUT_PIN     

    #define TIMER_BASE      TIMER_A1_BASE
    #define TIMER_PORT      GPIO_PORT_P8
    #define TIMER_PIN       GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION
    #define TIMER_VECTOR0   TIMER1_A0_VECTOR
    #define TIMER_VECTOR1   TIMER1_A1_VECTOR
    #define TIMER_IV        TA1IV

    #define UNLOCK_PINS     PMM_unlockLPM5();
#endif

#ifdef __MSP430FR5969
    #define BUTTON1_PORT    GPIO_PORT_P4
    #define BUTTON1_PIN     GPIO_PIN5
    #define BUTTON1_VECTOR  PORT2_VECTOR

    #define BUTTON2_PORT    GPIO_PORT_P1
    #define BUTTON2_PIN     GPIO_PIN1
    #define BUTTON2_VECTOR  PORT1_VECTOR

    #define LED1_PORT       GPIO_PORT_P4
    #define LED1_PIN        GPIO_PIN6

    #define LED2_PORT       GPIO_PORT_P1
    #define LED2_PIN        GPIO_PIN0

    #define LFXT_DO_SETUP   1
    #define LFXTIN_PORT     GPIO_PORT_PJ
    #define LFXTIN_PIN      GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION

    #define LFXTOUT_PORT    GPIO_PORT_PJ
    #define LFXTOUT_PIN     GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION

//    #define HFXT_DO_SETUP   1
    #define HFXTIN_PORT     GPIO_PORT_PJ
    #define HFXTIN_PIN      GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION

    #define HFXTOUT_PORT    GPIO_PORT_PJ
    #define HFXTOUT_PIN     GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION

    #define TIMER_BASE      TIMER_A1_BASE
    #define TIMER_PORT      GPIO_PORT_P1
    #define TIMER_PIN       GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION
    #define TIMER_VECTOR0   TIMER1_A0_VECTOR
    #define TIMER_VECTOR1   TIMER1_A1_VECTOR
    #define TIMER_IV        TA1IV

    #define UNLOCK_PINS     PMM_unlockLPM5();
#endif

#ifdef __MSP430FR6989
    #define BUTTON1_PORT    GPIO_PORT_P1
    #define BUTTON1_PIN     GPIO_PIN1
    #define BUTTON1_VECTOR  PORT1_VECTOR

    #define BUTTON2_PORT    GPIO_PORT_P1
    #define BUTTON2_PIN     GPIO_PIN2
    #define BUTTON2_VECTOR  PORT1_VECTOR

    #define LED1_PORT       GPIO_PORT_P1
    #define LED1_PIN        GPIO_PIN0

    #define LED2_PORT       GPIO_PORT_P9
    #define LED2_PIN        GPIO_PIN7

    #define LFXT_DO_SETUP   1
    #define LFXTIN_PORT     GPIO_PORT_PJ
    #define LFXTIN_PIN      GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION

    #define LFXTOUT_PORT    GPIO_PORT_PJ
    #define LFXTOUT_PIN     GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION

//    #define HFXT_DO_SETUP   1
    #define HFXTIN_PORT     GPIO_PORT_PJ
    #define HFXTIN_PIN      GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION

    #define HFXTOUT_PORT    GPIO_PORT_PJ
    #define HFXTOUT_PIN     GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION

    #define TIMER_BASE      TIMER_A1_BASE
    #define TIMER_PORT      GPIO_PORT_P1
    #define TIMER_PIN       GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION
    #define TIMER_VECTOR0   TIMER1_A0_VECTOR
    #define TIMER_VECTOR1   TIMER1_A1_VECTOR
    #define TIMER_IV        TA1IV

    #define UNLOCK_PINS     PMM_unlockLPM5();
#endif

#ifdef __MSP432P401R__
    #define BUTTON1_PORT    GPIO_PORT_P1
    #define BUTTON1_PIN     GPIO_PIN1
    #define BUTTON1_VECTOR  PORT1_VECTOR

    #define BUTTON2_PORT    GPIO_PORT_P1
    #define BUTTON2_PIN     GPIO_PIN4
    #define BUTTON2_VECTOR  PORT1_VECTOR

    #define LED1_PORT       GPIO_PORT_P1
    #define LED1_PIN        GPIO_PIN0

    #define LED2_PORT       GPIO_PORT_P2
    #define LED2_PIN        GPIO_PIN1

    #define LFXT_DO_SETUP	1
	#define LFXTIN_PORT     GPIO_PORT_PJ
	#define LFXTIN_PIN      GPIO_PIN0

	#define LFXTOUT_PORT    GPIO_PORT_PJ
	#define LFXTOUT_PIN     GPIO_PIN1

    #define HFXT_DO_SETUP   1
	#define HFXTIN_PORT     GPIO_PORT_PJ
	#define HFXTIN_PIN      GPIO_PIN3

	#define HFXTOUT_PORT    GPIO_PORT_PJ
	#define HFXTOUT_PIN     GPIO_PIN2

	#define TIMER_BASE      TIMER_A0_BASE
	#define TIMER_PORT      GPIO_PORT_P2
	#define TIMER_PIN       GPIO_PIN5
	#define NVIC_TIMER_BIT  TA0_N_IRQn
	#define TIMER_VECTOR0   TIMER0_A0_VECTOR
	#define TIMER_VECTOR1   TIMER0_A1_VECTOR
	#define TIMER_IV        TA0IV

    #define UNLOCK_PINS
#endif

//***** Prototypes ************************************************************
void initGPIO(void);

#endif /* HAL_H_ */
