/*
 * File:   LabC4.c
 * Author: Amit
 *
 * Created on April 11, 2021, 16:17 AM
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "System/system.h"
#include "System/delay.h"
#include "oledDriver/oledC.h"
#include "oledDriver/oledC_colors.h"
#include "oledDriver/oledC_shapes.h"


void __attribute__((__interrupt__)) _T1Interrupt(void)
{
    static bool on = false;
    if (!on)
    {
        oledC_sendCommand(OLEDC_CMD_SET_DISPLAY_MODE_INVERSE, NULL, 0);
        on = true;
    }
    else
    {
        oledC_sendCommand(OLEDC_CMD_SET_DISPLAY_MODE_ON, NULL, 0);
        on = false;
    }
    IFS0bits.T1IF = 0;
}

void User_Initialize(void)
{
    timer_1_init();
    oledC_setBackground(OLEDC_COLOR_WHITE);
    oledC_clearScreen();
}

/*
                Main application
 */
int main(void)
{
    // initialize the system
    SYSTEM_Initialize();
    User_Initialize();

    // Main loop
    while (1)
        ;
    return 1;
}
/**
 End of File
 */