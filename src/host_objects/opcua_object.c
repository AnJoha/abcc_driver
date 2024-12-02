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
** Source file for the OPC UA object.
********************************************************************************
*/

#include "abcc_types.h"
#include "abcc.h"
#include "abcc_object_config.h"
#include "application_abcc_handler.h"
#include "abcc_port.h"
#include "abp_opcua.h"
#include "opcua_object.h"

#if OPCUA_OBJ_ENABLE

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define OPCUA_OA_NAME_VALUE                        "OPC UA"
#define OPCUA_OA_REV_VALUE                         2
#define OPCUA_OA_NUM_INST_VALUE                    1
#define OPCUA_OA_HIGHEST_INST_VALUE                1

#if OPCUA_IA_APPLICATION_MODELS_ENABLE
#define OPCUA_APPLICATION_MODEL_ENTRY_DS           ( ABP_UINT8_SIZEOF + ABP_UINT8_SIZEOF + ABP_UINT16_SIZEOF )
#endif

/*------------------------------------------------------------------------------
** Structure describing an OPC UA object.
**------------------------------------------------------------------------------
*/
typedef struct opcua_Object
{
   const char* pcName;
   UINT8  bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
}
opcua_ObjectType;

static const opcua_ObjectType opcua_sObject =
{
   OPCUA_OA_NAME_VALUE,                            /* Name.                                      */
   OPCUA_OA_REV_VALUE,                             /* Revision.                                  */
   OPCUA_OA_NUM_INST_VALUE,                        /* Number of instances.                       */
   OPCUA_OA_HIGHEST_INST_VALUE                     /* Highest instance number.                   */
};

#if OPCUA_IA_SERVER_STATUS_ENABLE
static OPCUA_ServerStatusType opcua_sServerStatus =
{
    ABP_OPCUA_STATUS_NOT_STARTED,                  /* OPC UA server state.                       */
    0                                              /* OPC UA number of active sessions.          */
};
#endif

