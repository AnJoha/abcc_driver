/*******************************************************************************
** Copyright 2024-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Handle incoming ABCC commands.
********************************************************************************
*/

#include "abcc.h"
#include "abcc_command_handler.h"
#include "host_objects/abcc_command_handler_lookup.h"

#include "host_objects/application_data_object.h"
#include "host_objects/application_object.h"
#include "host_objects/assembly_mapping_object.h"
#include "host_objects/ethernet_object.h"
#include "host_objects/mqtt_object.h"
#include "host_objects/opcua_object.h"
#include "host_objects/safety_object.h"
#include "host_objects/sync_object.h"

#include "host_objects/network_objects/bacnet_object.h"
#include "host_objects/network_objects/canopen_object.h"
#include "host_objects/network_objects/cclink_ie_field_network_object.h"
#include "host_objects/network_objects/cclink_ie_tsn_object.h"
#include "host_objects/network_objects/cclink_object.h"
#include "host_objects/network_objects/devicenet_object.h"
#include "host_objects/network_objects/ethercat_object.h"
#include "host_objects/network_objects/ethernetip_object.h"
#include "host_objects/network_objects/ethernet_powerlink_object.h"
#include "host_objects/network_objects/modbus_object.h"
#include "host_objects/network_objects/profibus_dpv1_object.h"
#include "host_objects/network_objects/profinet_io_object.h"

#define ABCC_MATH_MIN( nX, nY )   ( ( (nX) < (nY) ) ? (nX) : (nY) )

#ifdef ABCC_CFG_COMMAND_RESPONSE_LIST
static const Command_Handler_Lookup_Type command_handler_lookup_table[] = { ABCC_CFG_OBJ_COMMAND_RESPONSE_LIST, 
                                                    ABCC_CFG_COMMAND_RESPONSE_LIST };
#endif

static void GetAttributeHandler( ABP_MsgType* psCommandMessage, Command_Handler_Lookup_Type* psEntry )
{
   switch( psEntry->eServiceTag )
   {
      case SERVICE_INT8:
      case SERVICE_UINT8:
         ABCC_SetMsgData8( psCommandMessage,
                           psEntry->uCbx.pnGetUint8Attr ?
                           psEntry->uCbx.pnGetUint8Attr() :
                           psEntry->uData.bUnsigned8,
                           0 );
         ABP_SetMsgResponse( psCommandMessage, ABP_UINT8_SIZEOF );
         ABCC_SendRespMsg( psCommandMessage );
         return;
      case SERVICE_INT16:
      case SERVICE_UINT16:
         ABCC_SetMsgData16( psCommandMessage,
                            psEntry->uCbx.pnGetUint16Attr ?
                            psEntry->uCbx.pnGetUint16Attr() :
                            psEntry->uData.iUnsigned16,
                            0 );
         ABP_SetMsgResponse( psCommandMessage, ABP_UINT16_SIZEOF );
         ABCC_SendRespMsg( psCommandMessage );
         return;
      case SERVICE_INT32:
      case SERVICE_UINT32:
         ABCC_SetMsgData32( psCommandMessage,
                            psEntry->uCbx.pnGetUint32Attr ?
                            psEntry->uCbx.pnGetUint32Attr() :
                            psEntry->uData.lUnsigned32,
                            0 );
         ABP_SetMsgResponse( psCommandMessage, ABP_UINT32_SIZEOF );
         ABCC_SendRespMsg( psCommandMessage );
         return;
      case SERVICE_STR:
      {
         UINT16 iStrLength;
         if( psEntry->uCbx.pnGetStrAttr )
         {
            iStrLength = psEntry->uCbx.pnGetStrAttr( (char*)ABCC_GetMsgDataPtr( psCommandMessage ),
                                                     ABCC_MATH_MIN( ABCC_CFG_MAX_MSG_SIZE,
                                                     psEntry->uData.iAttrMaxDataSize ) );
         }
         else
         {
            const char* pStr = psEntry->uData.pacString;
            iStrLength = ( UINT16 )strlen( pStr );
            ABCC_SetMsgString( psCommandMessage,
                               pStr,
                               iStrLength,
                               0 );
         }
         ABP_SetMsgResponse( psCommandMessage, iStrLength );
         ABCC_SendRespMsg( psCommandMessage );
         return;
      }
      default:
         ABCC_LOG_ERROR( ABCC_EC_UNSUPPORTED_DATA_TYPE, (UINT32)psEntry->eServiceTag, "Response to Get_Attribute command not possible (unsupported datatype).\n" );
         break;
   }
}

static void SetAttributeHandler( ABP_MsgType* psCommandMessage, Command_Handler_Lookup_Type* psEntry )
{
    switch ( psEntry->eServiceTag )
    {
    case SERVICE_INT8:
    case SERVICE_UINT8:
        UINT8 bValue = 0;
        ABCC_GetMsgData8( psCommandMessage, &bValue, 0 );
        psEntry->uCbx.pnSetUint8Attr( bValue );
        ABP_SetMsgResponse( psCommandMessage, 0 );
        ABCC_SendRespMsg( psCommandMessage );
        return;
    case SERVICE_INT16:
    case SERVICE_UINT16:
        UINT16 iValue = 0;
        ABCC_GetMsgData16( psCommandMessage, &iValue, 0 );
        psEntry->uCbx.pnSetUint8Attr( iValue );
        ABP_SetMsgResponse( psCommandMessage, 0 );
        ABCC_SendRespMsg( psCommandMessage );
        return;
    case SERVICE_INT32:
    case SERVICE_UINT32:
        UINT32 lValue = 0;
        ABCC_GetMsgData32( psCommandMessage, &lValue, 0 );
        psEntry->uCbx.pnSetUint8Attr( lValue );
        ABP_SetMsgResponse( psCommandMessage, 0 );
        ABCC_SendRespMsg( psCommandMessage );
        return;
    case SERVICE_STR:
        psEntry->uCbx.pnSetStrAttr(
           (char*)ABCC_GetMsgDataPtr( psCommandMessage ),
           ABCC_MATH_MIN( ABCC_CFG_MAX_MSG_SIZE,
           psEntry->uData.iAttrMaxDataSize ) );
        return;
    default:
      ABCC_LOG_ERROR( ABCC_EC_UNSUPPORTED_DATA_TYPE, (UINT32)psEntry->eServiceTag, "Unsupported Set_Attribute datatype.\n" );
      break;
    }
}

