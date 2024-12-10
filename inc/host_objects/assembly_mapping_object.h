/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 0401fde (2024-11-13)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2018-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Assembly Mapping Object 0xEB - Public interfaces
********************************************************************************
*/

#ifndef ASM_OBJ_H_
#define ASM_OBJ_H_

#include "abcc_types.h"

/*------------------------------------------------------------------------------
** Structure representing an assembly mapping instance.
**------------------------------------------------------------------------------
*/
typedef struct ASM_Instance
{
   const UINT32 lDescriptor;
#if ASM_SUPPORT_DYNAMIC_INSTANCES
   AD_MapType* psMap;
#else
   const AD_MapType* psMap;
#endif
#if ASM_IA_NAME_ENABLE
   const char* pacName;
#endif
#if ASM_IA_MAX_NUM_ADI_MAPS_ENABLE
   const UINT16 iMaxNumAdiMaps;
#endif
}
ASM_InstanceType;

/*------------------------------------------------------------------------------
** Get assembly data for an instance.
**------------------------------------------------------------------------------
** Arguments:
**    iInstance     - Assembly mapping instance.
**    pxBuffer      - Buffer to put the assembly data in.
**    piOctetOffset - Offset in octets in pxBuffer where the data will be
**                    written to. This offset will be incremented according to
**                    the size written.
**
** Returns:
**    TRUE - Data was successfully written to destionation buffer.
**    FALSE - Invalid assembly mapping instance.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ASM_GetData( UINT16 iInstance,
                          void* pxBuffer,
                          UINT16* piOctetOffset );

/*------------------------------------------------------------------------------
** Initializes the Assembly Mapping object.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ASM_Init( void );

/*------------------------------------------------------------------------------
** Process an object request against the Assembly Mapping Object.
**------------------------------------------------------------------------------
** Arguments:
**    psMsgBuffer - Pointer to ABCC command message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ASM_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#endif  /* inclusion lock */
