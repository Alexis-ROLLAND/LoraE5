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


#include "lib_lcd_explorer.h" // Inclusion du fichier .h "Applicatif" renommé

/* Directives de compilation		*/


/* Déclarations des variables globales 	*/



/*	Implémentation du code */
void    lcd_init(void)
{
    // GPIO
    TRIS_LCD_PORT &= 0xFF00;    // LCD_DATA as output
    LCD_PORT = 0;
    
    #ifdef   WITH_LCDRW
    TRIS_LCD_RW = 0;   
    LCD_RW = 0;
    #endif

    TRIS_LCD_EN = 0;
    LCD_EN = 0;
        
    TRIS_LCD_RS = 0;
    LCD_RS = 0;
    
    // Init du LCD (cf : http://web.alfredstate.edu/faculty/weimandn/lcd/lcd_initialization/lcd_initialization_index.html )
    // step 1
    __delay_ms(100);
    
    // step 2
    lcd_cmd(0x30);
    __delay_ms(10);
    
    // step 3
    lcd_cmd(0x30);
    __delay_us(100);
    
    // step 4
    lcd_cmd(0x30);
    __delay_us(100);
    
    // step 5
    lcd_cmd(0x38);
    __delay_us(55);
    
    // step 6
    lcd_cmd(0x0C);
    __delay_us(55);
    
    // step 7
    lcd_cmd(0x01);
    __delay_ms(3);
    
    // step 8
    lcd_cmd(0x06);
    __delay_us(55);
    
    __delay_ms(100);
}


void	lcd_cmd(unsigned char c)
{
LCD_RS = 0;
__delay_us(1);
LCD_EN = 1;
__delay_us(1);
LCD_PORT = c;
__delay_us(1);
LCD_EN = 0;
__delay_us(20);
}

void	lcd_data(unsigned char c)
{
LCD_RS = 1;
__delay_us(1);
LCD_EN = 1;
__delay_us(1);
LCD_PORT = c;
__delay_us(1);
LCD_EN = 0;
__delay_us(20);

}

void lcd_puts(const char * s)
{
unsigned char	Index = 0;
while(s[Index] != '\0')
	{	
	lcd_data(s[Index]);
	Index++;
    }
}

