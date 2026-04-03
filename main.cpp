/* NOTAS:
- 
*/

#include "mbed.h"

#define A_INPUT 0
#define A_OUTPUT 1

typedef struct {  
    int spd;
    int pin1, pin2;
    char p1, p2;
} DCMotor;

void meu_pinMode(char porta, int pin, int modo);
void meu_analogWrite(char porta, int pin, int valor);
void Pinout(DCMotor *m, char porta1, int in1, char porta2, int in2);
void Speed(DCMotor *m, int in);
void Forward(DCMotor *m);
void Backward(DCMotor *m);
void Stop(DCMotor *m);
void delayMs(int n);
   
DCMotor Motor1, Motor2;

int main(void) {
    SIM->SCGC6 |= 0x1000000; // Habilita o clock para o módulo TPM0
    SIM->SCGC6 |= 0x2000000; // Habilita o clock para o módulo TPM1
    SIM->SCGC6 |= 0x4000000; // Habilita o clock para o módulo TPM2
    SIM->SOPT2 |= 0x1000000; // Seleciona o clock MCGFLLCLK (41.94 MHz)

    Pinout(&Motor1, 'A', 4, 'A', 5);
    Pinout(&Motor2, 'A', 12, 'A', 13);

    while (1) {
        Speed(&Motor1, 200); // A velocidade do motor pode variar de 0 a 255, onde 255 é a velocidade máxima.
        Speed(&Motor2, 200);
  
        Forward(&Motor1); // Comando para o motor ir para frente
        Forward(&Motor2);
        delayMs(1000);

        Backward(&Motor1); // Comando para o motor ir para trás
        Backward(&Motor2);
        delayMs(1000);

        Stop(&Motor1); // Comando para o motor parar
        Stop(&Motor2);
        delayMs(500);
    }
}

void meu_pinMode(char porta, int pin, int modo) {
    if (porta == 'A') {
        SIM->SCGC5 |= 0x200;
        if (modo == A_OUTPUT) {
            PORTA->PCR[pin] |= 0x300;
        }
    }
    if (porta == 'B') {
        SIM->SCGC5 |= 0x400;
        if (modo == A_OUTPUT) {
            PORTB->PCR[pin] |= 0x300;
        }
    }
    if (porta == 'C') {
        SIM->SCGC5 |= 0x800;
        if (modo == A_OUTPUT) {
            PORTC->PCR[pin] |= 0x300;
        } 
    }
    if (porta == 'D') {
        SIM->SCGC5 |= 0x1000;
        if (modo == A_OUTPUT) {
            PORTD->PCR[pin] |= 0x300;
        }
    }
    if (porta == 'E') {
        SIM->SCGC5 |= 0x2000;
        if (modo == A_OUTPUT) {
            PORTE->PCR[pin] |= 0x300;
        }
    }
}

void meu_analogWrite(char porta, int pin, int valor) {
    if (porta == 'A' && pin == 4) {
        TPM0->SC = 0; // Desabilita o timer enquanto as configurações estão sendo mudadas
        TPM0->CONTROLS[1].CnSC = 0x20 | 0x4; 
        TPM0->MOD = 20970; // Atribui o valor a TPM0->MOD para a frequência desejada (1kHz = 41.94 * 10^6 / 20970 * 2)
        TPM0->CONTROLS[1].CnV = (valor / 255) * TPM0->MOD; // Atribui o valor a TPM0->CnV para o duty cycle desejado
        TPM0->SC |= 0x18; // Habilita o clock no modo center-aligned PWM
    }

    else if (porta == 'A' && pin == 5) {
        TPM0->SC = 0; // Desabilita o timer enquanto as configurações estão sendo mudadas
        TPM0->CONTROLS[2].CnSC = 0x20 | 0x4; 
        TPM0->MOD = 20970; // Atribui o valor a TPM0->MOD para a frequência desejada (1kHz = 41.94 * 10^6 / 20970 * 2)
        TPM0->CONTROLS[2].CnV = (valor / 255) * TPM0->MOD; // Atribui o valor a TPM0->CnV para o duty cycle desejado
        TPM0->SC |= 0x18; // Habilita o clock no modo center-aligned PWM
    }

    else if (porta == 'A' && pin == 12) {
        TPM1->SC = 0; // Desabilita o timer enquanto as configurações estão sendo mudadas
        TPM1->CONTROLS[0].CnSC = 0x20 | 0x4; 
        TPM1->MOD = 20970; // Atribui o valor a TPM0->MOD para a frequência desejada (1kHz = 41.94 * 10^6 / 20970 * 2)
        TPM1->CONTROLS[0].CnV = (valor / 255) * TPM0->MOD; // Atribui o valor a TPM0->CnV para o duty cycle desejado
        TPM1->SC |= 0x18; // Habilita o clock no modo center-aligned PWM
    }

    else if (porta == 'A' && pin == 13) {
        TPM1->SC = 0; // Desabilita o timer enquanto as configurações estão sendo mudadas
        TPM1->CONTROLS[1].CnSC = 0x20 | 0x4; 
        TPM1->MOD = 20970; // Atribui o valor a TPM0->MOD para a frequência desejada (1kHz = 41.94 * 10^6 / 20970 * 2)
        TPM1->CONTROLS[1].CnV = (valor / 255) * TPM0->MOD; // Atribui o valor a TPM0->CnV para o duty cycle desejado
        TPM1->SC |= 0x18; // Habilita o clock no modo center-aligned PWM
    }
}

void Pinout(DCMotor *m, char porta1, int in1, char porta2, int in2) { 
    m->p1 = porta1;
    m->p2 = porta2;
    m->pin1 = in1;
    m->pin2 = in2;
    meu_pinMode(m->p1, m->pin1, A_OUTPUT);
    meu_pinMode(m->p2, m->pin2, A_OUTPUT);
}

void Speed(DCMotor *m, int in) {
    m->spd = in;
}

void Forward(DCMotor *m) { 
    meu_analogWrite(m->p1, m->pin1, m->spd);
    meu_analogWrite(m->p2, m->pin2, 0);
}  

void Backward(DCMotor *m) {
    meu_analogWrite(m->p1, m->pin1, 0);
    meu_analogWrite(m->p2, m->pin2, m->spd);
}

void Stop(DCMotor *m) { 
    meu_analogWrite(m->p1, m->pin1, 0);
    meu_analogWrite(m->p2, m->pin2, 0);
}

void delayMs(int n) {
    SysTick->LOAD = 50000;
    SysTick->CTRL = 5;
    for (int i = 0; i < n; i++) {
        while ((SysTick->CTRL & 0x10000) == 0) {}
    }
}
