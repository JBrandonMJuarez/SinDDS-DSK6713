/*************************************************************************
 *  Basic stereo loop code for C6713 DSK and AIC23 codec
 *  D. Richard Brown on 22-Aug-2011
 *  Based on code from "Real-Time Digital Signal Processing Based on TMS320C6000"
 *  by N. Kehtarnavaz and N. Kim.
 *************************************************************************/

#define CHIP_6713 1

#include <stdio.h>
#include <c6x.h>
#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_irq.h>

#include <math.h>
#include <stdlib.h>

#include "dsk6713.h"
#include "dsk6713_aic23.h"

#define sineTableLen 128
short loop = 0;
short gain = 80;
short frequency = 2;
short sine_table[sineTableLen]={0};

interrupt void serialPortRcvISR(void);

DSK6713_AIC23_CodecHandle hCodec;                           // Codec handle
DSK6713_AIC23_Config config = DSK6713_AIC23_DEFAULTCONFIG;  // Codec configuration with default settings

interrupt void serialPortRcvISR(void);

void main()
{
    short i=0;
    float pi=3.14159;
    for(i = 0; i < sineTableLen; i++){
        sine_table[i] = 300*sin(2.0*pi*i/sineTableLen);
    }
    DSK6713_init();     // Initialize the board support library, must be called first
    hCodec = DSK6713_AIC23_openCodec(0, &config);   // open codec and get handle

    // Configure buffered serial ports for 32 bit operation
    // This allows transfer of both right and left channels in one read/write
    MCBSP_FSETS(SPCR1, RINTM, FRM);
    MCBSP_FSETS(SPCR1, XINTM, FRM);
    MCBSP_FSETS(RCR1, RWDLEN1, 32BIT);
    MCBSP_FSETS(XCR1, XWDLEN1, 32BIT);

    // set codec sampling frequency
    DSK6713_AIC23_setFreq(hCodec, DSK6713_AIC23_FREQ_44KHZ);

    // interrupt setup
    IRQ_globalDisable();            // Globally disables interrupts
    IRQ_nmiEnable();                // Enables the NMI interrupt
    IRQ_map(IRQ_EVT_RINT1,15);      // Maps an event to a physical interrupt
    IRQ_hook(15, serialPortRcvISR);
    IRQ_enable(IRQ_EVT_RINT1);      // Enables the event
    IRQ_globalEnable();             // Globally enables interrupts
    int x = 0;
    while(1);                        // main loop - do nothing but wait for interrupts
}

interrupt void serialPortRcvISR()
{
    //IRQ_globalDisable();            // Globally disables interrupts
    //IRQ_nmiEnable();                // Enables the NMI interrupt
    union {Uint32 combo; short channel[2];} temp;

    temp.combo = MCBSP_read(DSK6713_AIC23_DATAHANDLE);
    // Note that right channel is in temp.channel[0]
    // Note that left channel is in temp.channel[1]
    MCBSP_write(DSK6713_AIC23_DATAHANDLE,(sine_table[loop]*gain));

    //output_sample(sine_table[loop]*gain);
    loop += frequency;
    loop = loop%sineTableLen;
}
