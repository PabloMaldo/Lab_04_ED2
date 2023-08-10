//******************************************************************************
/* 
 * File:   
 * Author: Pablo Maldonado
 * Comments:
 * Revision history: 
 */
//******************************************************************************
#include "Interrupciones.h"
//******************************************************************************

//******************************************************************************
//  Funcion para configurar los botones en puerto B
//******************************************************************************
void ioc_init(char pin){
    TRISB |= (1 << pin);    //Estableciendo el pin como input
    // *************************************************************************
    // Habilitando los pull ups
    // *************************************************************************
    OPTION_REGbits.nRBPU = 0; // Enable de los pull ups individuales
    WPUB |= (1 << pin);
    // *************************************************************************
    // Configurar interrupciones
    // *************************************************************************
    INTCONbits.RBIE = 1;    // PORTB change interrupt enable
    INTCONbits.RBIF = 0;    // PORTB change interrupt Flag
    IOCB |= (1 << pin);
    INTCONbits.GIE = 1; // Interrupciones globales
}