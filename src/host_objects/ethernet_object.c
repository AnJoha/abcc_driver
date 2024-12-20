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
** Source file for the Ethernet Host object.
********************************************************************************
*/

#include "abcc_types.h"
#include "abcc_hardware_abstraction.h"
#include "abcc_object_config.h"
#include "abcc.h"
#include "ethernet_object.h"
#include "abp_etn.h"
#include "string.h"
#include "application_abcc_handler.h"
#include "abcc_port.h"

#if ETN_OBJ_ENABLE

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define ETN_OA_NAME_VALUE                          "Ethernet"
#define ETN_OA_REV_VALUE                           2
#define ETN_OA_NUM_INST_VALUE                      1
#define ETN_OA_HIGHEST_INST_VALUE                  1

/*------------------------------------------------------------------------------
** etn_ObjectType.
** Structure describing an Ethernet Object.
**------------------------------------------------------------------------------
*/
typedef struct etn_ObjectType
{
    const  char* pcName;
    UINT8  bRevision;
    UINT16 iNumberOfInstances;
    UINT16 iHighestInstanceNo;
}
etn_ObjectType;

/*------------------------------------------------------------------------------
** Forward declarations
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage );
static void ObjectCommand( ABP_MsgType* psNewMessage );

#if ETN_IA_NETWORK_STATUS_ENABLE
static UINT16 etn_iNetworkStatus;
#endif
#if ETN_IA_IP_CONFIGURATION_ENABLE
static ETN_IpConfigType etn_sIpConfig;
#endif
static const etn_ObjectType etn_sObject =
{
   ETN_OA_NAME_VALUE,                           /* Name.                                              */
   ETN_OA_REV_VALUE,                            /* Revision.                                          */
   ETN_OA_NUM_INST_VALUE,                       /* Number of instances.                               */
   ETN_OA_HIGHEST_INST_VALUE                    /* Highest instance number.                           */
};

void ETN_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Ethernet Object and it's Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** Ethernet object Command
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Ethernet instance Command
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

#if ETN_IA_IP_CONFIGURATION_ENABLE
void ETN_GetIpConfig( ETN_IpConfigType* psIpConfig )
{
   ABCC_PORT_UseCritical();

   ABCC_PORT_EnterCritical();
   {
      *psIpConfig = etn_sIpConfig;
   }
   ABCC_PORT_ExitCritical();
}
#endif /* #if ETN_IA_IP_CONFIGURATION_ENABLE */

#if ETN_IA_NETWORK_STATUS_ENABLE
void ETN_GetNwStatus( UINT16* piNwStatus )
{
   ABCC_PORT_UseCritical();

   ABCC_PORT_EnterCritical();
   *piNwStatus = etn_iNetworkStatus;
   ABCC_PORT_ExitCritical();
}
#endif /* #if ETN_IA_NETWORK_STATUS_ENABLE */

/*------------------------------------------------------------------------------
** Processes commands to ETN Instances
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
   UINT16 iInstance;
   UINT8 bAttribute;

   iInstance = ABCC_GetMsgInstance( psNewMessage );
   bAttribute = ABCC_GetMsgCmdExt0( psNewMessage );

#if ETN_IA_SNMP_READ_ONLY_ENABLE   ||       \
    ETN_IA_SNMP_READ_WRITE_ENABLE
   char*  pcStr;
   UINT16 iStrLength;
#endif

   /*
   ** This function processes commands to the Ethernet Instance.
   */
   if( iInstance != 1 )
   {
      /*
      ** The Instance does not exist.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );
      return;
   }

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:

      switch( bAttribute )
      {
#if ETN_IA_MAC_ADDRESS_ENABLE
      case ABP_ETN_IA_MAC_ADDRESS:
      {
         UINT16 i;
         UINT8  abMac[ 6 ];

         /*
         ** Copy the 1st Instance 1 attribute (MAC Address) to the message.
         */

         ABCC_PORT_MemCpy( abMac, ETN_IA_MAC_ADDRESS_VALUE, ABP_ETN_IA_MAC_ADDRESS_DS );

         for( i = 0; i < ABP_ETN_IA_MAC_ADDRESS_DS; i++ )
         {
            ABCC_SetMsgData8( psNewMessage, abMac[ i ], i );
         }
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_MAC_ADDRESS_DS );
         break;
      }
