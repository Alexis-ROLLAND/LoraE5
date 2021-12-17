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


#include "lib_LoRaE5.h"

/* Directives de compilation		*/


/* Déclarations des variables globales 	*/
uint8_t CarRec;
uint8_t RxBuffer[BUFFER_SIZE];
uint8_t TxBuffer[BUFFER_SIZE];
uint8_t RxPtr=0;
RxStatus_t  RxStatus = WAIT_SOL;


/*	Implémentation du code */
//------------------------------------------------------------------------------
e5_error_t  E5_test_AT(void)
{
    sprintf(TxBuffer,"AT\n");
    E5_ll_send(TxBuffer);
    return E5_OK;
}

//------------------------------------------------------------------------------
e5_error_t  E5_sendVER(void)
{
    sprintf(TxBuffer,"AT+VER\n");
    E5_ll_send(TxBuffer);
    return E5_OK;
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void    E5_ll_send(uint8_t *pstr)
{
    while (*pstr != '\0')
    {
        while (U2STAbits.UTXBF); // wait while Tx buffer full
        U2TXREG = *pstr;
        pstr++;
    }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void _ISR __attribute__((no_auto_psv))  _U2RXInterrupt(void)
{   
    CarRec = U2RXREG;
    switch(RxStatus)
    {
        case WAIT_SOL: 
            if (CarRec == E5_SOL){
                RxPtr=0;
                RxBuffer[RxPtr] = CarRec;
                RxStatus = RX_ON;
            }
            break;
        case RX_ON:
            if (CarRec != E5_EOL){
                RxPtr++;
                RxBuffer[RxPtr] = CarRec;
            }
            else{
                RxPtr++;
                RxBuffer[RxPtr] = '\0';
                RxStatus = EOL_RECEIVED;
            }
            break;
        case EOL_RECEIVED: break;
    }
    
    
	IFS1bits.U2RXIF = 0;    // RxFlag
}

