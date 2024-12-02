/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version edc67ee (2024-10-25)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** This is the public header file for the DeviceNet Object.
********************************************************************************
*/

#ifndef DEV_H
#define DEV_H

#if ASM_OBJ_ENABLE
/*------------------------------------------------------------------------------
** Variable declarations for attribute #21(Producing Instance Number list) and
** attribute #22(Consuming Instance Number list).
** If the Assembly object is enabled, those variables must be defined elsewhere.
** An example is available in
** "/example_application/application_data_instance_mapping_asm.c".
**------------------------------------------------------------------------------
*/
EXTVAR const UINT16 DEV_aiProdInstNumList[];
EXTVAR const UINT16 DEV_iProdInstNumListSize;

EXTVAR const UINT16 DEV_aiConsInstNumList[];
EXTVAR const UINT16 DEV_iConsInstNumListSize;

#endif /* ASM_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** Processes commands sent to the DEV object
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void DEV_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#endif  /* inclusion lock */
