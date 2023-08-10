//******************************************************************************
/* 
 * File:   
 * Author: Pablo Maldonado
 * Comments:
 * Revision history: 
 */
//******************************************************************************
#include "adc.h"
#define _XTAL_FREQ 4000000
//******************************************************************************

//******************************************************************************
//  Funcion para inicializar los canales
//******************************************************************************
void adc_init(int channel){
    switch (channel) {
        case 0:
            ANSELbits.ANS0 = 1;
            TRISAbits.TRISA0 = 1;
            break;
        case 1:
            ANSELbits.ANS1 = 1;
            TRISAbits.TRISA1 = 1;
            break;
        case 2:
            ANSELbits.ANS2 = 1;
            TRISAbits.TRISA2 = 1;
            break;
        case 3:
            ANSELbits.ANS3 = 1;
            TRISAbits.TRISA3 = 1;
            break;
        case 4:
            ANSELbits.ANS4 = 1;
            TRISAbits.TRISA5 = 1;
            break;
        case 5:
            ANSELbits.ANS5 = 1;
            TRISEbits.TRISE0 = 1;
            break;
        case 6:
            ANSELbits.ANS6 = 1;
            TRISEbits.TRISE1 = 1;
            break;
        case 7:
            ANSELbits.ANS7 = 1;
            TRISEbits.TRISE2 = 1;
            break;
        case 8:
            ANSELHbits.ANS8 = 1;
            TRISBbits.TRISB2 = 1;
            break;
        case 9:
            ANSELHbits.ANS9 = 1;
            TRISBbits.TRISB3 = 1;
            break;
        case 10:
            ANSELHbits.ANS10 = 1;
            TRISBbits.TRISB1 = 1;
            break;
        case 11:
            ANSELHbits.ANS11 = 1;
            TRISBbits.TRISB4 = 1;
            break;
        case 12:
            ANSELHbits.ANS12 = 1;
            TRISBbits.TRISB0 = 1;
            break;
        case 13:
            ANSELHbits.ANS13 = 1;
            TRISBbits.TRISB5 = 1;
            break;
        
    }
          
    ADCON0bits.CHS = channel;
    ADCON0bits.ADCS = 0b01; // FOSC/32
    __delay_ms(1);
    ADCON1bits.ADFM = 0; //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0; //Voltaje de ref. a GND
    ADCON1bits.VCFG1 = 0; //Voltaje de ref. a 5v
    ADCON0bits.ADON = 1; // ADC is enabled
    
    PIE1bits.ADIE = 1;  // Enable ADC interrupt
    PIR1bits.ADIF = 0; //Limpiar la bandera de la interrupcion del ADC
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1; // Interrupciones globales
}
//******************************************************************************
//******************************************************************************

//******************************************************************************
//  Funcion para leer los canales
//******************************************************************************
int adc_read(void){
    return ADRESH;
}
//******************************************************************************
//******************************************************************************

//******************************************************************************
//  Funcion para cambiar los canales
//******************************************************************************
void adc_change_channel(int channel){
    
        switch (channel) {
            case 0:
                ADCON0bits.CHS = 0b0000;
                break;
            case 1:
                ADCON0bits.CHS = 0b0001;
                break;
            case 2:
                ADCON0bits.CHS = 0b0010;
                break;
            case 3:
                ADCON0bits.CHS = 0b0011;
                break;
            case 4:
                ADCON0bits.CHS = 0b0100;
                break;
            case 5:
                ADCON0bits.CHS = 0b0101;
                break;
            case 6:
                ADCON0bits.CHS = 0b0110;
                break;
            case 7:
                ADCON0bits.CHS = 0b0111;
                break;
            case 8:
                ADCON0bits.CHS = 0b1000;
                break;
            case 9:
                ADCON0bits.CHS = 0b1001;
                break;
            case 10:
                ADCON0bits.CHS = 0b1010;
                break;
            case 11:
                ADCON0bits.CHS = 0b1011;
                break;
            case 12:
                ADCON0bits.CHS = 0b1100;
                break;
            case 13:
                ADCON0bits.CHS = 0b1101;
                break;
        
    }
}
//******************************************************************************
//******************************************************************************

//******************************************************************************
//  Funcion para pedir los canales
//******************************************************************************
int adc_get_channel(void){
    return ADCON0bits.CHS;  // regresa el canal en el que nos encontramos
}
//******************************************************************************
//******************************************************************************