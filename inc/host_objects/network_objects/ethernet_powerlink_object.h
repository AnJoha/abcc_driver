/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 0401fde (2024-11-13)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** This is the public header file for the Ethernet Powerlink Object.
********************************************************************************
*/

#ifndef EPL_H
#define EPL_H

/*------------------------------------------------------------------------------
** Processes commands sent to the EPL object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage - Pointer to a received message addressing the EPL object.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void EPL_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#endif  /* inclusion lock */
