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
** Source file for the Profinet IO object.
********************************************************************************
*/

#include "abcc_types.h"
#include "abcc_hardware_abstraction.h"
#include "abcc_object_config.h"
#include "abcc.h"
#include "profinet_io_object.h"
#include "abp_pnio.h"
#include "string.h"
#include "application_abcc_handler.h"
#include "abcc_port.h"

#if PRT_OBJ_ENABLE

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define PRT_OA_NAME_VALUE                          "PROFINET IO"
#define PRT_OA_REV_VALUE                           1
#define PRT_OA_NUM_INST_VALUE                      1
#define PRT_OA_HIGHEST_INST_VALUE                  1

/*------------------------------------------------------------------------------
** prt_ObjectType.
** Structure describing an ProfiNet Object.
**------------------------------------------------------------------------------
*/
typedef struct prt_ObjectType
{
    const  char* pcName;
    UINT8  bRevision;
    UINT16 iNumberOfInstances;
    UINT16 iHighestInstanceNo;
}
prt_ObjectType;

/*------------------------------------------------------------------------------
** Forward declarations
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage );
static void ObjectCommand( ABP_MsgType* psNewMessage );

static const prt_ObjectType prt_sObject =
{
   PRT_OA_NAME_VALUE,                           /* Name.                                              */
   PRT_OA_REV_VALUE,                            /* Revision.                                          */
   PRT_OA_NUM_INST_VALUE,                       /* Number of instances.                               */
   PRT_OA_HIGHEST_INST_VALUE                    /* Highest instance number.                           */
};


#if PRT_IA_S2_PRIMARY_AR_HANDLE_ENABLE
/*
** Storage space for "iS2PrimaryARHandle"
*/
static UINT16 iS2PrimaryARHandle = 0;
#endif

void PRT_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Profinet Object and it's Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** Profinet object Command
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Profinet instance Command
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

