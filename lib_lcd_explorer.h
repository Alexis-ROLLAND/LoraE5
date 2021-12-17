/***************************************************************************
* Nom du fichier:
* Description	: 
*
* Auteur	: 
* Cr�� le	:
* Compilateur	: 
***************************************************************************
* Modifi� le  	:
***************************************************************************/

#include <xc.h>

#ifndef FCY
#define FCY 4000000UL
#endif
#include <libpic30.h>

// Uncomment if RW line is connected
//#define WITH_LCDRW
#ifdef   WITH_LCDRW
#define  LCD_RW         LATDbits.LATD5
#define  TRIS_LCD_RW    TRISDbits.TRISD5   
#endif


// LCD EN
#define LCD_EN          LATDbits.LATD4
#define TRIS_LCD_EN     TRISDbits.TRISD4

//LCD_RS
#define LCD_RS          LATBbits.LATB15
#define TRIS_LCD_RS     TRISBbits.TRISB15

/*
#define LCD_DB0 LATEbits.LATE0
#define LCD_DB1 LATEbits.LATE1
#define LCD_DB2 LATEbits.LATE2
#define LCD_DB3 LATEbits.LATE2
#define LCD_DB4 LATEbits.LATE4
#define LCD_DB5 LATEbits.LATE5
#define LCD_DB6 LATEbits.LATE6
#define LCD_DB7 LATEbits.LATE7
*/

// LCD DATA
#define LCD_PORT        LATE        // LCD data sur LATE[7:0]
#define TRIS_LCD_PORT   TRISE

// Constantes et macros pour l'afficheur LCD
#define		DISPLAY_ON	0x04
#define		CURSOR_ON	0x02
#define		BLINK_ON	0x01

/*******************************************************************************
 fonction   	:	lcd_cursor(x)
 description	: 
 PE		: 
 PS		:
 Mode d'emploi  :
*******************************************************************************/
#define		lcd_cursor(x)			lcd_cmd(((x)&0x7F)|0x80)

/*******************************************************************************
 fonction   	:	lcd_clear()
 description	: 	Effacement écran.
 PE		: 	Aucun
 PS		:	Aucun
 Mode d'emploi  :
*******************************************************************************/
#define 	lcd_clear()				lcd_cmd(0x01)

/*******************************************************************************
 fonction   	:	lcd_putch(x)
 description	: 	Affichage d'un caractère
 PE		: 	Code ASCII du caractère (le plus souvent)
 PS		:	Aucun
 Mode d'emploi  :	Attention aux caractères accentués, etc...
			Voir table de caractères en ROM du LCD pour
			les caractères non standard.
*******************************************************************************/
#define 	lcd_putch(x)			lcd_data(x)

/*******************************************************************************
 fonction   	:	lcd_goto(x)
 description	: 	Positionner le curseur
 PE		: 	Offset par rapport à L1C1
 PS		:	Aucun
 Mode d'emploi  :	
*******************************************************************************/
#define 	lcd_goto(x)				lcd_cmd(0x80+(x))

/*******************************************************************************
 fonction   	:	lcd_cursor_right()
 description	: 	Déplacement curseur à droite.
 PE		: 	Aucun
 PS		:	Aucun
 Mode d'emploi  :
*******************************************************************************/
#define 	lcd_cursor_right()		lcd_cmd(0x14)

/*******************************************************************************
 fonction   	:	lcd_cursor_left()
 description	:	Déplacement curseur à gauche.
 PE		: 	Aucun
 PS		:	Aucun
 Mode d'emploi  :
*******************************************************************************/
#define 	lcd_cursor_left()		lcd_cmd(0x10)

/*******************************************************************************
 fonction   	:  
 description	: 
 PE		: 
 PS		:
 Mode d'emploi  :
*******************************************************************************/
#define 	lcd_display_shift()		lcd_cmd(0x1C)

/*******************************************************************************
 fonction   	:	lcd_home()
 description	: 	Retour curseur en L1C1
 PE		:	Aucun
 PS		:	Aucun
 Mode d'emploi  :
*******************************************************************************/
#define 	lcd_home()				lcd_cmd(0x02)

/*******************************************************************************
 fonction   	:  
 description	: Fonctions "Display" (curseur visible, clignotant...)
 PE		: 
 PS		:
 Mode d'emploi  :
*******************************************************************************/
#define 	lcd_display(x)			lcd_cmd(((x)&0x0F)|0x08)
/*--------------------------------------------------------------------------*/
/* Fonction  : void Initialiser(void)    									*/
/* Description : Configuration ressources                                   */
/* PE : Aucune                                                              */
/* ps : Aucune                                                              */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
void    lcd_init(void);


/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
void	lcd_cmd(unsigned char c);


/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
void	lcd_data(unsigned char c);

/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
/*******************************************************************************
 fonction   	:	void lcd_puts(const char * s)
 description	: 	Affichage d'une chaine de caractères
 PE		: 	Chaine de caractères à afficher.
 PS		:	Aucun
 Mode d'emploi  :	Vérifier que la chaîne est bien terminée par '\0'
*******************************************************************************/
void lcd_puts(const char * s);

/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/





