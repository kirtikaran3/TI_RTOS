/*
 * myClocks.h
 *
 */

#ifndef MYCLOCKS_H_
#define MYCLOCKS_H_

//***** Prototypes ************************************************************
void initClocks(void);

//***** Defines ***************************************************************
#define LFXT_CRYSTAL_FREQUENCY_IN_HZ        32768                               // Freq of external crystal
#define HFXT_CRYSTAL_FREQUENCY_IN_HZ     48000000                               // Freq of external crystal

// The following oscillator and clock frequencies are set in the associated 'C' file
#define myDCO_FREQUENCY_IN_HZ             6000000
#define myREFO_FREQUENCY_IN_HZ          4 * 32768

#define myACLK_FREQUENCY_IN_HZ          32768 / 2
#define myBCLK_FREQUENCY_IN_HZ              32768
#define myMCLK_FREQUENCY_IN_HZ            6000000
#define myHSMCLK_FREQUENCY_IN_HZ          1500000
#define mySMCLK_FREQUENCY_IN_HZ        750 * 1000


#endif /* MYCLOCKS_H_ */

