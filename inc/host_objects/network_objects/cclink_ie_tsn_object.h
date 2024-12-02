/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version edc67ee (2024-10-25)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2022-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** This is the public header file for the CC-Link IE TSN Object.
********************************************************************************
*/

#ifndef CIET_H
#define CIET_H

/*------------------------------------------------------------------------------
** Processes commands sent to the CIET object
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void CIET_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#endif  /* inclusion lock */
