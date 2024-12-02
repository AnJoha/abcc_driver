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
** Source file for the CC-Link Object.
********************************************************************************
*/

#include "abcc_types.h"
#include "abcc.h"
#include "abcc_object_config.h"
#include "cclink_object.h"
#include "abp.h"
#include "abp_ccl.h"
#include "string.h"
#include "application_abcc_handler.h"
#include "abcc_port.h"

#if CCL_OBJ_ENABLE

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define CCL_OA_NAME_VALUE                          "CC-Link"
#define CCL_OA_REV_VALUE                           1
#define CCL_OA_NUM_INST_VALUE                      1
#define CCL_OA_HIGHEST_INST_VALUE                  1

/*------------------------------------------------------------------------------
** Structure describing an CC-Link Object.
**------------------------------------------------------------------------------
*/
typedef struct ccl_Object
{
   const  char* pcName;
   UINT8  bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
}
ccl_ObjectType;

/*------------------------------------------------------------------------------
** Forward declarations
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage );
static void ObjectCommand( ABP_MsgType* psNewMessage );

static const ccl_ObjectType ccl_sObject =
{
   CCL_OA_NAME_VALUE,                           /* Name.                                              */
   CCL_OA_REV_VALUE,                            /* Revision.                                          */
   CCL_OA_NUM_INST_VALUE,                       /* Number of instances.                               */
   CCL_OA_HIGHEST_INST_VALUE                    /* Highest instance number.                           */
};

void CCL_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the CC-Link Object and it's Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** CC-Link object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** CC-Link instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

/*------------------------------------------------------------------------------
** Processes commands to CCL Instances
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
   ** This function processes commands to the CC-Link Instance.
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
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if CCL_IA_VENDOR_CODE_ENABLE
      case ABP_CCL_IA_VENDOR_CODE:

         /*
         ** Copy the 1st Instance 1 attribute (Vendor Code) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CCL_IA_VENDOR_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CCL_IA_VENDOR_CODE_DS );
         break;
#endif
#if CCL_IA_SOFTWARE_VERSION_ENABLE
      case ABP_CCL_IA_SOFTWARE_VERSION:

         /*
         ** Copy the 2nd Instance 1 attribute (Software Version) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, CCL_IA_SOFTWARE_VERSION_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CCL_IA_SOFTWARE_VERSION_DS );
         break;
#endif
#if CCL_IA_MODEL_CODE_ENABLE
      case ABP_CCL_IA_MODEL_CODE:

         /*
         ** Copy the 3rd Instance 1 attribute (Model Code) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, CCL_IA_MODEL_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CCL_IA_MODEL_CODE_DS );
         break;
#endif
#if CCL_IA_NETWORK_SETTING_ENABLE
      case ABP_CCL_IA_NETWORK_SETTINGS:

         /*
         ** Copy the 4th Instance 1 attribute (Network Setting) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, CCL_IA_CCLINK_VERSION_VALUE, 0 );
         ABCC_SetMsgData8( psNewMessage, CCL_IA_NUM_OF_OCCUPIED_STATION_VALUE, 1 );
         ABCC_SetMsgData8( psNewMessage, CCL_IA_NUM_OF_EXTENSION_CYCLES_VALUE, 2 );
         ABP_SetMsgResponse( psNewMessage, ABP_CCL_IA_NETWORK_SETTINGS_DS );
         break;
#endif
#if CCL_IA_SYSTEM_AREA_HANDLER_ENABLE
      case ABP_CCL_IA_SYS_AREA_HANDLER:

         /*
         ** Copy the 5th Instance 1 attribute (System Area Handler) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CCL_IA_SYSTEM_AREA_HANDLER_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CCL_IA_SYS_AREA_HANDLER_DS );
         break;
#endif
#if CCL_IA_OUTPUT_HOLD_CLEAR_ENABLE
      case ABP_CCL_IA_HOLD_CLEAR_SETTING:

         /*
         ** Copy the 6th Instance 1 attribute (Output Hold/Clear) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CCL_IA_OUTPUT_HOLD_CLEAR_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CCL_IA_HOLD_CLEAR_SETTING_DS );
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
   }
   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( Command number ) */
}

/*------------------------------------------------------------------------------
** Processes commands to the CCL Object (Instance 0)
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
   ** This function processes commands to the CC-Link Object (Instance 0).
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
         iStrLength = (UINT16)strlen( ccl_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, ccl_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }

      case ABP_OA_REV:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData8( psNewMessage, ccl_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, ccl_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, ccl_sObject.iHighestInstanceNo, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_HIGHEST_INST_DS );
         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

      } /* End of switch( Attribute number ) */

      break;
   }
   case ABP_CCL_CMD_INITIAL_DATA_SETTING_NOTIFICATION:

      /*
      ** Optionally implement the Initial_Data_Setting_Notification service here.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   case ABP_CCL_CMD_INITIAL_DATA_PROCESSING_COMPLETED_NOTIFICATION:

      /*
      ** Optionally implement the Initial_Data_Processing_Notification service here.
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

#endif /* CCL_OBJ_ENABLE */