#endif
#if ETN_IA_ENABLE_HICP_ENABLE
      case ABP_ETN_IA_ENABLE_HICP:

         /*
         ** Copy the 2nd Instance 1 attribute (Enable HICP) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_HICP_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_HICP_DS );
         break;
#endif
#if ETN_IA_ENABLE_WEB_ENABLE
      case ABP_ETN_IA_ENABLE_WEB:

         /*
         ** Copy the 3rd Instance 1 attribute (Enable WEB) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_WEB_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_WEB_DS );
         break;
#endif
#if ETN_IA_ENABLE_WEB_ADI_ACCESS_ENABLE
      case ABP_ETN_IA_ENABLE_WEB_ADI_ACCESS:

         /*
         ** Copy the 5th Instance 1 attribute (Enable WEB ADI access) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_WEB_ADI_ACCESS_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_WEB_ADI_ACCESS_DS );
         break;
#endif
#if ETN_IA_ENABLE_FTP_ENABLE
      case ABP_ETN_IA_ENABLE_FTP:

         /*
         ** Copy the 6th Instance 1 attribute (Enable FTP) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_FTP_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_FTP_DS );
         break;
#endif
#if ETN_IA_ENABLE_ADMIN_MODE_ENABLE
      case ABP_ETN_IA_ENABLE_ADMIN_MODE:

         /*
         ** Copy the 7th Instance 1 attribute (Enable admin mode) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_ADMIN_MODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_ADMIN_MODE_DS );
         break;
#endif
#if ETN_IA_NETWORK_STATUS_ENABLE
      case ABP_ETN_IA_NETWORK_STATUS:

         /*
         ** Copy the 8th Instance 1 attribute (Network Status) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, etn_iNetworkStatus, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_NETWORK_STATUS_DS );
         break;
#endif
#if ETN_IA_PORT1_MAC_ADDRESS_ENABLE
      case ABP_ETN_IA_PORT1_MAC_ADDRESS:
      {
         UINT16 i;
         UINT8  abMac[ 6 ];

         /*
         ** Copy the 9th Instance 1 attribute (Port 1 MAC Address) to the message.
         */

         ABCC_PORT_MemCpy( abMac, ETN_IA_PORT1_MAC_ADDRESS_VALUE, ABP_ETN_IA_PORT1_MAC_ADDRESS_DS );

         for( i = 0; i < ABP_ETN_IA_PORT1_MAC_ADDRESS_DS; i++ )
         {
            ABCC_SetMsgData8( psNewMessage, abMac[ i ], i );
         }
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_PORT1_MAC_ADDRESS_DS );
         break;
      }
#endif
#if ETN_IA_PORT2_MAC_ADDRESS_ENABLE
      case ABP_ETN_IA_PORT2_MAC_ADDRESS:
      {
         UINT16 i;
         UINT8  abMac[ 6 ];

         /*
         ** Copy the 10th Instance 1 attribute (Port 2 MAC Address) to the message.
         */

         ABCC_PORT_MemCpy( abMac, ETN_IA_PORT2_MAC_ADDRESS_VALUE, ABP_ETN_IA_PORT2_MAC_ADDRESS_DS );

         for( i = 0; i < ABP_ETN_IA_PORT2_MAC_ADDRESS_DS; i++ )
         {
            ABCC_SetMsgData8( psNewMessage, abMac[ i ], i );
         }
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_PORT2_MAC_ADDRESS_DS );
         break;
      }