/*------------------------------------------------------------------------------
** The function that processes the commands to the OPC UA Instance.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage )
{
#if OPCUA_IA_APPLICATION_URI_ENABLE        ||                                  \
    OPCUA_IA_VENDOR_NAMESPACE_URI_ENABLE   ||                                  \
    OPCUA_IA_DEVICE_TYPE_NAME_ENABLE       ||                                  \
    OPCUA_IA_DEVICE_INST_NAME_ENABLE       ||                                  \
    OPCUA_IA_PRODUCT_URI_ENABLE
   char*  pcStr;
   UINT16 iStrLength;
#endif

   if( ABCC_GetMsgInstance( psNewMessage ) != 1 )
   {
      /*
      ** The requested instance does not exist.
      ** Respond with a error.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );

      return;
   }

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if OPCUA_IA_MODEL_ENABLE
      case ABP_OPCUA_IA_MODEL:

         /*
         ** The 'OPC UA model' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, OPCUA_IA_MODEL_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OPCUA_IA_MODEL_DS );
         break;
#endif
#if OPCUA_IA_APPLICATION_URI_ENABLE
      case ABP_OPCUA_IA_APPLICATION_URI:

         /*
         ** The 'Application/Local namespace URI' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_APPLICATION_URI_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_APPLICATION_URI_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
#endif
#if OPCUA_IA_VENDOR_NAMESPACE_URI_ENABLE
      case ABP_OPCUA_IA_VENDOR_NAMESPACE_URI:

         /*
         ** The 'Vendor namespace URI' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_VENDOR_NAMESPACE_URI_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_VENDOR_NAMESPACE_URI_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
#endif
#if OPCUA_IA_DEVICE_TYPE_NAME_ENABLE
      case ABP_OPCUA_IA_DEVICE_TYPE_NAME:

         /*
         ** The 'DeviceType name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_DEVICE_TYPE_NAME_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_DEVICE_TYPE_NAME_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
#endif
#if OPCUA_IA_DEVICE_INST_NAME_ENABLE
      case ABP_OPCUA_IA_DEVICE_INST_NAME:

         /*
         ** The 'Device instance name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_DEVICE_INST_NAME_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_DEVICE_INST_NAME_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
#endif
#if OPCUA_IA_PRODUCT_URI_ENABLE
      case ABP_OPCUA_IA_PRODUCT_URI:

         /*
         ** The 'Product URI' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_PRODUCT_URI_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_PRODUCT_URI_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
#endif
#if OPCUA_IA_LIMITS_ENABLE
      case ABP_OPCUA_IA_LIMITS:

         /*
         ** The 'Limits' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, OPCUA_IA_LIMITS_MAX_NUM_MON_ITEMS, 0 );
         ABCC_SetMsgData32( psNewMessage, OPCUA_IA_LIMITS_MIN_PUBLISHING_INT_MS, ABP_UINT16_SIZEOF );
         ABCC_SetMsgData32( psNewMessage, OPCUA_IA_LIMITS_MIN_SAMPLING_INT_MS, ABP_UINT16_SIZEOF + ABP_UINT32_SIZEOF );
         ABCC_SetMsgData32( psNewMessage, OPCUA_IA_LIMITS_MAX_QUEUE_SIZE, ABP_UINT16_SIZEOF + ABP_UINT32_SIZEOF + ABP_UINT32_SIZEOF );

         ABP_SetMsgResponse( psNewMessage, ABP_OPCUA_IA_LIMITS_DS );
         break;
#endif

#if OPCUA_IA_APPLICATION_MODELS_ENABLE
      case ABP_OPCUA_IA_APPLICATION_MODELS:
      {
         UINT8 bNumOfListEntries;
         OPCUA_ApplicationModelEntryType const * pasApplModelList;
         UINT8 bListIndex;
         UINT16 iMsgDataOffset;

         bNumOfListEntries = OPCUA_CbfGetApplModelListNum();
         if( ( bNumOfListEntries == 0 ) || ( bNumOfListEntries > ABP_OPCUA_IA_APPLICATIONS_MODELS_MAX  ) )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_GENERAL_ERROR );
            break;
         }

         pasApplModelList = OPCUA_CbfGetApplModelList();
         if( pasApplModelList == NULL )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_GENERAL_ERROR );
            break;
         }

         iMsgDataOffset = 0;
         for( bListIndex = 0; bListIndex < bNumOfListEntries; bListIndex++ )
         {
            if( ( iMsgDataOffset + OPCUA_APPLICATION_MODEL_ENTRY_DS ) > ABCC_GetMaxMessageSize() )
            {
               ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );
               break;
            }

            ABCC_SetMsgData8( psNewMessage, pasApplModelList[ bListIndex ].bModelFilenameIndex, iMsgDataOffset );
            iMsgDataOffset += ABP_UINT8_SIZEOF;
            ABCC_SetMsgData8( psNewMessage, pasApplModelList[ bListIndex ].bNamespaceUriIndex, iMsgDataOffset );
            iMsgDataOffset += ABP_UINT8_SIZEOF;
            ABCC_SetMsgData16( psNewMessage, pasApplModelList[ bListIndex ].iAdiOffset, iMsgDataOffset );
            iMsgDataOffset += ABP_UINT16_SIZEOF;
         }
         ABP_SetMsgResponse( psNewMessage, iMsgDataOffset );

         break;
      }
#endif

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;
   }
#if OPCUA_IA_SERVER_STATUS_ENABLE
   case ABP_CMD_SET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if OPCUA_IA_SERVER_STATUS_ENABLE
      case ABP_OPCUA_IA_STATUS:
      {
         UINT8 bTemp;

         if( ABCC_GetMsgDataSize( psNewMessage ) > ABP_OPCUA_IA_STATUS_DS )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );
            break;
         }
         else if( ABCC_GetMsgDataSize( psNewMessage ) < ABP_OPCUA_IA_STATUS_DS )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_NOT_ENOUGH_DATA );
            break;
         }

         ABCC_GetMsgData8( psNewMessage, &bTemp, 0 );
         opcua_sServerStatus.eState = (ABP_OpcuaStateType)bTemp;
         ABCC_GetMsgData8( psNewMessage, &opcua_sServerStatus.bActiveSessions, ABP_ENUM_SIZEOF );
         ABP_SetMsgResponse( psNewMessage, 0 );

         break;
      }
#endif
      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;
   }
#endif
   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   } /* End switch( command number ) */
}

