#include "mbed.h"

int main(void) {
    SIM->SCGC5 |= 0x400; // Enable the clock to GPIO pin
    PORTB->PCR[18] |= 0x100;
    PTB->PDDR |= 0x40000;
    PTB->PCOR = 0x40000;
    PORTB->PCR[19] |= 0x300; // Assign the TPM2 signals the pin

    SIM->SCGC6 |= 0x4000000; // Enable clock to TPM2 module in SIM_SCGC6
    SIM->SOPT2 |= 0x1000000; // Select counter clock MCGFLLCLK
    TPM2->SC = 0; // Disable timer while the configuration is being modified
    int a = 1;
    TPM2->CONTROLS[a].CnSC = 0x20 | 0x8; // Set the mode for edge-aligned PWM
    TPM2->MOD = 43702; // Load the value into TPM2_MOD to set frequency = 60kHz
    TPM2->CONTROLS[a].CnV = 0; // Load the value into TPM2_CnV to set the desired duty cycle
    TPM2->SC |= 0x0C; // Enable timer with prescalar = 128
    while(1) {}
}