#endif
#if ETN_IA_ENABLE_ACD_ENABLE
      case ABP_ETN_IA_ENABLE_ACD:

         /*
         ** Copy the 11th Instance 1 attribute (Enable ACD) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_ACD_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_ACD_DS );
         break;
#endif
#if ETN_IA_PORT1_STATE_ENABLE
      case ABP_ETN_IA_PORT1_STATE:

         /*
         ** Copy the 12th Instance 1 attribute (Port 1 state) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_PORT1_STATE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_PORT1_STATE_DS );
         break;
#endif
#if ETN_IA_PORT2_STATE_ENABLE
      case ABP_ETN_IA_PORT2_STATE:

         /*
         ** Copy the 13th Instance 1 attribute (Port 2 state) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_PORT2_STATE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_PORT2_STATE_DS );
         break;
#endif
#if ETN_IA_ENABLE_WEB_UPDATE_ENABLE
      case ABP_ETN_IA_ENABLE_WEB_UPDATE:

         /*
         ** Copy the 14th Instance 1 attribute (Enable web update) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_WEB_UPDATE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_WEB_UPDATE_DS );
         break;
#endif
#if ETN_IA_ENABLE_HICP_RESET_ENABLE
      case ABP_ETN_IA_ENABLE_HICP_RESET:

         /*
         ** Copy the 15th Instance 1 attribute (Enable HICP reset) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_HICP_RESET_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_HICP_RESET_DS );
         break;
#endif
#if ETN_IA_IP_CONFIGURATION_ENABLE
      case ABP_ETN_IA_IP_CONFIGURATION:
      {
         UINT16 i;

         /*
         ** Copy the 16th Instance 1 attribute (IP configuration) to the message.
         */

         i = 0;
         while( i < ABP_ETN_IA_IP_CONFIGURATION_DS )
         {
            ABCC_SetMsgData32( psNewMessage, etn_sIpConfig.alIpConfig[ ( i / ABP_UINT32_SIZEOF ) ], i );
            i += ABP_UINT32_SIZEOF;
         }
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_IP_CONFIGURATION_DS );
         break;
      }
#endif
#if ETN_IA_IP_ADDRESS_BYTE_0_2_ENABLE
      case ABP_ETN_IA_IP_ADDRESS_BYTE_0_2:
      {
         UINT16 i;
         UINT8  abIp[ 3 ];

         /*
         ** Copy the 17th Instance 1 attribute (IP address byte 0 to 2) to the message.
         */

         ABCC_PORT_MemCpy( abIp, ETN_IA_IP_ADDRESS_BYTE_0_2_VALUE, ABP_ETN_IA_IP_ADDRESS_BYTE_0_2_DS );

         for( i = 0; i < ABP_ETN_IA_IP_ADDRESS_BYTE_0_2_DS; i++ )
         {
            ABCC_SetMsgData8( psNewMessage, abIp[ i ], i );
         }
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_IP_ADDRESS_BYTE_0_2_DS );
         break;
      }
