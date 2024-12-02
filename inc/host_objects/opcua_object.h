/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version edc67ee (2024-10-25)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2017-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** This is the public header file for the OPC UA object.
********************************************************************************
*/
#ifndef OPCUA_OBJ_H
#define OPCUA_OBJ_H

#include "abp_opcua.h"

#if OPCUA_IA_APPLICATION_MODELS_ENABLE
typedef struct OPCUA_ApplicationModelEntryTag
{
   UINT8    bModelFilenameIndex;
   UINT8    bNamespaceUriIndex;
   UINT16   iAdiOffset;
} OPCUA_ApplicationModelEntryType;
#endif

#if OPCUA_IA_SERVER_STATUS_ENABLE
/*------------------------------------------------------------------------------
** Struct for the "Status" attribute.
**------------------------------------------------------------------------------
*/
typedef struct OPCUA_ServerStatus
{
   ABP_OpcuaStateType eState;
   UINT8 bActiveSessions;
}
OPCUA_ServerStatusType;
#endif

/*------------------------------------------------------------------------------
** The function that processes the commands sent to the OPC UA object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void OPCUA_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#if OPCUA_IA_APPLICATION_MODELS_ENABLE
/*------------------------------------------------------------------------------
** Callbacks used to fetch pointers to the Application model, nodeset filename,
** and namespace URI lists.
**------------------------------------------------------------------------------
** Arguments:
**    -
**
** Returns:
**    Pointer to a list of the corresponding type.
**    NULL on errors.
**------------------------------------------------------------------------------
*/
EXTFUNC OPCUA_ApplicationModelEntryType const *OPCUA_CbfGetApplModelList( void );
EXTFUNC char const **OPCUA_CbfGetApplModelFilenameList( void );
EXTFUNC char const **OPCUA_CbfGetApplModelNamespaceList( void );

/*------------------------------------------------------------------------------
** Callbacks used to fetch the number of elements in the Application model,
** nodeset filename, and namespace URI lists.
**------------------------------------------------------------------------------
** Arguments:
**    -
**
** Returns:
**    The number of elements in the list.
**    Value '0' on error.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 OPCUA_CbfGetApplModelListNum( void );
EXTFUNC UINT8 OPCUA_CbfGetApplModelFilenameListNum( void );
EXTFUNC UINT8 OPCUA_CbfGetApplModelNamespaceListNum( void );
#endif

#if OPCUA_IA_SERVER_STATUS_ENABLE
/*------------------------------------------------------------------------------
** The function updates the contents of the output argument with the most recent
** status of the OPC UA server. The status contains the state of the OPC UA
** server and the number of active sessions.
**------------------------------------------------------------------------------
** Arguments:
**    psServerStatus    - Pointer to a OPCUA_ServerStatusType status holder.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void OPCUA_GetServerStatus( OPCUA_ServerStatusType* psServerStatus );
#endif  /* OPCUA_IA_SERVER_STATUS_ENABLE */

#endif  /* inclusion lock */
