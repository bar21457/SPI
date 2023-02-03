/*
 * File:   setupINTOSC.c
 * Author: byron
 *
 * Created on January 26, 2023, 11:14 PM
 */

#include <xc.h>
#include "configINTOSC.h"

void setupINTOSC(FOSC F) {
    
    OSCCONbits.IRCF = F ;
    OSCCONbits.SCS = 1;
}