#endif
#if ETN_IA_ETH_PHY_CONFIG_ENABLE
      case ABP_ETN_IA_ETH_PHY_CONFIG:
      {
         /*
         ** Copy the 18th Instance 1 attribute (Ethernet PHY Configuration) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, ETN_IA_ETH_PHY_CONFIG_PORT1_VALUE, 0 );
         ABCC_SetMsgData16( psNewMessage, ETN_IA_ETH_PHY_CONFIG_PORT2_VALUE, 2 );
         ABP_SetMsgResponse( psNewMessage, 4 );
         break;
      }
#endif
#if ETN_IA_SNMP_READ_ONLY_ENABLE
      case ABP_ETN_IA_SNMP_READ_ONLY:
      {
         /*
         ** Copy the 20th Instance 1 attribute (SNMP read-only community string) to the message.
         */
         pcStr = ETN_IA_SNMP_READ_ONLY_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_ETN_IA_SNMP_READ_ONLY_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#if ETN_IA_SNMP_READ_WRITE_ENABLE
      case ABP_ETN_IA_SNMP_READ_WRITE:
      {
         /*
         ** Copy the 21st Instance 1 attribute (SNMP read-write community string) to the message.
         */
         pcStr = ETN_IA_SNMP_READ_WRITE_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_ETN_IA_SNMP_READ_WRITE_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#if ETN_IA_DHCP_OPTION_61_SOURCE_ENABLE
      case ABP_ETN_IA_DHCP_OPTION_61_SOURCE:
      {
         /*
         ** Copy the 22nd Instance 1 attribute (DHCP Option 61 source) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_DHCP_OPTION_61_SOURCE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_DHCP_OPTION_61_SOURCE_DS );
         break;
      }
#endif
#if ETN_IA_DHCP_OPTION_61_GENERIC_STR_ENABLE
      case ABP_ETN_IA_DHCP_OPTION_61_GENERIC_STR:
      {
         UINT16  iIndex;
         /*
         ** Copy the 23rd Instance 1 attribute (DHCP Option 61 generic string) to the message.
         */
         for( iIndex = 0; iIndex < ETN_IA_DHCP_OPTION_61_GENERIC_STR_ARRAY_SIZE; iIndex++ )
         {
            ABCC_SetMsgData8( psNewMessage, ETN_IA_DHCP_OPTION_61_GENERIC_STR_VALUE[ iIndex ], iIndex );
         }
         ABP_SetMsgResponse( psNewMessage, ETN_IA_DHCP_OPTION_61_GENERIC_STR_ARRAY_SIZE );
         break;
      }
#endif
#if ETN_IA_ENABLE_DHCP_CLIENT_ENABLE
      case ABP_ETN_IA_ENABLE_DHCP_CLIENT:
      {
         /*
         ** Copy the 24th Instance 1 attribute (Enable DHCP client) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_DHCP_CLIENT_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_DHCP_CLIENT_DS );
         break;
      }
#endif
#if ETN_IA_ENABLE_WEBDAV_ENABLE
      case ABP_ETN_IA_ENABLE_WEBDAV:
      {
         /*
         ** Copy the 25th Instance 1 attribute (Enable WebDAV Server) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, ETN_IA_ENABLE_WEBDAV_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ETN_IA_ENABLE_WEBDAV_DS );
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

   case ABP_CMD_SET_ATTR:

      switch( bAttribute )
      {
#if ETN_IA_MAC_ADDRESS_ENABLE
      case ABP_ETN_IA_MAC_ADDRESS:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_ENABLE_HICP_ENABLE
      case ABP_ETN_IA_ENABLE_HICP:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_ENABLE_WEB_ENABLE
      case ABP_ETN_IA_ENABLE_WEB:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_ENABLE_WEB_ADI_ACCESS_ENABLE
      case ABP_ETN_IA_ENABLE_WEB_ADI_ACCESS:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_ENABLE_FTP_ENABLE
      case ABP_ETN_IA_ENABLE_FTP:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_ENABLE_ADMIN_MODE_ENABLE
      case ABP_ETN_IA_ENABLE_ADMIN_MODE:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_NETWORK_STATUS_ENABLE
      case ABP_ETN_IA_NETWORK_STATUS:
      {
         /*
         ** Set the 8th Instance 1 attribute (Network Status) to the message.
         */

         if( ABCC_GetMsgDataSize( psNewMessage ) > ABP_ETN_IA_NETWORK_STATUS_DS )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );
            break;
         }
         else if( ABCC_GetMsgDataSize( psNewMessage ) < ABP_ETN_IA_NETWORK_STATUS_DS )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_NOT_ENOUGH_DATA );
            break;
         }

         ABCC_GetMsgData16( psNewMessage, &etn_iNetworkStatus, 0 );
#if ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK
         ETN_SetAttrSuccessCallback( iInstance, bAttribute );
