/**
* @file lib_LoRaE5.h
* 
* @author Alexis ROLLAND 
*/

#ifndef __LIB_LORAE5_H__
#define __LIB_LORAE5_H__

#include <xc.h>
#include <stdio.h>
#include <string.h>
typedef enum {false,true} bool_t;  
typedef enum {WAIT_SOL,RX_ON,EOL_RECEIVED} RxStatus_t;
typedef enum {  RESP_UNKNOWN,
                RESP_AT,
                RESP_VER,
                RESP_ID,
                RESP_RESET,
                RESP_MSG,
                RESP_CMSG,
                RESP_MSGHEX,
                RESP_CMSGHEX,
                RESP_PMSG,
                RESP_PMSGHEX,
                RESP_PORT,
                RESP_ADR,
                RESP_DR,
                RESP_CH,
                RESP_POWER,
                RESP_RETRY,
                RESP_RXWIN2,
                RESP_RXWIN1,
                RESP_KEY,
                RESP_MODE,
                RESP_JOIN,
                RESP_CLASS                
                } resp_id_t;
                

typedef enum{
            E5_OK,
            E5_ERROR,
            E5_TIME_OUT,
            E5_BUSY} e5_error_t;

typedef enum{TIME_OUT_STOPPED, TIME_OUT_RUNNING,TIME_OUT_EXPIRED} TimeOutStatus_t;



#define E5_SOL      0x2B        // '+'
#define E5_EOL      0x0A        // LF
#define BUFFER_SIZE 65

#define VAL_500MS   7812        // @Fcy=4MHz

/**
 * @brief Checks UART connection between the app MCU and the LoRa-E5-Mini module
 * This function uses the dummy AT command
 * 
 * @return E5_OK on success 
*/
e5_error_t  E5_CheckConnection(void);

/**
 * @brief  Initiates a joining process with the configured LoRaWAN app 
 * 
 * @param[in] ForceJoin true to use the force join (disconnect before joining)
 * 
 * @return E5_OK on success 
*/
e5_error_t  E5_Join(bool_t ForceJoin);

/**
 * @brief Sends a string to the LoRaWAN app 
 * 
 * @param[in] pMsg Pointer to the "CString" to transmit
 * 
 * @return E5_OK on success 
*/
e5_error_t  E5_SendStrMsg(uint8_t *pMsg);

/**
 * @brief Sends a byte buffer to the LoRaWAN app 
 * 
 * @param[in] pByteBuffer Pointer to the data to Tx
 * @param[in] size number of bytes to Tx
 * 
 * @return E5_OK on success 
*/
e5_error_t  E5_SendByteMsg(uint8_t *pByteBuffer, uint8_t size);

/**
 * @brief Resets the LoRa-E5 module
 * 
 * @warning Wait for at least 5 seconds before trying a join procedure after a reset
 * 
 * @return E5_OK on success 
*/
e5_error_t  E5_Reset(void);

/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE : Time Out value, in x500ms Tic                                       */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
void                E5_startTimeOut(uint8_t Nb500ms);
TimeOutStatus_t     E5_getTimeOutStatus(void);
void                E5_ClearTimeOutStatus(void);
void                E5_ReStartTimeOut(void);
/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
void    E5_ll_send(uint8_t *pstr);    

/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description : removes RC & LF cars from the end rx frame                 */
/*                  removes the initial "+"                                 */
/* PE :                                                                     */
/* ps :   Frame Resp Id                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
resp_id_t    E5_trim(uint8_t *pDest, uint8_t *pSource);

/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/



#endif


