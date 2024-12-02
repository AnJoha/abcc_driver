/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 390fce4 (2024-10-25)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**    ABCC Driver    edc67ee (2024-10-25)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2019-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** This is an example implementation of the application abcc handler.
** It includes the following section:
** 1. ADI definition and default mappings
** 2. ABCC driver callback implementations (process data, default mapping ...)
** 3. State machine implementation for message sequencing, e.g. during user init
**    or reading exception state
** 4. ABCC handler main function to be called from the main loop, including a main
**    state machine for handling startup and restart of the ABCC.
********************************************************************************
*/

#include <stdio.h>
#include "abcc_types.h"
#include "abp.h"
#include "abcc.h"
#include "abcc_command_sequencer_interface.h"
#include "abcc_port.h"

#include "application_data_object.h"   /* Application data object:   254 */
#include "application_object.h"        /* Application object:        255 */
#include "application_abcc_handler.h"
#include "abcc_port.h"
#include "ethernet_object.h"
#include "opcua_object.h"
#include "mqtt_object.h"
#include "sync_object.h"
#include "safety_object.h"
#include "assembly_mapping_object.h"
#include "anybus_file_system_interface_object.h"

#include "ethernetip_object.h"
#include "profinet_io_object.h"
#include "ethernet_powerlink_object.h"
#include "profibus_dpv1_object.h"
#include "ethercat_object.h"
#include "devicenet_object.h"
#include "modbus_object.h"
#include "canopen_object.h"
#include "cclink_object.h"
#include "cclink_ie_field_network_object.h"
#include "bacnet_object.h"
#include "cclink_ie_tsn_object.h"

#include "abcc_object_config.h"
#include "application_data_instance_config.h"

/*------------------------------------------------------------------------------
** Maximum start up time when the module is upgrading its firmware
**------------------------------------------------------------------------------
*/
#define APPL_FW_UPGRADE_STARTUP_TIME_MS     ( 2 * 60 * (UINT32)1000 )

/*------------------------------------------------------------------------------
** Default IP configuration when using HW switches
**------------------------------------------------------------------------------
*/
#ifndef APPL_DEFAULT_IP_NETWORK_ADDRESS
#define APPL_DEFAULT_IP_NETWORK_ADDRESS         { 192, 168, 0, 0 }
#endif

#ifndef APPL_DEFAULT_NETMASK
#define APPL_DEFAULT_NETMASK                    { 255, 255, 255, 0 }
#endif

#ifndef APPL_DEFAULT_GATEWAY
#define APPL_DEFAULT_GATEWAY                    { 0, 0, 0, 0 }
#endif

#ifndef APPL_DEFAULT_DHCP_ENABLE
#define APPL_DEFAULT_DHCP_ENABLE                { FALSE }
#endif

/*------------------------------------------------------------------------------
** Comm settings values
**------------------------------------------------------------------------------
*/
#define APPL_COMM_SETTING_1_INSTANCE            7
#define APPL_COMM_SETTING_2_INSTANCE            8

#ifndef APPL_DEFAULT_COMM_SETTING
#define APPL_DEFAULT_COMM_SETTING               APPL_COMM_SETTING_AUTO
#endif

/*------------------------------------------------------------------------------
** The basic IPv4 settings.
**------------------------------------------------------------------------------
*/
typedef struct
{
   UINT16   iInstance;
   union
   {
      UINT8 abValue[ 4 ];
      BOOL  fValue;
   }
   uValue;
}
appl_IpElementType;

typedef struct
{
   appl_IpElementType  sAddress;
   appl_IpElementType  sNetmask;
   appl_IpElementType  sGateway;
   appl_IpElementType  sDhcp;
}
appl_IpSettingsType;

/*------------------------------------------------------------------------------
** ABCC Handler states
**------------------------------------------------------------------------------
*/
typedef enum appl_AbccHandlerState
{
   APPL_INIT,
   APPL_WAITCOM,
   APPL_WAIT_FW_UPDATE,
   APPL_RUN,
   APPL_SHUTDOWN,
   APPL_ABCCRESET,
   APPL_DEVRESET,
   APPL_HALT
}
appl_AbccHandlerStateType;

/*------------------------------------------------------------------------------
** Current state of the ABCC
**------------------------------------------------------------------------------
*/
static appl_AbccHandlerStateType appl_eAbccHandlerState = APPL_INIT;

/*------------------------------------------------------------------------------
** Current anybus state
**------------------------------------------------------------------------------
*/
static volatile ABP_AnbStateType appl_eAnbState = ABP_ANB_STATE_SETUP;

/*------------------------------------------------------------------------------
** Network Node address.
**------------------------------------------------------------------------------
*/
#if APPL_USE_16BIT_NODE_ADDR
static UINT16 appl_iNwNodeAddress;
#else
static UINT8 appl_bNwNodeAddress;
#endif

/*------------------------------------------------------------------------------
** Network IP address settings
**------------------------------------------------------------------------------
*/
static appl_IpSettingsType appl_sIpSettings =
{
   { 3, { APPL_DEFAULT_IP_NETWORK_ADDRESS } },
   { 4, { APPL_DEFAULT_NETMASK } },
   { 5, { APPL_DEFAULT_GATEWAY } },
   { 6, { APPL_DEFAULT_DHCP_ENABLE } }
};

