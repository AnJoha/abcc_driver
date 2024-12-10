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
** This is the public header file for the Functional Safety object.
********************************************************************************
*/
#ifndef SAFE_OBJ_H
#define SAFE_OBJ_H

#include "abcc_object_config.h"

/*------------------------------------------------------------------------------
** The function that processes the commands sent to the Functional Safety
** object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void SAFE_ProcessCmdMsg( ABP_MsgType* psNewMessage );

/*------------------------------------------------------------------------------
** The function will control the ABP_SAFE_IA_SAFETY_ENABLED attribute
** value in runtime.
**------------------------------------------------------------------------------
** Arguments:
**    fEnabled    - TRUE: Safety is enabled; FALSE: Safety is disabled
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
#if( SAFE_IA_SAFETY_ENABLED_ENABLE )
EXTFUNC void SAFE_SetSafetyEnable( BOOL fEnabled );
#endif

#endif  /* inclusion lock */
