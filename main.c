/*
 * File:   LabC6.c
 * Author: Alon
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "System/system.h"
#include "System/delay.h"
#include "oledDriver/oledC.h"
#include "oledDriver/oledC_colors.h"
#include "oledDriver/oledC_shapes.h"

void oledC_BitBangCommand(unsigned char cmd)
{
    // Write your code here
    // You may use calls to DELAY_microseconds(), defined in delay.c
    // Sometimes Called “Bit-Banging”
    //  We define CS#/DC/MOSI/Clock as GPOs
    //  We use software to:
    // 1.   (A) Lower CS#
    LATCbits.LATC9 = 0;
    //      (B) Set D/C# (Low is Command)
    LATCbits.LATC3 = 0;
    bool bit = 0;
    for (uint8_t i = 0; i < 8; i++)
    // Repeat (2)→(4) 8 times per each byte
    {
        // 2. Wait for one microsecond and Lower Clock
        DELAY_microseconds(1);
        LATBbits.LATB15 = 0;
        // 3. Set Data bit (MSB first)
        bit = (cmd >> (7 - i)) & 0b1;
        LATBbits.LATB14 = bit;
        // 4. Wait for one microsecond and Raise Clock
        DELAY_microseconds(1);
        LATBbits.LATB15 = 1;
    }
    // 5. Raise CS
    LATCbits.LATC9 = 1;
};

void remove_hardware_spi()
{
    // Remove Hardware SPI (see L9 slide #16)
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR7bits.RP14R = 0;                    // RB14->SPI MOSI
    RPOR7bits.RP15R = 0;                    // RB15->SPI Clock
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
}

void oledC_as_slave()
{
    // Set Direction (GPO) of software-SPI signals
    //<Add your code here>
    TRISCbits.TRISC9 = 0;  // Slave Select
    TRISCbits.TRISC3 = 0;  // Data/Command
    TRISBbits.TRISB14 = 0; // MOSI
    TRISBbits.TRISB15 = 0; // Clock
}

void initialize()
{
    SYSTEM_Initialize();
    oledC_setBackground(OLEDC_COLOR_RED);
    remove_hardware_spi();
    oledC_as_slave();
}

int main(void)
{
    // initialize the system
    initialize();

    // Main loop
    while (1)
    {
        oledC_BitBangCommand(OLEDC_CMD_SET_DISPLAY_MODE_INVERSE);
        DELAY_milliseconds(1000);
        oledC_BitBangCommand(OLEDC_CMD_SET_DISPLAY_MODE_ON);
        DELAY_milliseconds(1000);
    }
}
/**
 End of File
 */