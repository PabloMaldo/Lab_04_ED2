//******************************************************************************
/* 
 * File:   
 * Author: Pablo Maldonado
 * Comments:
 * Revision history: 
 */
//******************************************************************************

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//******************************************************************************
//  Funcion para inicializar los canales
//******************************************************************************
void adc_init(int channel);

//******************************************************************************
//  Funcion para leer los canales
//******************************************************************************
int adc_read(void);

//******************************************************************************
//  Funcion para cambiar los canales
//******************************************************************************
void adc_change_channel(int channel);

//******************************************************************************
//  Funcion para pedir los canales
//******************************************************************************
int adc_get_channel(void);

#endif	/* ADC_H */
