#include "p33Fxxxx.h"
#include <spi.h>

#define STARTWORD 0xFABC

void spiPinConfig(){
    //remap pins

    TRISB = 0xFFFF;
    TRISBbits.TRISB7 = 0;     // RB6 as output

    RPINR20bits.SDI1R = 8;    // SPI1 data input tie to RP14
    RPINR20bits.SCK1R = 6;    // SPI1 clock input tie to RP6
    RPINR21bits.SS1R = 9;     // SPI1 slave select tie to RP9
    RPOR3bits.RP7R = 0b00111; // RP7 tie to SPI data out
}

void initSPI(void) {

    //IPC2bits.SPI1EIP = 7;// priority for spi, 7 is highest

    /* The following code shows the SPI register configuration for Slave mode */
    SPI1BUF = 0;
    IFS0bits.SPI1IF = 0; // Clear the Interrupt Flag
    IEC0bits.SPI1IE = 0; // Disable the Interrupt
    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0; // Internal Serial Clock is Enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 1; // Communication is word-wide (16 bits)
    SPI1CON1bits.SMP = 0; // Input data is sampled at the middle of data
    // output time
    SPI1CON1bits.CKE = 1; // Serial output data changes on transition
    // from Idle clock state to active clock state
    SPI1CON1bits.CKP = 0; // Idle state for clock is a low level; active
    // state is a high level
    SPI1CON1bits.MSTEN = 0; // Master mode Disabled
    SPI1CON1bits.SSEN = 1;
    SPI1STATbits.SPIROV = 0; // No Receive Overflow has occurred
    
    SPI1STATbits.SPIEN = 1; // Enable SPI module

    // Interrupt Controller Settings
    IFS0bits.SPI1IF = 0; // Clear the Interrupt Flag

    // we won't be using interrupts
    //IEC0bits.SPI1IE = 1; // Enable the Interrupt
}

void spiEnable(){
    SPI1STATbits.SPIEN = 1;
}

void spiDisable(){
    SPI1STATbits.SPIEN = 0;
}

void spiSendWordBlocking(int data){
    while(SPI1STATbits.SPITBF);
    SPI1BUF = data;
}

void spiSendWordArrayBlocking(int* data, int count) {
    int i;
    while (SPI1STATbits.SPITBF);
    SPI1BUF = 0xFFFF;
    while (SPI1STATbits.SPITBF);
    SPI1BUF = STARTWORD;
    while (SPI1STATbits.SPITBF);
    SPI1BUF = count;
    for (i = 0; i < count; i++) {
        while (SPI1STATbits.SPITBF);
        SPI1BUF = data[i];
    }
    while (SPI1STATbits.SPITBF);
    SPI1BUF = 0;
    while (SPI1STATbits.SPITBF);
}

/*
void spiSendWordArrayBlockingMarkMSB(int* data, int count){
    int i;
    for(i=0;i<count;i++){
        while(SPI1STATbits.SPITBF);
        SPI1BUF = data[i] | 0x8000;
    }
    while(SPI1STATbits.SPITBF);
}
*/
