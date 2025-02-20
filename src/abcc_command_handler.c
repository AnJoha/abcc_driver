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
#include "host_objects/abcc_host_attributes_lookup.h"

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
static const attr_lookup_type attribute_table[] = ABCC_CFG_COMMAND_RESPONSE_LIST;
#endif

void ABCC_HandleCommandMessage( ABP_MsgType* psCommandMessage )
{
#if ABCC_CFG_CATCH_COMMAND_MESSAGES_ENABLED
   if( ABCC_CbfCatchCommandMessage( psCommandMessage ) )
   {
      return;
   }
#endif
#ifdef ABCC_CFG_COMMAND_RESPONSE_LIST
   /*
   ** For any unhandled Msg look for a handler in the table.
   */
   if( ( ABCC_GetMsgCmdBits( psCommandMessage ) == ABP_CMD_GET_ATTR ) &&
       ( ABCC_GetMsgInstance( psCommandMessage ) == 1 ) )
   {
      const attr_lookup_type* pFoundEntry = NULL;

      UINT8  bDestObj  = ABCC_GetMsgDestObj( psCommandMessage );
      UINT8  bAttr     = ABCC_GetMsgCmdExt0( psCommandMessage );

      for( size_t i = 0; i < sizeof( attribute_table ) / sizeof( attr_lookup_type ); i++ )
      {
         const attr_lookup_type*  pEntry = &attribute_table[i];
         if( ( bDestObj  == pEntry->bObject ) &&
             ( bAttr     == pEntry->bAttribute ) &&
             ( ABP_CMD_GET_ATTR == pEntry->bCommand ) )
         {
            ABCC_LOG_INFO( "Attribute handler found\n" );
            pFoundEntry = pEntry;
            break;  
         }
      }

      if (pFoundEntry)
      {
         switch( pFoundEntry->eServiceTag )
         {
         case SERVICE_INT8:
         case SERVICE_UINT8:
            ABCC_SetMsgData8(
               psCommandMessage,
               pFoundEntry->uCbx.pnGetUint8Attr ?
                  pFoundEntry->uCbx.pnGetUint8Attr() :
                  pFoundEntry->uData.bUnsigned8,
               0 );
            ABP_SetMsgResponse( psCommandMessage, ABP_UINT8_SIZEOF );
            ABCC_SendRespMsg( psCommandMessage );
            return;

         case SERVICE_INT16:
         case SERVICE_UINT16:
            ABCC_SetMsgData16(
               psCommandMessage,
               pFoundEntry->uCbx.pnGetUint16Attr ?
                  pFoundEntry->uCbx.pnGetUint16Attr() :
                  pFoundEntry->uData.iUnsigned16,
               0 );
            ABP_SetMsgResponse( psCommandMessage, ABP_UINT16_SIZEOF );
            ABCC_SendRespMsg( psCommandMessage );
            return;

         case SERVICE_INT32:
         case SERVICE_UINT32:
            ABCC_SetMsgData32(
               psCommandMessage,
               pFoundEntry->uCbx.pnGetUint32Attr ?
                  pFoundEntry->uCbx.pnGetUint32Attr() :
                  pFoundEntry->uData.lUnsigned32,
               0 );
            ABP_SetMsgResponse( psCommandMessage, ABP_UINT32_SIZEOF );
            ABCC_SendRespMsg( psCommandMessage );
            return;

         case SERVICE_STR:
            {
               UINT16 iStrLength;

               if( pFoundEntry->uCbx.pnGetStrAttr )
               {
                  iStrLength = pFoundEntry->uCbx.pnGetStrAttr(
                     (char*)ABCC_GetMsgDataPtr( psCommandMessage ),
                     ABCC_MATH_MIN( ABCC_CFG_MAX_MSG_SIZE,
                     pFoundEntry->uData.iAttrMaxDataSize ) );
               }
               else
               {
                  const char* pStr = pFoundEntry->uData.pacString;
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
         }

         ABCC_LOG_FATAL(
            ABCC_EC_INTERNAL_ERROR,
            pFoundEntry->eServiceTag,
            "Missing attribute handler implementation\n" );
      }
   }
   else
   if( ( ABCC_GetMsgCmdBits( psCommandMessage ) == ABP_CMD_SET_ATTR ) &&
       ( ABCC_GetMsgInstance( psCommandMessage ) == 1 ) )
   {
      const attr_lookup_type* pFoundEntry = NULL;

      UINT8  bDestObj  = ABCC_GetMsgDestObj( psCommandMessage );
      UINT8  bAttr     = ABCC_GetMsgCmdExt0( psCommandMessage );

      for( size_t i = 0; i < sizeof( attribute_table ) / sizeof( attr_lookup_type ); i++ )
      {
         const attr_lookup_type*  pEntry = &attribute_table[i];
         if( ( bDestObj == pEntry->bObject ) &&
             ( bAttr    == pEntry->bAttribute ) &&
             ( ABP_CMD_SET_ATTR == pEntry->bCommand ) )
         {
            ABCC_LOG_INFO( "Attribute handler found\n" );
            pFoundEntry = pEntry;
            break;
         }
      }

      if (pFoundEntry)
      {
         switch( pFoundEntry->eServiceTag )
         {
         case SERVICE_INT8:
         case SERVICE_UINT8:
            UINT8 bValue = 0;
            ABCC_GetMsgData8( psCommandMessage, (UINT8*)&bValue, 0 );
            pFoundEntry->uCbx.pnSetUint8Attr( bValue );
            ABP_SetMsgResponse( psCommandMessage, 0 );
            ABCC_SendRespMsg( psCommandMessage );
            return;
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