/*------------------------------------------------------------------------------
** The function that processes the commands to the OPC UC Object (instance 0).
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void ObjectCommand( ABP_MsgType* psNewMessage )
{
   UINT16 iStrLength;

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:

         /*
         ** The 'name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         iStrLength = (UINT16)strlen( opcua_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, opcua_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;

      case ABP_OA_REV:

         /*
         ** The 'revision' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, opcua_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** The 'Number of Instances' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, opcua_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** The 'Highest Instance Number' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, opcua_sObject.iHighestInstanceNo, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_HIGHEST_INST_DS );
         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }

      break;
   } /* End 'case ABP_CMD_GET_ATTR' */

   case ABP_OPCUA_CMD_METHOD_CALL:
   {
      /*
      ** PORTING ALERT!
      **
      ** Presently this is just a placeholder for the processing that
      ** needs to be made when a Method_Call command is received. The
      ** application needs to extract the relevant data from the command
      ** message and build a suitable response message based on that.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   }

#if OPCUA_IA_APPLICATION_MODELS_ENABLE
   case ABP_OPCUA_CMD_GET_APPLICATION_MODELS_FILENAME:
   case ABP_OPCUA_CMD_GET_APPLICATION_MODELS_NAMESPACE:
   {
      UINT8 bCmd;
      UINT8 bNumOfEntries;
      UINT8 bIndex;
      char const **ppacList;
      UINT16 iStringLength;

      bCmd = ABCC_GetMsgCmdBits( psNewMessage );
      if( bCmd == ABP_OPCUA_CMD_GET_APPLICATION_MODELS_FILENAME )
      {
         bNumOfEntries = OPCUA_CbfGetApplModelFilenameListNum();
         ppacList = OPCUA_CbfGetApplModelFilenameList();
      }
      else
      {
         bNumOfEntries = OPCUA_CbfGetApplModelNamespaceListNum();
         ppacList = OPCUA_CbfGetApplModelNamespaceList();
      }

      if( bNumOfEntries == 0 )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_GENERAL_ERROR );
         break;
      }

      bIndex = ABCC_GetMsgCmdExt0( psNewMessage );
      if( bIndex >= bNumOfEntries )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }

      if( ppacList == NULL )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_GENERAL_ERROR );
         break;
      }

      iStringLength = (UINT16)strlen( ppacList[ bIndex ] );
      if( iStringLength > ABCC_GetMaxMessageSize() )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );
         break;
      }

      ABCC_SetMsgString( psNewMessage, ppacList[ bIndex ], iStringLength, 0 );
      ABP_SetMsgResponse( psNewMessage, iStringLength );

      break;
   }
#endif

   default:

      /*
      ** Unsupported command.
      ** Respond with an error.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   } /* End switch( command number ) */
}

void OPCUA_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** Process the OPC UAC Object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Process the OPC UA Instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

#if OPCUA_IA_SERVER_STATUS_ENABLE
void OPCUA_GetServerStatus( OPCUA_ServerStatusType* psServerStatus )
{
   *psServerStatus = opcua_sServerStatus;
}
#endif /* OPCUA_IA_SERVER_STATUS_ENABLE */

#endif /* OPCUA_OBJ_ENABLE */
