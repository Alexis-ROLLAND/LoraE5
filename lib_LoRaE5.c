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

const uint8_t   E5_AT_RESP[]="AT: OK";
const uint8_t   E5_JOIN_DONE_RESP[]="JOIN: Done";
const uint8_t   E5_MSG_DONE_RESP[]="MSG: Done";
const uint8_t   E5_MSGHEX_DONE_RESP[]="MSGHEX: Done";
const uint8_t   E5_RESET_RESP[]="RESET: OK";
/* Déclarations des variables globales 	*/
uint8_t CarRec;
uint8_t RxBuffer[BUFFER_SIZE];
uint8_t TxBuffer[BUFFER_SIZE];
uint8_t RxPtr=0;
RxStatus_t  RxStatus = WAIT_SOL;

uint8_t FrameId[10];

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
    sprintf((char*)TxBuffer,(char*)"AT\n");
    
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
    
    if (strcmp((char*)RxFrame,(char*)E5_AT_RESP) != 0) return E5_ERROR;
    
    return E5_OK;
}
//------------------------------------------------------------------------------
e5_error_t  E5_Reset(void)
{
    if (TimeOutStatus != TIME_OUT_STOPPED) return E5_ERROR;
    sprintf((char*)TxBuffer,(char*)"AT+RESET\n");
    
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
    
    if (strcmp((char*)RxFrame,(char*)E5_RESET_RESP) != 0) return E5_ERROR;
    return E5_OK;
}
//------------------------------------------------------------------------------
e5_error_t  E5_Join(bool_t ForceJoin)
{
    if (TimeOutStatus != TIME_OUT_STOPPED) return E5_ERROR;
    
    if (ForceJoin == false) sprintf((char*)TxBuffer,(char*)"AT+JOIN\n");
    else sprintf((char*)TxBuffer,(char*)"AT+JOIN=FORCE\n");
    
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
        
    } while (strcmp((char*)RxFrame,(char*)E5_JOIN_DONE_RESP) != 0);                                   
    E5_ClearTimeOutStatus();    // Stop time out timer & clear time out status
    
    return E5_OK;
}
//------------------------------------------------------------------------------
e5_error_t  E5_SendStrMsg(uint8_t *pMsg)
{
    if (TimeOutStatus != TIME_OUT_STOPPED) return E5_ERROR;
    
    sprintf((char*)TxBuffer,"AT+MSG=\"%s\"\n",pMsg);
    
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
        
    } while (strcmp((char*)RxFrame,(char*)E5_MSG_DONE_RESP) != 0);                                   
    E5_ClearTimeOutStatus();    // Stop time out timer & clear time out status
    
    
    return E5_OK;
}

//------------------------------------------------------------------------------
e5_error_t  E5_SendByteMsg(uint8_t *pByteBuffer, uint8_t size)
{
    uint8_t strByte[3];
    uint8_t i;
    
    if (TimeOutStatus != TIME_OUT_STOPPED) return E5_ERROR;
    
    sprintf((char*)TxBuffer,"AT+MSGHEX=\"");
    for (i=0;i<size;i++)
    {
        sprintf((char*)strByte,"%02x",pByteBuffer[i]);
        strcat((char*)TxBuffer,(char*)strByte);
    }
    strcat(TxBuffer,"\"\n");
    
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
        
    } while (strcmp((char*)RxFrame,(char*)E5_MSGHEX_DONE_RESP) != 0);                                   
    E5_ClearTimeOutStatus();    // Stop time out timer & clear time out status
    
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
resp_id_t    E5_trim(uint8_t *pDest, uint8_t *pSource)
{
    bool_t FrameIdCompleted = false;
    uint8_t FidPtr = 0;
    resp_id_t FrameType;
    
    if (pSource[0] != '+') return RESP_UNKNOWN; 
    pSource++;
    while (*pSource != '\0')
    {
        if ((*pSource != 0x0A) && (*pSource != 0x0D))
        {
            if (FrameIdCompleted==false) 
            {
                if (*pSource != ':')
                {
                    FrameId[FidPtr] = *pSource;
                    FidPtr++;
                }
                else
                {
                    FrameId[FidPtr] = '\0';
                    FrameIdCompleted=true;
                }
            }
            *pDest = *pSource;
            pDest++;
        }
        pSource++;
    }
    
    *pDest = '\0';
    
    if (strcmp((char*)FrameId,(char*)"AT")==0) FrameType=RESP_AT;
    else if ((strcmp((char*)FrameId,(char*)"VER")==0)) FrameType=RESP_VER;
    else if ((strcmp((char*)FrameId,(char*)"ID")==0)) FrameType=RESP_ID;
    else if ((strcmp((char*)FrameId,(char*)"RESET")==0)) FrameType=RESP_RESET;
    else if ((strcmp((char*)FrameId,(char*)"MSG")==0)) FrameType=RESP_MSG;
    else if ((strcmp((char*)FrameId,(char*)"CMSG")==0)) FrameType=RESP_CMSG;
    else if ((strcmp((char*)FrameId,(char*)"MSGHEX")==0)) FrameType=RESP_MSGHEX;
    else if ((strcmp((char*)FrameId,(char*)"CMSGHEX")==0)) FrameType=RESP_CMSGHEX;
    else if ((strcmp((char*)FrameId,(char*)"PMSG")==0)) FrameType=RESP_PMSG;
    else if ((strcmp((char*)FrameId,(char*)"PMSGHEX")==0)) FrameType=RESP_PMSGHEX;
    else if ((strcmp((char*)FrameId,(char*)"PORT")==0)) FrameType=RESP_PORT;
    else if ((strcmp((char*)FrameId,(char*)"ADR")==0)) FrameType=RESP_ADR;
    else if ((strcmp((char*)FrameId,(char*)"DR")==0)) FrameType=RESP_DR;
    else if ((strcmp((char*)FrameId,(char*)"CH")==0)) FrameType=RESP_CH;
    else if ((strcmp((char*)FrameId,(char*)"POWER")==0)) FrameType=RESP_POWER;
    else if ((strcmp((char*)FrameId,(char*)"RETRY")==0)) FrameType=RESP_RETRY;
    else if ((strcmp((char*)FrameId,(char*)"RXWIN2")==0)) FrameType=RESP_RXWIN2;
    else if ((strcmp((char*)FrameId,(char*)"RXWIN1")==0)) FrameType=RESP_RXWIN1;
    else if ((strcmp((char*)FrameId,(char*)"KEY")==0)) FrameType=RESP_KEY;
    else if ((strcmp((char*)FrameId,(char*)"MODE")==0)) FrameType=RESP_MODE;
    else if ((strcmp((char*)FrameId,(char*)"JOIN")==0)) FrameType=RESP_JOIN;
    else if ((strcmp((char*)FrameId,(char*)"CLASS")==0)) FrameType=RESP_CLASS;
    else FrameType=RESP_UNKNOWN;
    
    return FrameType;
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
    //LATAbits.LATA0 = ~LATAbits.LATA0;   // Debug purposes
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
