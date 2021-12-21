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

const uint8_t   E5_AT_RESP[]="+AT: OK";
const uint8_t   E5_JOIN_DONE_RESP[]="+JOIN: Done";
const uint8_t   E5_MSG_DONE_RESP[]="+MSG: Done";

/* Déclarations des variables globales 	*/
uint8_t CarRec;
uint8_t RxBuffer[BUFFER_SIZE];
uint8_t TxBuffer[BUFFER_SIZE];
uint8_t RxPtr=0;
RxStatus_t  RxStatus = WAIT_SOL;

// Time out variables
uint8_t TargetTimeOut;
uint8_t CurrNbTimoutTics;
TimeOutStatus_t  TimeOutStatus;

uint8_t RxFrame[BUFFER_SIZE];


/*	Implémentation du code */
//------------------------------------------------------------------------------
e5_error_t  E5_CheckConnection(void)
{
    if (TimeOutStatus != TIME_OUT_STOPPED) return E5_ERROR;
    sprintf(TxBuffer,"AT\n");
    
    E5_ll_send(TxBuffer);
    E5_startTimeOut(1);
    
    while( (TimeOutStatus != TIME_OUT_EXPIRED) && (RxStatus != EOL_RECEIVED));
    
    if (TimeOutStatus == TIME_OUT_EXPIRED){
        E5_ClearTimeOutStatus();    // Stop time out timer & clear time out status (no longer needed)
        return E5_TIME_OUT;
        }
        
    E5_ClearTimeOutStatus();    // Stop time out timer & clear time out status
    E5_trim(RxFrame,RxBuffer);
    RxStatus = WAIT_SOL;
    
    if (strcmp(RxFrame,E5_AT_RESP) != 0) return E5_ERROR;
    
    return E5_OK;
}
//------------------------------------------------------------------------------
e5_error_t  E5_Join(bool_t ForceJoin)
{
    if (TimeOutStatus != TIME_OUT_STOPPED) return E5_ERROR;
    
    if (ForceJoin == false) sprintf(TxBuffer,"AT+JOIN\n");
    else sprintf(TxBuffer,"AT+JOIN=FORCE\n");
    
    E5_ll_send(TxBuffer);
    E5_startTimeOut(20);
    
    do{
        while( (TimeOutStatus != TIME_OUT_EXPIRED) && (RxStatus != EOL_RECEIVED));
    
        if (TimeOutStatus == TIME_OUT_EXPIRED){
            E5_ClearTimeOutStatus();    // Stop time out timer & clear time out status (no longer needed)
            return E5_TIME_OUT;
            }
        E5_ReStartTimeOut();
        
        E5_trim(RxFrame,RxBuffer);
        RxStatus = WAIT_SOL;
        
    } while (strcmp(RxFrame,E5_JOIN_DONE_RESP) != 0);                                   
    E5_ClearTimeOutStatus();    // Stop time out timer & clear time out status
    
    return E5_OK;
}
//------------------------------------------------------------------------------
e5_error_t  E5_SendStrMsg(uint8_t *pMsg)
{
    if (TimeOutStatus != TIME_OUT_STOPPED) return E5_ERROR;
    
    sprintf(TxBuffer,"AT+MSG=\"%s\"\n",pMsg);
    
    E5_ll_send(TxBuffer);
    E5_startTimeOut(20);
    
    do{
        while( (TimeOutStatus != TIME_OUT_EXPIRED) && (RxStatus != EOL_RECEIVED));
    
        if (TimeOutStatus == TIME_OUT_EXPIRED){
            E5_ClearTimeOutStatus();    // Stop time out timer & clear time out status (no longer needed)
            return E5_TIME_OUT;
            }
        E5_ReStartTimeOut();
        
        E5_trim(RxFrame,RxBuffer);
        RxStatus = WAIT_SOL;
        
    } while (strcmp(RxFrame,E5_MSG_DONE_RESP) != 0);                                   
    E5_ClearTimeOutStatus();    // Stop time out timer & clear time out status
    
    
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
void    E5_trim(uint8_t *pDest, uint8_t *pSource)
{
    while (*pSource != '\0')
    {
        if ((*pSource != 0x0A) && (*pSource != 0x0D))
        {
            *pDest = *pSource;
            pDest++;
        }
        pSource++;
    }
    //pDest++;
    *pDest = '\0';
}
//------------------------------------------------------------------------------
void    E5_startTimeOut(uint8_t Nb500ms)
{
    if (Nb500ms < 1) return;
    
    TargetTimeOut = Nb500ms;
    CurrNbTimoutTics = 0;  
    TimeOutStatus = TIME_OUT_RUNNING;
    IEC0bits.T1IE = 1; // Enable T1 IRQ
    T1CONbits.TON = 1; // Start Timer
}
//------------------------------------------------------------------------------
TimeOutStatus_t E5_getTimeOutStatus(void)
{
    return TimeOutStatus;
}
//------------------------------------------------------------------------------
void    E5_ClearTimeOutStatus(void)
{
    T1CONbits.TON = 0;  // Stop Timer
    IEC0bits.T1IE = 0;  // Disable T1 IRQ
    TimeOutStatus = TIME_OUT_STOPPED;
}
//------------------------------------------------------------------------------
void    E5_ReStartTimeOut(void)
{
    if (TimeOutStatus == TIME_OUT_RUNNING) CurrNbTimoutTics = 0;
}
//------------------------------------------------------------------------------
void _ISR __attribute__((no_auto_psv))  _U2RXInterrupt(void)
{   
    CarRec = U2RXREG;
    switch(RxStatus)
    {
        case WAIT_SOL: 
            if (CarRec == E5_SOL){
                CurrNbTimoutTics=0;     // Extends time out (just in case...)
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
//------------------------------------------------------------------------------
void _ISR __attribute__((no_auto_psv))  _T1Interrupt(void)
{
    LATAbits.LATA0 = ~LATAbits.LATA0;   // Debug purposes
    CurrNbTimoutTics++;
    if (CurrNbTimoutTics >= TargetTimeOut)
    {
        TimeOutStatus = TIME_OUT_EXPIRED;
        T1CONbits.TON = 0;  // Stop Timer
        IEC0bits.T1IE = 0;  // Disable T1 IRQ
    }
    IFS0bits.T1IF = 0;
}
//------------------------------------------------------------------------------
