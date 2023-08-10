/*
 * File:   VER2.c
 * Author: pablo
 *
 * Created on August 9, 2023, 11:15 AM
 */

//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include <xc.h>
#include "LCD_8b.h"
#include "Interrupciones.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
#define RS RE0
#define EN RE1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
uint8_t temp_hr;
uint8_t temp_min;
uint8_t temp_sec;
uint8_t temp_day;
uint8_t temp_mon;
uint8_t temp_year;
uint8_t temp_slave;
uint8_t d_hr;
uint8_t u_hr;
uint8_t d_min;
uint8_t u_min;
uint8_t d_sec;
uint8_t u_sec;
uint8_t d_day;
uint8_t u_day;
uint8_t d_mon;
uint8_t u_mon;
uint8_t d_year;
uint8_t u_year;
uint8_t c_slave;
uint8_t d_slave;
uint8_t u_slave;

uint8_t writeMode = 0;
uint8_t selector = 0;
uint8_t multiCont = 0;
uint8_t unidades;
uint8_t decenas;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
uint8_t readRTC(uint8_t r_addrs);
uint8_t int2char(uint8_t num);
void writeRTC(uint8_t r_addrs, uint8_t data);
//******************************************************************************
// Vector de Interrupciones
//******************************************************************************
void __interrupt() isr(void){
    if(INTCONbits.RBIF)
    {
        if (PORTBbits.RB0 == 0){
            writeMode = 1; //Se habilita el modo de escritura
            PORTBbits.RB7 = 1;
        }
        INTCONbits.RBIF = 0;
    } 
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Clear();
    while(1){
        // CONFIGURANDO PARAMETROS RTC
        if (writeMode == 1){
            if (PORTBbits.RB1 == 0){// Cambiando el valor del selector
                __delay_ms(200);    //Antirebote
                selector++;
                PORTA = selector;
            }
            if (PORTBbits.RB2 == 0){//Boton Aumentar
                __delay_ms(200);    //Antirebote
                multiCont++;
                PORTA = multiCont;
            }
            if (PORTBbits.RB3 == 0){//Boton decrementar
                __delay_ms(200);    //Antirebote
                multiCont--;
                PORTA = multiCont;
            }
            //******************************************************************
            if (selector == 1){//modificamos los segundos
                if (multiCont > 59){
                    multiCont = 59;
                }
                //CONFIGURANDO SEGUNDOS EN RTC
                unidades = multiCont % 10;
                decenas = multiCont / 10;
                uint8_t dato = (decenas << 4)| unidades;
                writeRTC(0x00, dato);                
            }
            if (selector == 2){//modificamos los minutos
                if (multiCont > 59){
                    multiCont = 59;
                }
                //CONFIGURANDO MINUTOS EN RTC
                unidades = multiCont % 10;
                decenas = multiCont / 10;
                uint8_t dato = (decenas << 4)| unidades;
                writeRTC(0x01, dato);
            }
            if (selector == 3){//modificamos las horas
                if (multiCont > 23){
                    multiCont = 23;
                }
                //CONFIGURANDO HORAS EN RTC
                unidades = multiCont % 10;
                decenas = multiCont / 10;
                uint8_t dato = (decenas << 4)| unidades;
                writeRTC(0x02, dato);
            }
            if (selector == 4){//modificamos el dia
                if (multiCont > 31){
                    multiCont = 31;
                }
                //CONFIGURANDO DIAS EN RTC
                unidades = multiCont % 10;
                decenas = multiCont / 10;
                uint8_t dato = (decenas << 4)| unidades;
                writeRTC(0x04, dato);                
            }
            if (selector == 5){//modificamos el mes
                if (multiCont > 12){
                    multiCont = 12;
                }
                //CONFIGURANDO MESES EN RTC
                unidades = multiCont % 10;
                decenas = multiCont / 10;
                uint8_t dato = (decenas << 4)| unidades;
                writeRTC(0x05, dato);
            }
            if (selector == 6){//modificamos el año
                if (multiCont > 99){
                    multiCont = 99;
                }
                //CONFIGURANDO AÑOS EN RTC
                unidades = multiCont % 10;
                decenas = multiCont / 10;
                uint8_t dato = (decenas << 4)| unidades;
                writeRTC(0x06, dato);
            }
            if (selector == 7){
                selector = 0;
                writeMode = 0;
                multiCont = 0;
            }
            
        }
        
            //LEYENDO HORA, MINUTOS Y SEGUNDOS
            temp_hr = readRTC(0x02); //Leyendo las horas (registro 02h)
            temp_min = readRTC(0x01); //Leyendo los minutos (registro 01h)
            temp_sec = readRTC(0x0);   //Leyendo los segundos (registro 00h)
            //Separando unidades y decenas
            u_hr = temp_hr & 0x0F;  //Unidades hrs
            d_hr = (temp_hr >> 4) & 0x0F;  //Decenas hrs
            u_min = temp_min & 0x0F;  //Unidades min
            d_min = (temp_min >> 4) & 0x0F;  //Decenas min
            u_sec = temp_sec & 0x0F;  //Unidades sec
            d_sec = (temp_sec >> 4) & 0x0F;  //Decenas sec
            char hora[] = {d_hr+48, u_hr+48, ':', d_min+48, u_min+48, ':', d_sec+48, u_sec+48, '\0'};
            
            //LEYENDO DIA, MES Y AÑO
            temp_day = readRTC(0x04); //Leyendo el dia (registro 04h)
            temp_mon = readRTC(0x05); //Leyendo el mes (registro 05h)
            temp_year = readRTC(0x06); //Leyendo el año (registro 06h)
            //Separando unidades y decenas
            d_day = (temp_day >> 4) & 0x0F;  //Decenas dia
            u_day = temp_day & 0x0F;  //Unidades dia
            d_mon = (temp_mon >> 4) & 0x0F; //Decenas mes
            u_mon = temp_mon & 0x0F;    //Unidades mes
            d_year = (temp_year >> 4) & 0x0F;   //Decenas año
            u_year = temp_year & 0x0F;  //Unidades año
            char fecha[] = {d_day+48, u_day+48, '/', d_mon+48, u_mon+48, '/', '2', '0', d_year+48, u_year+48, '\0'};
            
            //LEYENDO DATA DEL ESCLAVO
            I2C_Master_Start();
            I2C_Master_Write(0x51); //Leer los datos del Slave
            temp_slave = I2C_Master_Read(0); // ACK 
            I2C_Master_Stop();
            __delay_ms(10);
            //Separando unidades, decenas y centenas
            c_slave = temp_slave / 100;
            d_slave = (temp_slave / 10) % 10;
            u_slave = temp_slave % 10;
            char esclavo[] = {c_slave+48, d_slave+48, u_slave+48, '\0'};
            
            //MOSTRANDO EN LCD
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String(hora);
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String(fecha);
            Lcd_Set_Cursor(1,14);
            Lcd_Write_String("S1:");
            Lcd_Set_Cursor(2,14);
            Lcd_Write_String(esclavo);
        
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    
    TRISE = 0;
    PORTE = 0;
    
    TRISD = 0x00;
    PORTD = 0x00;
    
    TRISA = 0;
    PORTA = 0;
    
    // Configuracion oscilador interno
    OSCCONbits.IRCF = 0b111; // 8MHz
    OSCCONbits.SCS = 1;
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C (Standar Mode)
    
    Lcd_Init();                     //Inicializar el la LCD
    
    ioc_init(3);                    //Decrementar
    ioc_init(2);                    //Aumentar
    ioc_init(1);                    //Selector
    ioc_init(0);                    //modo escritura on/off
}
//******************************************************************************
// Funcion para leer del RTC
//******************************************************************************
uint8_t readRTC(uint8_t r_addrs){
    uint8_t temp;
    I2C_Master_Start();
    I2C_Master_Write(0xD0); //Slave Address en modo write
    I2C_Master_Write(r_addrs); //Acceder a la dirección del registro de interes ej. Hr, Min, Sec...
    I2C_Master_Stop();
    __delay_ms(10);
       
    I2C_Master_Start();
    I2C_Master_Write(0xD1); //Leer los datos
    temp = I2C_Master_Read(0); // ACK 
    I2C_Master_Stop();
    __delay_ms(10);
    return temp;
}
//******************************************************************************
// Funcion para escribir al RTC
//******************************************************************************
void writeRTC(uint8_t r_addrs, uint8_t data){
    I2C_Master_Start();
    I2C_Master_Write(0xD0); //Slave Address en modo write
    I2C_Master_Write(r_addrs); //Acceder a la dirección del registro de interes ej. Hr, Min, Sec...
    I2C_Master_Write(data);    //Dato a escribir
    I2C_Master_Stop();
    __delay_ms(10);
}

uint8_t int2char(uint8_t num){
    uint8_t val;
    switch (num) {
            case 0:
                val = 48; //0
                break;
            case 1:
                val = 49; //1
                break;
            case 2:
                val = 50; //2
                break;
            case 3:
                val = 51; //3
                break;
            case 4:
                val = 52; //4
                break;
            case 5:
                val = 53; //5
                break;
            case 6:
                val = 54; //6
                break;
            case 7:
                val = 55; //7
                break;
            case 8:
                val = 56; //8
                break;
            case 9:
                val = 57; //9
                break;
    }
    return val;
}