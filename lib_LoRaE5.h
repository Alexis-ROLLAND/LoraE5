/***************************************************************************
* Nom du fichier:
* Description	: 
*
* Auteur	: 
* Créé le	:
* Compilateur	: 
***************************************************************************
* Modifié le  	:
***************************************************************************/

#ifndef __LIB_LORAE5_H__
#define __LIB_LORAE5_H__

#include <xc.h>
#include <stdio.h>
#include <string.h>
typedef enum {false,true} bool_t;  
typedef enum {WAIT_SOL,RX_ON,EOL_RECEIVED} RxStatus_t;
//typedef enum {AT_IDLE, AT_IN_PROGRESS, AT_RX_COMPLETE} at_status_t;

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

/*--------------------------------------------------------------------------*/
/* Fonction  :  e5_error_t  E5_CheckConnection(void);                       */
/* Description : Checks connection between MCU and LoRa-E5 module           */
/*              using the "dummy" AT command                                */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
e5_error_t  E5_CheckConnection(void);

/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
e5_error_t  E5_Join(bool_t ForceJoin);

/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
e5_error_t  E5_SendStrMsg(uint8_t *pMsg);

/*--------------------------------------------------------------------------*/
/* Fonction  :                                                              */
/* Description :                                                            */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
e5_error_t  E5_getVER(void);

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
/* Description : removes RC & LF cars from the rx frame                     */
/* PE :                                                                     */
/* ps :                                                                     */
/* Mode d'emploi :                                                          */
/*--------------------------------------------------------------------------*/
void    E5_trim(uint8_t *pDest, uint8_t *pSource);

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