/*------------------------------------------------------------------------------
** Common address variables (used for both node address and IP address depending
** on network type).
**------------------------------------------------------------------------------
*/
static BOOL appl_fSetAddr = FALSE;
static BOOL appl_fSetAddrInProgress = FALSE;

/*------------------------------------------------------------------------------
** Network baud rate.
**------------------------------------------------------------------------------
*/
static UINT8 appl_bNwBaudRate;
static BOOL  appl_fSetBaudRate = FALSE;
static BOOL  appl_fSetBaudRateInProgress = FALSE;

/*------------------------------------------------------------------------------
** Comm setting variables. Used for setting speed and duplex on ethernet based
** networks.
**------------------------------------------------------------------------------
*/
static APPL_CommSettingType appl_eCommSettings1 = APPL_DEFAULT_COMM_SETTING;
static APPL_CommSettingType appl_eCommSettings2 = APPL_DEFAULT_COMM_SETTING;
static BOOL appl_fSetCommSettings = FALSE;
static BOOL appl_fSetCommSettingsInProgress = FALSE;

/*------------------------------------------------------------------------------
** Flags to keep track of if the network type supports node ID/IP address,
** baud rate and/or Comm settings to be set.
**------------------------------------------------------------------------------
*/
static BOOL appl_fNwSupportsNodeId;
static BOOL appl_fNwSupportsBaudRate;
static BOOL appl_fNwSupportsDhcp;
static BOOL appl_fNwSupportsCommSettings;

/*------------------------------------------------------------------------------
** Set to TRUE when an unexpected error occur. The main state machine will
** return APPL_MODULE_UNEXPECTED_ERROR when this flag is set.
**------------------------------------------------------------------------------
*/
static BOOL appl_fUnexpectedError = FALSE;

/*------------------------------------------------------------------------------
** Set to TRUE the user init sequence is done.
**------------------------------------------------------------------------------
*/
static BOOL appl_fUserInitDone = FALSE;

/*------------------------------------------------------------------------------
** Event flags used by application to invoke the corresponding
** ABCC_Trigger<event_action> function from the desired context. The flag will
** be set to TRUE in ABCC_CbfEvent().
**------------------------------------------------------------------------------
*/
static volatile BOOL appl_fMsgReceivedEvent = FALSE;
static volatile BOOL appl_fRdPdReceivedEvent = FALSE;
static volatile BOOL appl_fTransmitMsgEvent = FALSE;
static volatile BOOL appl_fAbccStatusEvent = FALSE;

/*------------------------------------------------------------------------------
** Forward declarations
**------------------------------------------------------------------------------
*/

