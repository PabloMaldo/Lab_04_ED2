#include "LCD_8b.h"

void Lcd_Port(char bits) {
    PORTD = bits;   //Recibe los comandos de forma binaria
}

void Lcd_Cmd(char bits) {
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    Lcd_Port(bits);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0 // Datos/comandos se están transmitiendo al LCD
}

void Lcd_Clear(void) {
    Lcd_Cmd(0);    // Null?
    Lcd_Cmd(1);    // Borra la pantalla y coloca el cursor en la pos. inicial
}

void Lcd_Set_Cursor(char f, char c) {
    char temp;
    if (f == 1) {   //Primera linea del display
        temp = 0x80 + c - 1; //(0b1000 0000) + (DDRAM address) - 1
        Lcd_Cmd(temp);
    } else if (f == 2) {    //Segunda linea del display
        temp = 0xC0 + c - 1;    //(0b1100 0000) + (DDRAM address) - 1
        Lcd_Cmd(temp);
    }
}

void Lcd_Init(void) {
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x30);  // (Set 8-bit mode) initialization sequence
    __delay_ms(5);
    Lcd_Cmd(0x30);
    __delay_ms(11);
    Lcd_Cmd(0x30);  // asegurar que la pantalla esté correctamente configurada en el modo de ocho bits.
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x3C); // Condigura la LCD con pantalla de 2 líneas y tamaño de fuente extendido
    Lcd_Cmd(0x0C); // Enciende pantall y muestra el cursor sin parpadear
    Lcd_Cmd(0x01); // Borra la pantalla y coloca el cursor en la pos. inicial
    Lcd_Cmd(0x06); // establece el cursor para que se mueva hacia adelante automáticamente después de escribir un carácter y también desplaza la pantalla hacia la derecha.
}

void Lcd_Write_Char(char caracter) {
    RS = 1; // => RS = 1 // interpretados como datos
    Lcd_Port(caracter); //Data transfer (acaba el caracter)
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd_Write_String(char *caracter) {
    int i;
    for (i = 0; caracter[i] != '\0'; i++)
        Lcd_Write_Char(caracter[i]);
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}