void ABCC_HandleCommandMessage( ABP_MsgType* psCommandMessage )
{
#if ABCC_CFG_CATCH_COMMAND_MESSAGES_ENABLED
   if( ABCC_CbfCatchCommandMessage( psCommandMessage ) )
   {
      return;
   }
#endif
#ifdef ABCC_CFG_COMMAND_RESPONSE_LIST

const Command_Handler_Lookup_Type* psFoundEntry = NULL;

   UINT8 bDestObj      = ABCC_GetMsgDestObj( psCommandMessage );
   UINT8 bInst         = ABCC_GetMsgInstance( psCommandMessage );
   ABP_MsgCmdType bCmd = ABCC_GetMsgCmdBits( psCommandMessage );

   if( bDestObj != ABP_OBJ_NUM_APPD )
   {
      for( size_t i = 0; i < sizeof( command_handler_lookup_table ) / sizeof( Command_Handler_Lookup_Type ); i++ )
      {
         const Command_Handler_Lookup_Type*  psEntry = &command_handler_lookup_table[i];
         if( ( bDestObj == psEntry->bObject ) &&
             ( bInst    == psEntry->bInstance ) &&
             ( bCmd     == psEntry->bCommand ) )
         {
            switch ( bCmd )
            {
            case ABP_CMD_GET_ATTR:
               if( ABCC_GetMsgCmdExt0( psCommandMessage ) == psEntry->uCmdExt.bAttr )
               {
                  GetAttributeHandler( psCommandMessage, psEntry );
                  return;
               }
               break;
            case ABP_CMD_SET_ATTR:
               if( ABCC_GetMsgCmdExt0( psCommandMessage ) == psEntry->uCmdExt.bAttr )
               {
                  SetAttributeHandler( psCommandMessage, psEntry );
                  return;
               }
               break;
            case ABP_CMD_RESET:
               psEntry->uCbx.pnResetObj( ABCC_GetMsgCmdExt1( psCommandMessage ) );
               return;
            default:
               break;
            }
         }
      }
   }
#endif
   switch( ABCC_GetMsgDestObj( psCommandMessage ) )
   {
#if MQTT_OBJ_ENABLE
   case ABP_OBJ_NUM_MQTT:

      MQTT_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if OPCUA_OBJ_ENABLE
   case ABP_OBJ_NUM_OPCUA:

      OPCUA_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if SAFE_OBJ_ENABLE
   case ABP_OBJ_NUM_SAFE:

      SAFE_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if EPL_OBJ_ENABLE
   case ABP_OBJ_NUM_EPL:

      EPL_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if EIP_OBJ_ENABLE
   case ABP_OBJ_NUM_EIP:

      EIP_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if PRT_OBJ_ENABLE
   case ABP_OBJ_NUM_PNIO:

      PRT_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if DPV1_OBJ_ENABLE
   case ABP_OBJ_NUM_DPV1:

      DPV1_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if DEV_OBJ_ENABLE
   case ABP_OBJ_NUM_DEV:

      DEV_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if MOD_OBJ_ENABLE
   case ABP_OBJ_NUM_MOD:

      MOD_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if COP_OBJ_ENABLE
   case ABP_OBJ_NUM_COP:

      COP_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if ETN_OBJ_ENABLE
   case ABP_OBJ_NUM_ETN:

      ETN_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if ECT_OBJ_ENABLE
   case ABP_OBJ_NUM_ECT:

      ECT_ProcessCmdMsg( psCommandMessage );
      break;
#endif
   case ABP_OBJ_NUM_APPD:

      AD_ProcObjectRequest( psCommandMessage );
      break;

#if APP_OBJ_ENABLE
   case ABP_OBJ_NUM_APP:

      APP_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if SYNC_OBJ_ENABLE
   case ABP_OBJ_NUM_SYNC:

      SYNC_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if CCL_OBJ_ENABLE
   case ABP_OBJ_NUM_CCL:

      CCL_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if CFN_OBJ_ENABLE
   case ABP_OBJ_NUM_CFN:

      CFN_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if CIET_OBJ_ENABLE
   case ABP_OBJ_NUM_CIET:

      CIET_ProcessCmdMsg(psCommandMessage);
      break;
#endif
#if ASM_OBJ_ENABLE
   case ABP_OBJ_NUM_ASM:

      ASM_ProcessCmdMsg( psCommandMessage );
      break;
#endif
#if BAC_OBJ_ENABLE
   case ABP_OBJ_NUM_BAC:

      BAC_ProcessCmdMsg( psCommandMessage );
      break;
#endif
   default:

      /*
      ** A command to an unsupported object is received.
      */
      ABP_SetMsgErrorResponse( psCommandMessage, 1, ABP_ERR_UNSUP_OBJ );
      ABCC_SendRespMsg( psCommandMessage );
      break;
   }
}