/*------------------------------------------------------------------------------
** Processes commands to PRT Instances
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
   /*
   ** This function processes commands to the Profinet Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) != 1 )
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

      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if PRT_IA_DEVICE_ID_ENABLE
      case ABP_PNIO_IA_DEVICE_ID:

         /*
         ** Copy the 1st Instance 1 attribute (Device ID) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, PRT_IA_DEVICE_ID_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_DEVICE_ID_DS );
         break;
#endif
#if PRT_IA_VENDOR_ID_ENABLE
      case ABP_PNIO_IA_VENDOR_ID:

         /*
         ** Copy the 2nd Instance 1 attribute (Vendor ID) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, PRT_IA_VENDOR_ID_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_VENDOR_ID_DS );
         break;
#endif
#if PRT_IA_STATION_TYPE_ENABLE
      case ABP_PNIO_IA_STATION_TYPE:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( PRT_IA_STATION_TYPE_VALUE );

         /*
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */
         if( iStrLength > ABP_PNIO_IA_STATION_TYPE_DS )
         {
            iStrLength = ABP_PNIO_IA_STATION_TYPE_DS;
         }

         /*
         ** Copy the 3rd Instance 1 attribute (Station Type) to the message.
         */
         ABCC_SetMsgString( psNewMessage, PRT_IA_STATION_TYPE_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#if PRT_IA_MAX_AR_ENABLE
      case ABP_PNIO_IA_MAX_AR:

         /*
         ** Copy the 4th Instance 1 attribute (MaxAr) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, PRT_IA_MAX_AR_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_MAX_AR_DS );
         break;
#endif
#if PRT_IA_RTM_ENABLE
      case ABP_PNIO_IA_RTM:

         /*
         ** Copy the 7th Instance 1 attribute (Record Data Transparent Mode) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, PRT_IA_RTM_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_RTM_DS );
         break;
#endif
#if PRT_IA_IM_ORDER_ID_ENABLE
      case ABP_PNIO_IA_IM_ORDER_ID:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( PRT_IA_IM_ORDER_ID_VALUE );

         /*
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */
         if( iStrLength > ABP_PNIO_IA_IM_ORDER_ID_DS )
         {
            iStrLength = ABP_PNIO_IA_IM_ORDER_ID_DS;
         }

         /*
         ** Copy the 8th Instance 1 attribute (IM Order ID) to the message.
         */
         ABCC_SetMsgString( psNewMessage, PRT_IA_IM_ORDER_ID_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#if PRT_IA_IM_SERIAL_NBR_ENABLE
      case ABP_PNIO_IA_IM_SERIAL_NBR:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( PRT_IA_IM_SERIAL_NBR_VALUE );

         /*
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */
         if( iStrLength > ABP_PNIO_IA_IM_SERIAL_NBR_DS )
         {
            iStrLength = ABP_PNIO_IA_IM_SERIAL_NBR_DS;
         }

         /*
         ** Copy the 9th Instance 1 attribute (IM Serial number) to the message.
         */
         ABCC_SetMsgString( psNewMessage, PRT_IA_IM_SERIAL_NBR_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#if PRT_IA_IM_HW_REV_ENABLE
      case ABP_PNIO_IA_IM_HW_REV:

         /*
         ** Copy the 10th Instance 1 attribute (IM Hardware revision) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, PRT_IA_IM_HW_REV_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_IM_HW_REV_DS );
         break;
#endif
#if PRT_IA_IM_SW_REV_ENABLE
      case ABP_PNIO_IA_IM_SW_REV:

         /*
         ** Copy the 11th Instance 1 attribute (Software revision) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, PRT_IA_IM_SW_REV_SYMBOL_VALUE, 0 );
         ABCC_SetMsgData8( psNewMessage, PRT_IA_IM_SW_REV_MAJOR_VALUE, 1 );
         ABCC_SetMsgData8( psNewMessage, PRT_IA_IM_SW_REV_MINOR_VALUE, 2 );
         ABCC_SetMsgData8( psNewMessage, PRT_IA_IM_SW_REV_BUILD_VALUE, 3 );

         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_IM_SW_REV_DS );
         break;
#endif
#if PRT_IA_IM_REV_CNT_ENABLE
      case ABP_PNIO_IA_IM_REV_CNT:

         /*
         ** Copy the 12th Instance 1 attribute (IM Revision Counter) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, PRT_IA_IM_REV_CNT_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_IM_REV_CNT_DS );
         break;
#endif
#if PRT_IA_IM_PROFILE_ID_ENABLE
      case ABP_PNIO_IA_IM_PROFILE_ID:

         /*
         ** Copy the 13th Instance 1 attribute (IM Profile ID) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, PRT_IA_IM_PROFILE_ID_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_IM_PROFILE_ID_DS );
         break;
#endif
#if PRT_IA_IM_PROFILE_SPEC_TYPE_ENABLE
      case ABP_PNIO_IA_IM_PROFILE_SPEC_TYPE:

         /*
         ** Copy the 14th Instance 1 attribute (IM Profile Specific Type) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, PRT_IA_IM_PROFILE_SPEC_TYPE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_IM_PROFILE_SPEC_TYPE_DS );
         break;
#endif
#if PRT_IA_PORT1_MAC_ADDRESS_ENABLE
      case ABP_PNIO_IA_PORT1_MAC_ADDRESS:
      {
         UINT16 i;
         UINT8  abMac[ 6 ];

         /*
         ** Copy the 17th Instance 1 attribute (Port 1 MAC Address) to the message.
         */

         ABCC_PORT_MemCpy( abMac, PRT_IA_PORT1_MAC_ADDRESS_VALUE, 6 );

         for( i = 0; i < 6; i++ )
         {
            ABCC_SetMsgData8( psNewMessage, abMac[ i ], i );
         }
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_PORT1_MAC_ADDRESS_DS );
         break;
      }
#endif
#if PRT_IA_PORT2_MAC_ADDRESS_ENABLE
      case ABP_PNIO_IA_PORT2_MAC_ADDRESS:
      {
         UINT16 i;
         UINT8  abMac[ 6 ];

         /*
         ** Copy the 18th Instance 1 attribute (Port 2 MAC Address) to the message.
         */

         ABCC_PORT_MemCpy( abMac, PRT_IA_PORT2_MAC_ADDRESS_VALUE, 6 );

         for( i = 0; i < 6; i++ )
         {
            ABCC_SetMsgData8( psNewMessage, abMac[ i ], i );
         }
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_PORT2_MAC_ADDRESS_DS );
         break;
      }