#endif
         ABP_SetMsgResponse( psNewMessage, 0 );
         break;
      }
#endif
#if ETN_IA_PORT1_MAC_ADDRESS_ENABLE
      case ABP_ETN_IA_PORT1_MAC_ADDRESS:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_PORT2_MAC_ADDRESS_ENABLE
      case ABP_ETN_IA_PORT2_MAC_ADDRESS:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_ENABLE_ACD_ENABLE
      case ABP_ETN_IA_ENABLE_ACD:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_PORT1_STATE_ENABLE
      case ABP_ETN_IA_PORT1_STATE:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_PORT2_STATE_ENABLE
      case ABP_ETN_IA_PORT2_STATE:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_ENABLE_WEB_UPDATE_ENABLE
      case ABP_ETN_IA_ENABLE_WEB_UPDATE:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_ENABLE_HICP_RESET_ENABLE
      case ABP_ETN_IA_ENABLE_HICP_RESET:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_IP_CONFIGURATION_ENABLE
      case ABP_ETN_IA_IP_CONFIGURATION:
      {
         UINT16 i;

         /*
         ** Set the 16th Instance 1 attribute (IP configuration) to the message.
         */

         if( ABCC_GetMsgDataSize( psNewMessage ) > ABP_ETN_IA_IP_CONFIGURATION_DS )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );
            break;
         }
         else if( ABCC_GetMsgDataSize( psNewMessage ) < ABP_ETN_IA_IP_CONFIGURATION_DS )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_NOT_ENOUGH_DATA );
            break;
         }

         i = 0;
         while( i < ABP_ETN_IA_IP_CONFIGURATION_DS )
         {
            ABCC_GetMsgData32( psNewMessage, &etn_sIpConfig.alIpConfig[ ( i / ABP_UINT32_SIZEOF ) ], i );
            i += ABP_UINT32_SIZEOF;
         }
#if ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK
         ETN_SetAttrSuccessCallback( iInstance, bAttribute );
#endif
         ABP_SetMsgResponse( psNewMessage, 0 );
         break;
      }
#endif
#if ETN_IA_IP_ADDRESS_BYTE_0_2_ENABLE
      case ABP_ETN_IA_IP_ADDRESS_BYTE_0_2:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if ETN_IA_ETH_PHY_CONFIG_ENABLE
      case ABP_ETN_IA_ETH_PHY_CONFIG:
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
      }
#endif
#if ETN_IA_SNMP_READ_ONLY_ENABLE
      case ABP_ETN_IA_SNMP_READ_ONLY:
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
      }
#endif
#if ETN_IA_SNMP_READ_WRITE_ENABLE
      case ABP_ETN_IA_SNMP_READ_WRITE:
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
      }
#endif
#if ETN_IA_DHCP_OPTION_61_SOURCE_ENABLE
      case ABP_ETN_IA_DHCP_OPTION_61_SOURCE:
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
      }
#endif
#if ETN_IA_DHCP_OPTION_61_GENERIC_STR_ENABLE
      case ABP_ETN_IA_DHCP_OPTION_61_GENERIC_STR:
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
      }
#endif
#if ETN_IA_ENABLE_DHCP_CLIENT_ENABLE
      case ABP_ETN_IA_ENABLE_DHCP_CLIENT:
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
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

   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   }
}

/*------------------------------------------------------------------------------
** Processes commands to the ETN Object (Instance 0)
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
   /*
   ** This function processes commands to the Ethernet Object (Instance 0).
   */
   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         /*
         ** Copy the attribute to a message.
         */
         iStrLength = (UINT16)strlen( etn_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, etn_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }

      case ABP_OA_REV:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData8( psNewMessage, etn_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, etn_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, etn_sObject.iHighestInstanceNo, 0 );
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
   }

   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( Command number ) */
}

#endif /* ETN_OBJ_ENABLE */
