/***************************************************************************
* Nom du prg 	:  
* Description	: 
*
* Auteur	: 
* Créé le	:
* Compilateur	: 
***************************************************************************
* Modifié le  	:
***************************************************************************/
#define FCY 4000000UL

#include <libpic30.h>
#include <stdio.h>
#include <stdint.h>

#include "lib_LoRaE5.h"   
#include "lib_lcd_explorer.h"
/*----------------------------------------------------------------------------*/
/*Insérer Ici les bits de configuration pour le µC 							  */
/* -> Copier / Coller depuis le fichier Config_Pic24.txt					  */
/*----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/* PIC24FJ128GA010															*/
/*--------------------------------------------------------------------------*/
#pragma config POSCMOD = HS         // Primary Oscillator Select (EC, XT, HS, NONE)
#pragma config OSCIOFNC = ON        // Primary Oscillator Output Function (ON, OFF)
#pragma config FCKSM = CSDCMD       // Clock Switching and Monitor (CSECME, CSECMD, CSDCMD)
#pragma config FNOSC = PRI          // Oscillator Select (FRC, FRCPLL, PRI, PRIPLL, SOSC, LPRC, FRCDIV)
#pragma config IESO = OFF           // Internal External Switch Over Mode (ON, OFF)
#pragma config WDTPS = PS256        // Watchdog Timer Postscaler (PS1, PS2, PS4, PS8,..., PS32768)
#pragma config FWPSA = PR32         // WDT Prescaler (PR32, PR128)
#pragma config WINDIS = ON          // Watchdog Timer Window (ON, OFF)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable (ON, OFF)
#pragma config ICS = PGx2           // Comm Channel Select (PGx1, PGx2)
#pragma config BKBUG = OFF          // Background Debug (ON, OFF)
#pragma config GWRP = OFF           // General Code Segment Write Protect (ON, OFF)
#pragma config GCP = OFF            // General Code Segment Code Protect (ON, OFF)
#pragma config JTAGEN = OFF         // JTAG Port Enable (ON, OFF)
//----------------------------------------------------------------------------
/* Directives de compilation		*/


/* Macros   */
#define LED3    LATAbits.LATA0

/* Déclarations des variables globales 	*/

/* Programme Principal			*/
int main(void)
{
// Variables locales au main


// Initialisations
// GPIOS
    TRISAbits.TRISA0 = 0;   // LED3 GPIO Output
    LED3 = 0;
    
    // For debug Purposes
    U2MODE = 0x8008;    // High speed mode
    U2STA = 0x0400;
    U2BRG = 104;        // 9600bps @FCY=4MHz

    // Configuration IRQ sur RX
    IFS1bits.U2RXIF = 0;    // sécurité
    IPC7bits.U2RXIP = 4;    // Default priority level
    IEC1bits.U2RXIE = 1;    // Enable UART2 Rx IRQ
    
    lcd_init();
    lcd_puts("LoRa E5 - OK");
    __delay_ms(1000);
    
    
while(1)
    {
    lcd_clear();
    __delay_ms(5000);
    E5_sendVER();
     
     
    }
}					

