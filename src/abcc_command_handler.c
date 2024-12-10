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


static const attr_lookup_type attribute_table[] =
{
   ABCC_APPLICATION_OBJ_SERIAL_NR_CBFUNC,

   //ABCC_APPLICATION_OBJ_PRODUCT_NAME_VALUE("Liquid pumper 9000"),
   ABCC_APPLICATION_OBJ_PRODUCT_NAME_CBFUNC,

   //ABCC_APPLICATION_OBJ_VENDOR_NAME_VALUE("Hydro org"),
   ABCC_APPLICATION_OBJ_VENDOR_NAME_CBFUNC,



   //ABCC_ETHERNETIP_OBJ_VENDOR_ID_VALUE(1122),
   ABCC_ETHERNETIP_OBJ_VENDOR_ID_CBFUNC,

   //ABCC_ETHERNETIP_OBJ_DEVICE_TYPE_VALUE(5545),
   ABCC_ETHERNETIP_OBJ_DEVICE_TYPE_CBFUNC,

   //ABCC_ETHERNETIP_OBJ_PRODUCT_CODE_VALUE(112),
   ABCC_ETHERNETIP_OBJ_PRODUCT_CODE_CBFUNC,





   //ABCC_PROFINET_OBJ_DEVICE_ID_VALUE(0x1234),
   ABCC_PROFINET_OBJ_DEVICE_ID_CBFUNC,

   //ABCC_PROFINET_OBJ_VENDOR_ID_VALUE(0x00AB),
   ABCC_PROFINET_OBJ_VENDOR_ID_CBFUNC,

   //ABCC_PROFINET_OBJ_ORDER_ID_VALUE("Buy self @store"),
   ABCC_PROFINET_OBJ_ORDER_ID_CBFUNC,
};

