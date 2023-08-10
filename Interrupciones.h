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
#ifndef INTERRUPCIONES_H
#define INTERRUPCIONES_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//******************************************************************************
//  Funcion para configurar los botones en puerto B
//******************************************************************************
void ioc_init(char pin);

#endif	/* INTERRUPCIONES_H */

