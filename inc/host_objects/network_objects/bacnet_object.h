/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 0401fde (2024-11-13)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2019-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** This is the public header file for the BACnet object.
********************************************************************************
*/
#ifndef BAC_H
#define BAC_H

#include "abcc_types.h"

/*------------------------------------------------------------------------------
** BACnet-specific constants for the Advanced Mapping functions
**------------------------------------------------------------------------------
*/
#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE

#define BAC_OBJ_TYPE_AN_VAL                        2
#define BAC_OBJ_TYPE_BIN_VAL                       5
#define BAC_OBJ_TYPE_MS_VAL                        19
/*
** The ABCC only supports object instance numbers between 0 and 2039, so
** 0xFFFFFFFF is used as a 'do not assign' value.
*/
#define BAC_OBJ_TYPE_IGNORE                        0xFFFF, 0xFFFFFFFF

#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */

#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE

typedef struct BAC_ObjectListTag
{
   UINT16   iObjType;
   UINT32   lInstance;
}
BAC_ObjectListType;

#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */

/*------------------------------------------------------------------------------
** Process Data map for mapping ADIs to BACnet objects
**
** Maps ADIs to BACnet objects of the given type and instance number.
**
** PORTING ALERT!
** Only used if Advanced Mapping is enabled.
**
** If Advanced Mapping (BAC_IA_SUPPORT_ADV_MAPPING_VALUE) is enabled the ADI
** list must have the same number of elemens as this list and its contents must
** match that list in a 1:1 fashion. I.e. index 'X' in both lists must refer to
** data for the same ADI.
**------------------------------------------------------------------------------
*/
#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE

EXTVAR const BAC_ObjectListType BAC_asObjectList[];

#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */

/*------------------------------------------------------------------------------
** Processes commands sent to the BACnet object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
**------------------------------------------------------------------------------
*/
EXTFUNC void BAC_ProcessCmdMsg( ABP_MsgType* psNewMessage );
#endif  /* inclusion lock */