void ABCC_HandleCommandMessage( ABP_MsgType* psReceivedMsg )
{
#if ABCC_CFG_PRE_PROCESS_READ_MESSAGES_ENABLED
   if( ABCC_CbfReceiveMsg( psReceivedMsg ) )
   {
      return;
   }
#endif

   /*
   ** For any unhandled Msg look for a handler in the table.
   */
    if( ( ABCC_GetMsgCmdBits(psReceivedMsg) == ABP_CMD_GET_ATTR ) &&
        ( ABCC_GetMsgInstance(psReceivedMsg) == 1 ) )
    {
        const attr_lookup_type* pFoundEntry = NULL;

        UINT8  bDestObj  = ABCC_GetMsgDestObj(psReceivedMsg);
        UINT8  bAttr     = ABCC_GetMsgCmdExt0(psReceivedMsg);

        for( size_t i = 0; i < sizeof(attribute_table) / sizeof(attr_lookup_type); i++ )
        {
            const attr_lookup_type*  pEntry = &attribute_table[i];
            if( ( bDestObj  == pEntry->bObject ) &&
                ( bAttr     == pEntry->bAttribute ) )
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
           case SERVICE_INT16:
           case SERVICE_UINT16:
              ABCC_SetMsgData16(
                 psReceivedMsg,
                 pFoundEntry->uCbx.pnGetUint16Attr ?
                    pFoundEntry->uCbx.pnGetUint16Attr() :
                    pFoundEntry->uData.iUnsigned16,
                 0 );
              ABP_SetMsgResponse( psReceivedMsg, ABP_UINT16_SIZEOF );
              ABCC_SendRespMsg( psReceivedMsg );
              return;

           case SERVICE_INT32:
           case SERVICE_UINT32:
              ABCC_SetMsgData32(
                 psReceivedMsg,
                 pFoundEntry->uCbx.pnGetUint32Attr ?
                    pFoundEntry->uCbx.pnGetUint32Attr() :
                    pFoundEntry->uData.lUnsigned32,
                 0 );
              ABP_SetMsgResponse( psReceivedMsg, ABP_UINT32_SIZEOF );
              ABCC_SendRespMsg( psReceivedMsg );
              return;

           case SERVICE_STR:
              {
                 UINT16 iStrLength;

                 if( pFoundEntry->uCbx.pnGetStrAttr )
                 {
                    iStrLength = pFoundEntry->uCbx.pnGetStrAttr(
                       (char*)ABCC_GetMsgDataPtr( psReceivedMsg ),
                       ABCC_MATH_MIN( ABCC_CFG_MAX_MSG_SIZE,
                                      pFoundEntry->uData.iAttrMaxDataSize ) );
                 }
                 else
                 {
                    const char* pStr = pFoundEntry->uData.pacString;
                    iStrLength = (UINT16)strlen(pStr);
                    ABCC_SetMsgString( psReceivedMsg,
                                       pStr,
                                       iStrLength,
                                       0 );
                 }

                 ABP_SetMsgResponse( psReceivedMsg, iStrLength );
                 ABCC_SendRespMsg( psReceivedMsg );
                 return;
              }
           }

           ABCC_LOG_FATAL(
              ABCC_EC_INTERNAL_ERROR,
              pFoundEntry->eServiceTag,
              "Missing attribute handler implementation\n");
        }
    }



   switch( ABCC_GetMsgDestObj( psReceivedMsg ) )
   {
#if MQTT_OBJ_ENABLE
   case ABP_OBJ_NUM_MQTT:

      MQTT_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if OPCUA_OBJ_ENABLE
   case ABP_OBJ_NUM_OPCUA:

      OPCUA_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if SAFE_OBJ_ENABLE
   case ABP_OBJ_NUM_SAFE:

      SAFE_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if EPL_OBJ_ENABLE
   case ABP_OBJ_NUM_EPL:

      EPL_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if EIP_OBJ_ENABLE
   case ABP_OBJ_NUM_EIP:

      EIP_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if PRT_OBJ_ENABLE
   case ABP_OBJ_NUM_PNIO:

      PRT_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if DPV1_OBJ_ENABLE
   case ABP_OBJ_NUM_DPV1:

      DPV1_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if DEV_OBJ_ENABLE
   case ABP_OBJ_NUM_DEV:

      DEV_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if MOD_OBJ_ENABLE
   case ABP_OBJ_NUM_MOD:

      MOD_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if COP_OBJ_ENABLE
   case ABP_OBJ_NUM_COP:

      COP_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if ETN_OBJ_ENABLE
   case ABP_OBJ_NUM_ETN:

      ETN_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if ECT_OBJ_ENABLE
   case ABP_OBJ_NUM_ECT:

      ECT_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
   case ABP_OBJ_NUM_APPD:

      AD_ProcObjectRequest( psReceivedMsg );
      break;

#if APP_OBJ_ENABLE
   case ABP_OBJ_NUM_APP:

      APP_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if SYNC_OBJ_ENABLE
   case ABP_OBJ_NUM_SYNC:

      SYNC_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if CCL_OBJ_ENABLE
   case ABP_OBJ_NUM_CCL:

      CCL_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if CFN_OBJ_ENABLE
   case ABP_OBJ_NUM_CFN:

      CFN_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if CIET_OBJ_ENABLE
   case ABP_OBJ_NUM_CIET:

      CIET_ProcessCmdMsg(psReceivedMsg);
      break;
#endif
#if ASM_OBJ_ENABLE
   case ABP_OBJ_NUM_ASM:

      ASM_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
#if BAC_OBJ_ENABLE
   case ABP_OBJ_NUM_BAC:

      BAC_ProcessCmdMsg( psReceivedMsg );
      break;
#endif
   default:

      /*
      ** We have received a command to an unsupported object.
      */
      ABP_SetMsgErrorResponse( psReceivedMsg, 1, ABP_ERR_UNSUP_OBJ );
      ABCC_SendRespMsg( psReceivedMsg );
      break;
   }
}