#endif
#if PRT_IA_SYSTEM_DESCRIPTION_ENABLE
      case ABP_PNIO_IA_SYSTEM_DESCRIPTION:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( PRT_IA_SYSTEM_DESCRIPTION_VALUE );

         /*
         ** Copy the 19th Instance 1 attribute (System description) to the message.
         */
         ABCC_SetMsgString( psNewMessage, PRT_IA_SYSTEM_DESCRIPTION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#if PRT_IA_SYSTEM_CONTACT_ENABLE
      case ABP_PNIO_IA_SYSTEM_CONTACT:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( PRT_IA_SYSTEM_CONTACT_VALUE );

         /*
         ** Copy the 22nd Instance 1 attribute (System contact) to the message.
         */
         ABCC_SetMsgString( psNewMessage, PRT_IA_SYSTEM_CONTACT_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#if PRT_IA_IM_MODULE_ORDER_ID_ENABLE
      case ABP_PNIO_IA_IM_MODULE_ORDER_ID:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strnlen( PRT_IA_IM_MODULE_ORDER_ID_VALUE, ABP_PNIO_IA_IM_MODULE_ORDER_ID_DS );

         /*
         ** Copy the 25th Instance 1 attribute (IM5 Module Order ID) to the message.
         */
         ABCC_SetMsgString( psNewMessage, PRT_IA_IM_MODULE_ORDER_ID_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#if PRT_IA_IM_ANNOTATION_ENABLE
      case ABP_PNIO_IA_IM_ANNOTATION:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strnlen( PRT_IA_IM_ANNOTATION_VALUE, ABP_PNIO_IA_IM_ANNOTATION_ID_DS );

         /*
         ** Copy the 26th Instance 1 attribute (IM5 Annotation) to the message.
         */
         ABCC_SetMsgString( psNewMessage, PRT_IA_IM_ANNOTATION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#if PRT_IA_IM5_ENABLED_ENABLE
      case ABP_PNIO_IA_IM5_ENABLED:

         /*
         ** Copy the 27th Instance 1 attribute (IM5 Enabled) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, PRT_IA_IM5_ENABLED_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_IM5_ENABLE_DS );
         break;
#endif
#if PRT_IA_S2_ENABLED_ENABLE
      case ABP_PNIO_IA_S2_ENABLED:

         /*
         ** Copy the 28th Instance 1 attribute (S2 Enabled) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, PRT_IA_S2_ENABLED_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_S2_ENABLE_DS );
         break;
#endif
#if PRT_IA_S2_PRIMARY_AR_HANDLE_ENABLE
      case ABP_PNIO_IA_S2_PRIMARY_AR_HANDLE:

         /*
         ** 29th Instance 1 attribute (S2 Primary AR Handle)
         ** ABP_ERR_ATTR_NOT_GETABLE response for SET-only attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_GETABLE );
         break;
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
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if PRT_IA_DEVICE_ID_ENABLE
      case ABP_PNIO_IA_DEVICE_ID:
#endif
#if PRT_IA_VENDOR_ID_ENABLE
      case ABP_PNIO_IA_VENDOR_ID:
#endif
#if PRT_IA_STATION_TYPE_ENABLE
      case ABP_PNIO_IA_STATION_TYPE:
#endif
#if PRT_IA_MAX_AR_ENABLE
      case ABP_PNIO_IA_MAX_AR:
#endif
#if PRT_IA_RTM_ENABLE
      case ABP_PNIO_IA_RTM:
#endif
#if PRT_IA_IM_ORDER_ID_ENABLE
      case ABP_PNIO_IA_IM_ORDER_ID:
#endif
#if PRT_IA_IM_SERIAL_NBR_ENABLE
      case ABP_PNIO_IA_IM_SERIAL_NBR:
#endif
#if PRT_IA_IM_HW_REV_ENABLE
      case ABP_PNIO_IA_IM_HW_REV:
#endif
#if PRT_IA_IM_SW_REV_ENABLE
      case ABP_PNIO_IA_IM_SW_REV:
#endif
#if PRT_IA_IM_REV_CNT_ENABLE
      case ABP_PNIO_IA_IM_REV_CNT:
#endif
#if PRT_IA_IM_PROFILE_ID_ENABLE
      case ABP_PNIO_IA_IM_PROFILE_ID:
#endif
#if PRT_IA_IM_PROFILE_SPEC_TYPE_ENABLE
      case ABP_PNIO_IA_IM_PROFILE_SPEC_TYPE:
#endif
#if PRT_IA_PORT1_MAC_ADDRESS_ENABLE
      case ABP_PNIO_IA_PORT1_MAC_ADDRESS:
#endif
#if PRT_IA_PORT2_MAC_ADDRESS_ENABLE
      case ABP_PNIO_IA_PORT2_MAC_ADDRESS:
#endif
#if PRT_IA_SYSTEM_DESCRIPTION_ENABLE
      case ABP_PNIO_IA_SYSTEM_DESCRIPTION:
#endif
#if PRT_IA_SYSTEM_CONTACT_ENABLE
      case ABP_PNIO_IA_SYSTEM_CONTACT:
#endif
#if PRT_IA_IM_MODULE_ORDER_ID_ENABLE
      case ABP_PNIO_IA_IM_MODULE_ORDER_ID:
#endif
#if PRT_IA_IM_ANNOTATION_ENABLE
      case ABP_PNIO_IA_IM_ANNOTATION:
#endif
#if PRT_IA_IM5_ENABLED_ENABLE
      case ABP_PNIO_IA_IM5_ENABLED:
#endif
#if PRT_IA_S2_ENABLED_ENABLE
      case ABP_PNIO_IA_S2_ENABLED:
#endif
#if ( PRT_IA_DEVICE_ID_ENABLE ) ||             \
    ( PRT_IA_VENDOR_ID_ENABLE ) ||             \
    ( PRT_IA_STATION_TYPE_ENABLE ) ||          \
    ( PRT_IA_MAX_AR_ENABLE ) ||                \
    ( PRT_IA_RTM_ENABLE ) ||                   \
    ( PRT_IA_IM_ORDER_ID_ENABLE ) ||           \
    ( PRT_IA_IM_SERIAL_NBR_ENABLE ) ||         \
    ( PRT_IA_IM_HW_REV_ENABLE ) ||             \
    ( PRT_IA_IM_SW_REV_ENABLE ) ||             \
    ( PRT_IA_IM_REV_CNT_ENABLE ) ||            \
    ( PRT_IA_IM_PROFILE_ID_ENABLE ) ||         \
    ( PRT_IA_IM_PROFILE_SPEC_TYPE_ENABLE ) ||  \
    ( PRT_IA_PORT1_MAC_ADDRESS_ENABLE ) ||     \
    ( PRT_IA_PORT2_MAC_ADDRESS_ENABLE ) ||     \
    ( PRT_IA_SYSTEM_DESCRIPTION_ENABLE ) ||    \
    ( PRT_IA_SYSTEM_CONTACT_ENABLE ) ||        \
    ( PRT_IA_IM_MODULE_ORDER_ID_ENABLE ) ||    \
    ( PRT_IA_IM_ANNOTATION_ENABLE ) ||         \
    ( PRT_IA_IM5_ENABLED_ENABLE ) ||           \
    ( PRT_IA_S2_ENABLED_ENABLE )

         /*
         ** ABP_ERR_ATTR_NOT_SETABLE response for all GET-only attributes.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif
#if PRT_IA_S2_PRIMARY_AR_HANDLE_ENABLE
      case ABP_PNIO_IA_S2_PRIMARY_AR_HANDLE:
         /*
         ** PORTING ALERT!!!
         ** If this attribute is going to be used, add additional code here.
         */

         ABCC_GetMsgData16( psNewMessage, &iS2PrimaryARHandle, 0 );
         ABP_SetMsgResponse( psNewMessage, 0 );
         break;
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
** Processes commands to the PRT Object (Instance 0)
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
   ** This function processes commands to the Profinet Object (Instance 0).
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
         iStrLength = (UINT16)strlen( prt_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, prt_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }

      case ABP_OA_REV:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData8( psNewMessage, prt_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, prt_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, prt_sObject.iHighestInstanceNo, 0 );
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
   case ABP_PNIO_CMD_GET_RECORD:

      /*
      ** Optionally implement the Get Record service here.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   case ABP_PNIO_CMD_SET_RECORD:

      /*
      ** Optionally implement the Set Record service here.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   case ABP_PNIO_CMD_GET_IM_RECORD:

      /*
      ** Optionally implement the Get IM Record service here.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   case ABP_PNIO_CMD_SET_IM_RECORD:

      /*
      ** Optionally implement the Set IM Record service here.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   case ABP_PNIO_CMD_AR_CHECK_IND:

      /*
      ** Optionally implement the AR Check Ind. service here.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( Command number ) */
}

#endif /* PRT_OBJ_ENABLE */