static ABCC_CmdSeqRespStatusType HandleExceptionResp( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqRespStatusType HandleExceptionInfoResp( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType ReadExeption( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType ReadExeptionInfo( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType UpdateNodeAddress( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType UpdateIpAddress( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType UpdateNetmask( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType UpdateGateway( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType UpdateDhcp( ABP_MsgType* psMsg, void* pxUserData );
static void UpdateAddressDone( const ABCC_CmdSeqResultType eSeqResult, void* pxUserData );
static ABCC_CmdSeqCmdStatusType UpdateBaudRate( ABP_MsgType* psMsg, void* pxUserData );
static void UpdateBaudRateDone( const ABCC_CmdSeqResultType eSeqResult, void* pxUserData );
static ABCC_CmdSeqCmdStatusType UpdateCommSetting1( ABP_MsgType* psMsg, void* pxUserData );
static ABCC_CmdSeqCmdStatusType UpdateCommSetting2( ABP_MsgType* psMsg, void* pxUserData );
static void UpdateCommSettingsDone( const ABCC_CmdSeqResultType eSeqResult, void* pxUserData );

/*------------------------------------------------------------------------------
** User init sequence. See abcc_command_sequencer_interface.h
**------------------------------------------------------------------------------
*/
static const ABCC_CmdSeqType appl_asUserInitCmdSeq[] =
{
   ABCC_CMD_SEQ( UpdateIpAddress, NULL ),
   ABCC_CMD_SEQ( UpdateNetmask, NULL ),
   ABCC_CMD_SEQ( UpdateGateway, NULL ),
   ABCC_CMD_SEQ( UpdateDhcp, NULL ),
   ABCC_CMD_SEQ( UpdateNodeAddress, NULL ),
   ABCC_CMD_SEQ( UpdateBaudRate, NULL ),
   ABCC_CMD_SEQ( UpdateCommSetting1, NULL ),
   ABCC_CMD_SEQ( UpdateCommSetting2, NULL ),
   ABCC_CMD_SEQ_END()
};

/*------------------------------------------------------------------------------
** Set IP or node address.
** Sequence triggered when the address switch has changed value.
** See abcc_command_sequencer_interface.h.
**------------------------------------------------------------------------------
*/
static const ABCC_CmdSeqType appl_asAddressChangedCmdSeq[] =
{
   ABCC_CMD_SEQ( UpdateIpAddress, NULL ),
   ABCC_CMD_SEQ( UpdateNodeAddress, NULL ),
   ABCC_CMD_SEQ_END()
};

/*------------------------------------------------------------------------------
** Set baud rate.
** Sequence triggered when the baud rate switch has changed value.
** See abcc_command_sequencer_interface.h.
**------------------------------------------------------------------------------
*/
static const ABCC_CmdSeqType appl_asBaudRateChangedCmdSeq[] =
{
   ABCC_CMD_SEQ( UpdateBaudRate, NULL ),
   ABCC_CMD_SEQ_END()
};

/*------------------------------------------------------------------------------
** Set Comm settings.
** Sequence triggered when the Comm settings has changed value.
** See abcc_command_sequencer_interface.h.
**------------------------------------------------------------------------------
*/
static const ABCC_CmdSeqType appl_asCommSettingsChangedCmdSeq[] =
{
   ABCC_CMD_SEQ( UpdateCommSetting1, NULL ),
   ABCC_CMD_SEQ( UpdateCommSetting2, NULL ),
   ABCC_CMD_SEQ_END()
};

/*------------------------------------------------------------------------------
** Read exception info.
** Sequence triggered when the anybus state changes to exception state.
** See abcc_command_sequencer_interface.h.
**------------------------------------------------------------------------------
*/
static const ABCC_CmdSeqType appl_asReadExeptionCmdSeq[] =
{
   ABCC_CMD_SEQ( ReadExeption,     HandleExceptionResp ),
   ABCC_CMD_SEQ( ReadExeptionInfo, HandleExceptionInfoResp ),
   ABCC_CMD_SEQ_END()
};

/*------------------------------------------------------------------------------
**  Builds the command for reading the exception code (get attribute single).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType ReadExeption( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   ABCC_GetAttribute( psMsg, ABP_OBJ_NUM_ANB, 1, ABP_ANB_IA_EXCEPTION, ABCC_GetNewSourceId() );
   return( ABCC_CMDSEQ_CMD_SEND );
}

/*------------------------------------------------------------------------------
**  Builds the command for reading the exception info code (get attribute
**  single).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType ReadExeptionInfo( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   ABCC_GetAttribute( psMsg, ABP_OBJ_NUM_NW, 1, ABP_NW_IA_EXCEPTION_INFO, ABCC_GetNewSourceId() );
   return( ABCC_CMDSEQ_CMD_SEND );
}

/*------------------------------------------------------------------------------
**  Handles the exception code response (get attribute single).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqRespHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqRespStatusType HandleExceptionResp( ABP_MsgType* psMsg, void* pxUserData )
{
   UINT8 bException;
   (void)pxUserData;

   if( ABCC_VerifyMessage( psMsg ) != ABCC_EC_NO_ERROR )
   {
      APPL_UnexpectedError();
      return( ABCC_CMDSEQ_RESP_EXEC_NEXT );
   }

   ABCC_GetMsgData8( psMsg, &bException, 0 );
   ABCC_DebugPrintf( "Exception Code: %X:\n\n", bException );

   (void)bException;
   return( ABCC_CMDSEQ_RESP_EXEC_NEXT );
}

/*------------------------------------------------------------------------------
**  Handles the exception info code response (get attribute single).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqRespHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqRespStatusType HandleExceptionInfoResp( ABP_MsgType* psMsg, void* pxUserData )
{
   UINT8 bExceptionInfo;
   (void)pxUserData;

   if( ABCC_VerifyMessage( psMsg ) != ABCC_EC_NO_ERROR )
   {
      APPL_UnexpectedError();
      return( ABCC_CMDSEQ_RESP_EXEC_NEXT );
   }

   ABCC_GetMsgData8( psMsg, &bExceptionInfo, 0 );
   ABCC_DebugPrintf( "Exception Info: %X:\n\n", bExceptionInfo );

   (void)bExceptionInfo;
   return( ABCC_CMDSEQ_RESP_EXEC_NEXT );
}

/*------------------------------------------------------------------------------
**  Notification that the address is updated.
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqDoneHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static void UpdateAddressDone( const ABCC_CmdSeqResultType eSeqResult, void* pxUserData )
{
   (void)pxUserData;

   appl_fSetAddrInProgress = FALSE;

   switch( eSeqResult )
   {
   case ABCC_CMDSEQ_RESULT_COMPLETED:
      break;

   case ABCC_CMDSEQ_RESULT_ABORT_INT:
      ABCC_DebugPrintf( "UpdateAddressDone reported internally aborted command sequence.\n" );
      break;

   case ABCC_CMDSEQ_RESULT_ABORT_EXT:
      ABCC_DebugPrintf( "UpdateAddressDone reported externally aborted command sequence.\n" );
      break;

   default:
      ABCC_DebugPrintf( "UpdateAddressDone reported aborted command sequence.\n" );
      break;
   }
}

/*------------------------------------------------------------------------------
**  Builds the command for setting the ip-address (set attribute).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType UpdateIpAddress( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   if( ( !appl_fNwSupportsNodeId ) &&
       ( appl_fSetAddr ) )
   {
      ABCC_SetMsgHeader( psMsg,
                         ABP_OBJ_NUM_NC,
                         appl_sIpSettings.sAddress.iInstance,
                         ABP_NC_VAR_IA_VALUE,
                         ABP_CMD_SET_ATTR,
                         4,
                         ABCC_GetNewSourceId() );

      ABCC_SetMsgString( psMsg, (char*)appl_sIpSettings.sAddress.uValue.abValue, 4, 0 );

      return( ABCC_CMDSEQ_CMD_SEND );
   }

   return( ABCC_CMDSEQ_CMD_SKIP );
}

/*------------------------------------------------------------------------------
**  Builds the command for setting the netmask (set attribute).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType UpdateNetmask( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   if( ( !appl_fNwSupportsNodeId ) &&
       ( appl_fSetAddr ) )
   {
      ABCC_SetMsgHeader( psMsg,
                         ABP_OBJ_NUM_NC,
                         appl_sIpSettings.sNetmask.iInstance,
                         ABP_NC_VAR_IA_VALUE,
                         ABP_CMD_SET_ATTR,
                         4,
                         ABCC_GetNewSourceId() );

      ABCC_SetMsgString( psMsg, (char*)appl_sIpSettings.sNetmask.uValue.abValue, 4, 0 );

      return( ABCC_CMDSEQ_CMD_SEND );
   }

   return( ABCC_CMDSEQ_CMD_SKIP );
}

/*------------------------------------------------------------------------------
**  Builds the command for setting the gateway (set attribute).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType UpdateGateway( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   if( ( !appl_fNwSupportsNodeId ) &&
       ( appl_fSetAddr ) )
   {
      ABCC_SetMsgHeader( psMsg,
                         ABP_OBJ_NUM_NC,
                         appl_sIpSettings.sGateway.iInstance,
                         ABP_NC_VAR_IA_VALUE,
                         ABP_CMD_SET_ATTR,
                         4,
                         ABCC_GetNewSourceId() );

      ABCC_SetMsgString( psMsg, (char*)appl_sIpSettings.sGateway.uValue.abValue, 4, 0 );

      return( ABCC_CMDSEQ_CMD_SEND );
   }

   return( ABCC_CMDSEQ_CMD_SKIP );
}

/*------------------------------------------------------------------------------
**  Builds the command for enable/disable dhcp (set attribute).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType UpdateDhcp( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   if( ( appl_fNwSupportsDhcp ) &&
       ( appl_fSetAddr ) )
   {
      ABCC_SetByteAttribute( psMsg, ABP_OBJ_NUM_NC,
                             appl_sIpSettings.sDhcp.iInstance,
                             ABP_NC_VAR_IA_VALUE,
                             (UINT8)appl_sIpSettings.sDhcp.uValue.fValue,
                             ABCC_GetNewSourceId() );

      return( ABCC_CMDSEQ_CMD_SEND );
   }
   return( ABCC_CMDSEQ_CMD_SKIP );
}

/*------------------------------------------------------------------------------
**  Builds the command for setting the node-address (set attribute).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType UpdateNodeAddress( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   if( ( appl_fNwSupportsNodeId ) &&
       ( appl_fSetAddr ) )
   {
#if APPL_USE_16BIT_NODE_ADDR
      ABCC_SetMsgHeader( psMsg,
                         ABP_OBJ_NUM_NC,
                         ABP_NC_INST_NUM_SW1,
                         ABP_NC_VAR_IA_VALUE,
                         ABP_CMD_SET_ATTR,
                         ABP_UINT16_SIZEOF,
                         ABCC_GetNewSourceId() );

      ABCC_SetMsgData16( psMsg, appl_iNwNodeAddress, 0 );
#else
      ABCC_SetByteAttribute( psMsg, ABP_OBJ_NUM_NC,
                             ABP_NC_INST_NUM_SW1,
                             ABP_NC_VAR_IA_VALUE,
                             appl_bNwNodeAddress,
                             ABCC_GetNewSourceId() );
#endif

      return( ABCC_CMDSEQ_CMD_SEND );
   }

   return( ABCC_CMDSEQ_CMD_SKIP );
}

/*------------------------------------------------------------------------------
**  Notification that the Comm settings are updated.
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqDoneHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static void UpdateCommSettingsDone( const ABCC_CmdSeqResultType eSeqResult, void* pxUserData )
{
   (void)pxUserData;

   appl_fSetCommSettingsInProgress = FALSE;

   switch( eSeqResult )
   {
   case ABCC_CMDSEQ_RESULT_COMPLETED:
      break;

   case ABCC_CMDSEQ_RESULT_ABORT_INT:
      ABCC_DebugPrintf( "UpdateCommSettingsDone reported internally aborted command sequence.\n" );
      break;

   case ABCC_CMDSEQ_RESULT_ABORT_EXT:
      ABCC_DebugPrintf( "UpdateCommSettingsDone reported externally aborted command sequence.\n" );
      break;

   default:
      ABCC_DebugPrintf( "UpdateCommSettingsDone reported aborted command sequence.\n" );
      break;
   }
}

/*------------------------------------------------------------------------------
**  Builds the command for Comm settings.
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType SetCommSettingCmdMsgData( ABP_MsgType* psMsg,
                                                          UINT16 iInstance,
                                                          APPL_CommSettingType eCommSetting )
{
   if( ( appl_fNwSupportsCommSettings ) &&
       ( appl_fSetCommSettings ) )
   {
      ABCC_SetByteAttribute( psMsg,
                             ABP_OBJ_NUM_NC,
                             iInstance,
                             ABP_NC_VAR_IA_VALUE,
                             (UINT8)eCommSetting,
                             ABCC_GetNewSourceId() );
      return( ABCC_CMDSEQ_CMD_SEND );
   }

   return( ABCC_CMDSEQ_CMD_SKIP );
}

/*------------------------------------------------------------------------------
**  Builds the command for Comm 1 setting.
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType UpdateCommSetting1( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   return( SetCommSettingCmdMsgData( psMsg, APPL_COMM_SETTING_1_INSTANCE, appl_eCommSettings1 ) );
}

/*------------------------------------------------------------------------------
**  Builds the command for Comm 2 setting.
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType UpdateCommSetting2( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   return( SetCommSettingCmdMsgData( psMsg, APPL_COMM_SETTING_2_INSTANCE, appl_eCommSettings2 ) );
}

/*------------------------------------------------------------------------------
**  Notification that the baud rate is updated.
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqDoneHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static void UpdateBaudRateDone( const ABCC_CmdSeqResultType eSeqResult, void* pxUserData )
{
   (void)pxUserData;

   appl_fSetBaudRateInProgress = FALSE;

   switch( eSeqResult )
   {
   case ABCC_CMDSEQ_RESULT_COMPLETED:
      break;

   case ABCC_CMDSEQ_RESULT_ABORT_INT:
      ABCC_DebugPrintf( "UpdateBaudRateDone reported internally aborted command sequence.\n" );
      break;

   case ABCC_CMDSEQ_RESULT_ABORT_EXT:
      ABCC_DebugPrintf( "UpdateBaudRateDone reported externally aborted command sequence.\n" );
      break;

   default:
      ABCC_DebugPrintf( "UpdateBaudRateDone reported aborted command sequence.\n" );
      break;
   }
}

/*------------------------------------------------------------------------------
**  Builds the command for setting the baud rate (set attribute).
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqCmdHandler type in cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType UpdateBaudRate( ABP_MsgType* psMsg, void* pxUserData )
{
   (void)pxUserData;

   if( ( appl_fNwSupportsBaudRate ) &&
       ( appl_fSetBaudRate ) )
   {
      ABCC_SetByteAttribute( psMsg, ABP_OBJ_NUM_NC,
                             ABP_NC_INST_NUM_SW2,
                             ABP_NC_VAR_IA_VALUE,
                             appl_bNwBaudRate,
                             ABCC_GetNewSourceId() );

      return( ABCC_CMDSEQ_CMD_SEND );
   }
   return( ABCC_CMDSEQ_CMD_SKIP );
}

/*------------------------------------------------------------------------------
**  Notification that the user init sequence is done.
**
**  This function is a part of a command sequence. See description of
**  ABCC_CmdSeqDoneHandler type in abcc_cmd_seq_if.h
**------------------------------------------------------------------------------
*/
static void UserInitDone( const ABCC_CmdSeqResultType eSeqResult, void* pxUserData )
{
   (void)pxUserData;

   switch( eSeqResult )
   {

   case ABCC_CMDSEQ_RESULT_COMPLETED:
      appl_fUserInitDone = TRUE;
      ABCC_UserInitComplete();
      break;

   case ABCC_CMDSEQ_RESULT_ABORT_INT:
      ABCC_DebugPrintf( "UserInitDone reported internally aborted command sequence.\n" );
      APPL_UnexpectedError();
      break;

   case ABCC_CMDSEQ_RESULT_ABORT_EXT:
      ABCC_DebugPrintf( "UserInitDone reported externally aborted command sequence.\n" );
      APPL_UnexpectedError();
      break;

   default:
      ABCC_DebugPrintf( "UserInitDone reported aborted command sequence.\n" );
      APPL_UnexpectedError();
      break;
   }
}

APPL_AbccHandlerStatusType APPL_HandleAbcc( void )
{
   static APPL_AbccHandlerStatusType eModuleStatus = APPL_MODULE_NO_ERROR;
   UINT32 lStartupTimeMs;
   ABCC_CommunicationStateType eAbccComState;

   switch( appl_eAbccHandlerState )
   {
   case APPL_INIT:

      eModuleStatus = APPL_MODULE_NO_ERROR;
      appl_fMsgReceivedEvent = FALSE;
      appl_fRdPdReceivedEvent = FALSE;
      appl_fTransmitMsgEvent = FALSE;
      appl_fAbccStatusEvent = FALSE;
      appl_fUserInitDone = FALSE;

      if( !ABCC_ModuleDetect() )
      {
         eModuleStatus = APPL_MODULE_NOT_DETECTED;
      }

      if( eModuleStatus == APPL_MODULE_NO_ERROR )
      {
         /*
         ** Init application data object
         */
         if( AD_Init( APPL_asAdiEntryList,
                      APPL_GetNumAdi(),
                      APPL_asAdObjDefaultMap ) != APPL_NO_ERROR )
         {
            eModuleStatus = APPL_MODULE_UNEXPECTED_ERROR;
         }
#if ASM_OBJ_ENABLE
         /*
         ** Init Assembly mapping object
         */
         ASM_Init();
#endif
#if ANB_FSI_OBJ_ENABLE
         ANB_FSI_Init();
#endif
      }

      if( eModuleStatus == APPL_MODULE_NO_ERROR )
      {
#if APP_OBJ_ENABLE
         if( APP_GetCandidateFwAvailable() == TRUE )
         {
            ABCC_DebugPrintf( "Waiting for firmware update to complete.\n" );
            lStartupTimeMs = APPL_FW_UPGRADE_STARTUP_TIME_MS;
         }
         else
#endif
         {
            /*
            ** Default time will be used
            */
            lStartupTimeMs = 0;
         }

         if( ABCC_StartDriver( lStartupTimeMs ) == ABCC_EC_NO_ERROR )
         {
            ABCC_HWReleaseReset();
            appl_eAbccHandlerState = APPL_WAITCOM;
         }
         else
         {
            eModuleStatus = APPL_MODULE_NOT_ANSWERING;
         }
      }

      if( eModuleStatus != APPL_MODULE_NO_ERROR )
      {
         appl_eAbccHandlerState = APPL_HALT;
      }

      break;

   case APPL_WAITCOM:
   case APPL_WAIT_FW_UPDATE:

      eAbccComState = ABCC_isReadyForCommunication();

      if( eAbccComState == ABCC_READY_FOR_COMMUNICATION )
      {
         appl_eAbccHandlerState = APPL_RUN;
      }
      else if( eAbccComState == ABCC_STARTUP_TIMEOUT )
      {
         appl_eAbccHandlerState = APPL_HALT;
         eModuleStatus = APPL_MODULE_NOT_ANSWERING;
         ABCC_DebugPrintf( "ABCC did not start within the expected time\n" );
      }
#if ABCC_CFG_DRV_ASSUME_FW_UPDATE_ENABLED
      else if( eAbccComState == ABCC_ASSUME_FW_UPDATE )
      {
         appl_eAbccHandlerState = APPL_WAIT_FW_UPDATE;
         ABCC_DebugPrintf( "ABCC did not start within the expected time. Assume firmware update is ongoing. \n" \
                           "Now waiting an additional %d ms\n", APPL_FW_UPGRADE_STARTUP_TIME_MS );
         ABCC_WaitForFwUpdate( APPL_FW_UPGRADE_STARTUP_TIME_MS );
      }
#endif
      break;

   case APPL_RUN:

      /*------------------------------------------------------------------------
      ** Handle events indicated in ABCC_CbfEvent()callback function.
      ** Note that these events could be handled from any chosen context but in
      ** in this application it is done from main loop context.
      **------------------------------------------------------------------------
      */
      if( appl_fRdPdReceivedEvent )
      {
         appl_fRdPdReceivedEvent = FALSE;
         ABCC_TriggerRdPdUpdate();
      }

      if( appl_fMsgReceivedEvent )
      {
         appl_fMsgReceivedEvent = FALSE;
         ABCC_TriggerReceiveMessage();
      }

      if( appl_fTransmitMsgEvent )
      {
         appl_fTransmitMsgEvent = FALSE;
         ABCC_TriggerTransmitMessage();
      }

      if( appl_fAbccStatusEvent )
      {
         appl_fAbccStatusEvent = FALSE;
         ABCC_TriggerAnbStatusUpdate();
      }
      /*
      ** End event handling.
      */

#if SYNC_OBJ_ENABLE
      if( SYNC_GetMode() == SYNC_MODE_NONSYNCHRONOUS )
      {
         ABCC_TriggerWrPdUpdate();
      }
#else
      /*
      ** Always update write process data
      */
      ABCC_TriggerWrPdUpdate();
#endif

      ABCC_RunDriver();

      APPL_CyclicalProcessing();

      break;

   case APPL_SHUTDOWN:

      ABCC_HWReset();
      eModuleStatus = APPL_MODULE_SHUTDOWN;
      appl_eAbccHandlerState = APPL_HALT;

      break;

   case APPL_ABCCRESET:

      ABCC_HWReset();
      appl_eAbccHandlerState = APPL_INIT;
      eModuleStatus = APPL_MODULE_NO_ERROR;
      break;

   case APPL_DEVRESET:

      ABCC_HWReset();
      eModuleStatus = APPL_MODULE_RESET;
      appl_eAbccHandlerState = APPL_HALT;

      break;

   case APPL_HALT:

      break;

   default:

      break;
   }

   if( appl_fUnexpectedError )
   {
      return( APPL_MODULE_UNEXPECTED_ERROR );
   }

   return( eModuleStatus );
}

#if APPL_USE_16BIT_NODE_ADDR
void APPL_SetAddress( UINT16 iSwitchValue )
{
   appl_fSetAddr = TRUE;

   if( appl_fSetAddrInProgress )
   {
      /*
      ** Address updated next time
      */
      return;
   }

   if( appl_fUserInitDone == FALSE )
   {
      /*
      ** HW switch 1 will the last octet in the IP address
      ** for applicable networks ( 192.168.0.X )
      */
      appl_sIpSettings.sAddress.uValue.abValue[ 3 ] = (UINT8)iSwitchValue;

      /*
      ** Switch 1 is node address for applicable networks
      */
      appl_iNwNodeAddress = iSwitchValue;

      /*
      ** Indicate to application object that the address is set by HW switches
      */
   #if APP_OBJ_ENABLE
      APP_HwConfAddress( TRUE );
   #endif
   }
   else if( appl_iNwNodeAddress != iSwitchValue )
   {
     /*
     ** Start command sequence to update address if the value has changed.
     */
     appl_sIpSettings.sAddress.uValue.abValue[ 3 ] = (UINT8)iSwitchValue;
     appl_iNwNodeAddress = iSwitchValue;
     appl_fSetAddrInProgress = TRUE;
     ABCC_CmdSeqAdd( appl_asAddressChangedCmdSeq, UpdateAddressDone, NULL, NULL );
   }
}
#else
void APPL_SetAddress( UINT8 bSwitchValue )
{
   appl_fSetAddr = TRUE;

   if( appl_fSetAddrInProgress )
   {
      /*
      ** Address updated next time
      */
      return;
   }

   if( appl_fUserInitDone == FALSE )
   {
      /*
      ** HW switch 1 will the last octet in the IP address
      ** for applicable networks ( 192.168.0.X )
      */
      appl_sIpSettings.sAddress.uValue.abValue[ 3 ] = bSwitchValue;

      /*
      ** Switch 1 is node address for applicable networks
      */
      appl_bNwNodeAddress = bSwitchValue;

      /*
      ** Indicate to application object that the address is set by HW switches
      */
   #if APP_OBJ_ENABLE
      APP_HwConfAddress( TRUE );
   #endif
   }
   else if( appl_bNwNodeAddress != bSwitchValue )
   {
     /*
     ** Start command sequence to update address if the value has changed.
     */
     appl_sIpSettings.sAddress.uValue.abValue[ 3 ] = bSwitchValue;
     appl_bNwNodeAddress = bSwitchValue;
     appl_fSetAddrInProgress = TRUE;
     ABCC_CmdSeqAdd( appl_asAddressChangedCmdSeq, UpdateAddressDone, NULL, NULL );
   }
}
#endif /* #if APPL_USE_16BIT_NODE_ADDR */

void APPL_SetBaudrate( UINT8 bSwitchValue )
{
   if( appl_fSetBaudRateInProgress )
   {
      /*
      ** Baud rate updated next time
      */
      return;
   }

   if( appl_fUserInitDone == FALSE )
   {
      appl_bNwBaudRate = bSwitchValue;
   }
   else if( appl_bNwBaudRate != bSwitchValue )
   {
      /*
      ** Start command sequence to update baud rate if the value has changed.
      */
      appl_bNwBaudRate = bSwitchValue;
      ABCC_CmdSeqAdd( appl_asBaudRateChangedCmdSeq, UpdateBaudRateDone, NULL, NULL );
   }
   appl_fSetBaudRate = TRUE;
}

void APPL_SetCommSettings( APPL_CommSettingType eCommSettings1,
                           APPL_CommSettingType eCommSettings2 )
{
   if( appl_fSetCommSettingsInProgress )
   {
      /*
      ** Comm settings updated next time
      */
      return;
   }

   if( appl_fUserInitDone == FALSE )
   {
      appl_eCommSettings1 = eCommSettings1;
      appl_eCommSettings2 = eCommSettings2;
   }
   else if( ( appl_eCommSettings1 != eCommSettings1 ) ||
            ( appl_eCommSettings2 != eCommSettings2 ) )
   {
      /*
      ** Start command sequence to update Comm settings if values have changed.
      */
      appl_eCommSettings1 = eCommSettings1;
      appl_eCommSettings2 = eCommSettings2;
      appl_fSetCommSettingsInProgress = TRUE;
      ABCC_CmdSeqAdd( appl_asCommSettingsChangedCmdSeq, UpdateCommSettingsDone, NULL, NULL );
   }
   appl_fSetCommSettings = TRUE;
}

void APPL_UnexpectedError( void )
{
   appl_fUnexpectedError = TRUE;
}

void APPL_RestartAbcc( void )
{
   appl_eAbccHandlerState = APPL_ABCCRESET;
}

void APPL_Shutdown( void )
{
   appl_eAbccHandlerState = APPL_SHUTDOWN;
}


void APPL_Reset( void )
{
   appl_eAbccHandlerState = APPL_DEVRESET;
}

UINT16  ABCC_CbfAdiMappingReq( const AD_AdiEntryType**  const ppsAdiEntry,
                               const AD_MapType** const ppsDefaultMap )
{
   return( AD_AdiMappingReq( ppsAdiEntry, ppsDefaultMap ) );
}

BOOL ABCC_CbfUpdateWriteProcessData( void* pxWritePd )
{

   /*
   ** AD_UpdatePdWriteData is a general function that updates all ADI:s according
   ** to current map.
   ** If the ADI mapping is fixed there is potential for doing that in a more
   ** optimized way, for example by using memcpy.
   */
   return( AD_UpdatePdWriteData( pxWritePd ) );
}

#if( ABCC_CFG_REMAP_SUPPORT_ENABLED )
void ABCC_CbfRemapDone( void )
{
   AD_RemapDone();
}
#endif

void ABCC_CbfNewReadPd( void* pxReadPd )
{
   /*
   ** AD_UpdatePdReadData is a general function that updates all ADI:s according
   ** to current map.
   ** If the ADI mapping is fixed there is potential for doing that in a more
   ** optimized way, for example by using memcpy.
   */
   AD_UpdatePdReadData( pxReadPd );
}

void ABCC_CbfDriverError( ABCC_SeverityType eSeverity, ABCC_ErrorCodeType iErrorCode, UINT32 lAddInfo )
{
   switch( eSeverity )
   {
      case ABCC_SEV_FATAL:
      case ABCC_SEV_WARNING:
         APPL_UnexpectedError();
         break;
      default:
         break;
   }

   (void)iErrorCode;
   (void)lAddInfo;
}

void ABCC_CbfReceiveMsg( ABP_MsgType* psReceivedMsg )
{
   switch(  ABCC_GetMsgDestObj( psReceivedMsg ) )
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

void ABCC_CbfWdTimeout( void )
{
   ABCC_DebugPrintf( "ABCC watchdog timeout\n" );

#if ABCC_CFG_DRV_ASSUME_FW_UPDATE_ENABLED
   if( ABCC_IsFirstCommandPending() )
   {
      if( ABCC_WaitForFwUpdate( APPL_FW_UPGRADE_STARTUP_TIME_MS ) )
      {
         appl_eAbccHandlerState = APPL_WAIT_FW_UPDATE;
         ABCC_DebugPrintf( "Failed to establish communication within the expected time. Assume firmware \n" \
                           "update is ongoing. Now waiting an additional %d ms\n", APPL_FW_UPGRADE_STARTUP_TIME_MS );
      }
   }
#endif
}

void ABCC_CbfWdTimeoutRecovered( void )
{
   ABCC_DebugPrintf( "ABCC watchdog recovered\n" );
}

#if ABCC_CFG_SYNC_ENABLED
void ABCC_CbfSyncIsr( void )
{
   /*
   ** Call application specific handling of sync event
   */
   APPL_SyncIsr();
}
#endif

void ABCC_CbfEvent( UINT16 iEvents )
{

   /*
   ** Set flag to indicate that an event has occurred and the corresponding
   ** ABCC_Trigger<event_action> must be called. In the sample code the the
   ** trigger function is called from main loop context.
   */
   if( iEvents & ABCC_ISR_EVENT_RDPD )
   {
      appl_fRdPdReceivedEvent = TRUE;
   }

   if( iEvents & ABCC_ISR_EVENT_RDMSG )
   {
      appl_fMsgReceivedEvent = TRUE;
   }

   if( iEvents & ABCC_ISR_EVENT_WRMSG )
   {
      appl_fTransmitMsgEvent = TRUE;
   }

   if( iEvents & ABCC_ISR_EVENT_STATUS  )
   {
      appl_fAbccStatusEvent = TRUE;
   }
}

void ABCC_CbfAnbStateChanged( ABP_AnbStateType eNewAnbState )
{
   static const char* AnbStateString[ 8 ] =
   {
      "ABP_ANB_STATE_SETUP",
      "ABP_ANB_STATE_NW_INIT",
      "ABP_ANB_STATE_WAIT_PROCESS",
      "ABP_ANB_STATE_IDLE",
      "ABP_ANB_STATE_PROCESS_ACTIVE",
      "ABP_ANB_STATE_ERROR",
      "",
      "ABP_ANB_STATE_EXCEPTION"
   };
   (void)AnbStateString[ 0 ];

   appl_eAnbState = eNewAnbState;
   ABCC_DebugPrintf( "ANB_STATUS: %s \n",
                     AnbStateString[ appl_eAnbState ] );

   switch( appl_eAnbState )
   {
   case ABP_ANB_STATE_PROCESS_ACTIVE:

      ABCC_TriggerWrPdUpdate();
      break;

   case ABP_ANB_STATE_EXCEPTION:

      /* Trigger message sequence for reading exception data */
      ABCC_CmdSeqAdd( appl_asReadExeptionCmdSeq, NULL, NULL, NULL );
      break;

   case ABP_ANB_STATE_ERROR:

      /* Trigger message sequence for reading exception data */
      break;

   default:

      break;
   }
}

void ABCC_CbfUserInitReq( void )
{
   UINT16 iNetworkType;

   iNetworkType = ABCC_NetworkType();

   if( ( iNetworkType == ABP_NW_TYPE_DEV ) ||
       ( iNetworkType == ABP_NW_TYPE_PDPV0 ) ||
       ( iNetworkType == ABP_NW_TYPE_PDPV1 ) ||
       ( iNetworkType == ABP_NW_TYPE_COP ) ||
       ( iNetworkType == ABP_NW_TYPE_CNT ) ||
       ( iNetworkType == ABP_NW_TYPE_CCL ) ||
       ( iNetworkType == ABP_NW_TYPE_CFN ) ||
       ( iNetworkType == ABP_NW_TYPE_CPN ) ||
       ( iNetworkType == ABP_NW_TYPE_ECT ) ||
       ( iNetworkType == ABP_NW_TYPE_EPL ) )
   {
      appl_fNwSupportsNodeId = TRUE;
      appl_fNwSupportsDhcp = FALSE;
   }
   else
   {
      appl_fNwSupportsNodeId = FALSE;
      appl_fNwSupportsDhcp = TRUE;
   }

   if( ( iNetworkType == ABP_NW_TYPE_DEV ) ||
       ( iNetworkType == ABP_NW_TYPE_COP ) ||
       ( iNetworkType == ABP_NW_TYPE_CCL ) )
   {
      appl_fNwSupportsBaudRate = TRUE;
   }
   else
   {
      appl_fNwSupportsBaudRate = FALSE;
   }

   if( iNetworkType == ABP_NW_TYPE_CIET )
   {
      appl_fNwSupportsCommSettings = TRUE;
      appl_fNwSupportsDhcp = FALSE;
   }
   else
   {
      appl_fNwSupportsCommSettings = FALSE;
   }

   /*
   ** Start user init command sequence
   */
   ABCC_CmdSeqAdd( appl_asUserInitCmdSeq, UserInitDone, NULL, NULL );

}
