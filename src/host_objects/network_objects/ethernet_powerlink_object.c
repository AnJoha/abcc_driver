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
** Source file for the Ethernet POWERLINK ABCC Object.
********************************************************************************
*/

#include "abcc_types.h"
#include "abcc.h"
#include "abcc_hardware_abstraction.h"
#include "abcc_object_config.h"
#include "ethernet_powerlink_object.h"
#include "abp.h"
#include "abp_epl.h"
#include "string.h"
#include "application_abcc_handler.h"
#include "abcc_port.h"

#if EPL_OBJ_ENABLE

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define EPL_OA_NAME_VALUE                          "POWERLINK"
#define EPL_OA_REV_VALUE                           1
#define EPL_OA_NUM_INST_VALUE                      1
#define EPL_OA_HIGHEST_INST_VALUE                  1

/*------------------------------------------------------------------------------
** Structure describing a Powerlink Object.
**------------------------------------------------------------------------------
*/
typedef struct epl_Object
{
    const  char* pcName;
    UINT8  bRevision;
    UINT16 iNumberOfInstances;
    UINT16 iHighestInstanceNo;
}
epl_ObjectType;

static const epl_ObjectType epl_sObject =
{
   EPL_OA_NAME_VALUE,                           /* Name.                                              */
   EPL_OA_REV_VALUE,                            /* Revision.                                          */
   EPL_OA_NUM_INST_VALUE,                       /* Number of instances.                               */
   EPL_OA_HIGHEST_INST_VALUE                    /* Highest instance number.                           */
};

/*------------------------------------------------------------------------------
** Processes commands to EPL Instances
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
   ** This function processes commands to the Ethernet POWERLINK Instance.
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
#if EPL_IA_VENDOR_ID_ENABLE
      case ABP_EPL_IA_VENDOR_ID:

         /*
         ** Copy the 1st Instance 1 attribute (Vendor ID) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, EPL_IA_VENDOR_ID_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_EPL_IA_VENDOR_ID_DS );
         break;
#endif
#if EPL_IA_PRODUCT_CODE_ENABLE
      case ABP_EPL_IA_PRODUCT_CODE:

         /*
         ** Copy the 2nd Instance 1 attribute (Product code) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, EPL_IA_PRODUCT_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_EPL_IA_PRODUCT_CODE_DS );
         break;
#endif
#if EPL_IA_REVISION_HW_ENABLE
      case ABP_EPL_IA_MAJOR_REV:

         /*
         ** Copy the 3rd Instance 1 attribute (Revision high word) to the
         ** message.
         */
         ABCC_SetMsgData16( psNewMessage, EPL_IA_REVISION_HW_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_EPL_IA_MAJOR_REV_DS );
         break;
#endif
#if EPL_IA_REVISION_LW_ENABLE
      case ABP_EPL_IA_MINOR_REV:

         /*
         ** Copy the 4th Instance 1 attribute (Revision low word) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, EPL_IA_REVISION_LW_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_EPL_IA_MINOR_REV_DS );
         break;
#endif
#if EPL_IA_SERIAL_NUMBER_ENABLE
      case ABP_EPL_IA_SERIAL_NUMBER:

         /*
         ** Copy the 5th Instance 1 attribute (Serial number) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, EPL_IA_SERIAL_NUMBER_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_EPL_IA_SERIAL_NUMBER_DS );
         break;
#endif
#if EPL_IA_MANF_DEVICE_NAME_ENABLE
      case ABP_EPL_IA_MANF_DEV_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( EPL_IA_MANF_DEVICE_NAME_VALUE );

         /*
         ** Copy the 6th Instance 1 attribute (Manufacturer device name) to the
         ** message.
         */
         ABCC_SetMsgString( psNewMessage, EPL_IA_MANF_DEVICE_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
#endif
#if EPL_IA_MANF_HW_VERSION_ENABLE
      case ABP_EPL_IA_MANF_HW_VER:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( EPL_IA_MANF_HW_VERSION_VALUE );


         /*
         ** Copy the 7th Instance 1 attribute (Manufacturer hardware version) to
         ** the message.
         */
         ABCC_SetMsgString( psNewMessage, EPL_IA_MANF_HW_VERSION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
      }
      break;
#endif
#if EPL_IA_MANF_SW_VERSION_ENABLE
      case ABP_EPL_IA_MANF_SW_VER:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( EPL_IA_MANF_SW_VERSION_VALUE );

         /*
         ** Copy the 8th Instance 1 attribute (Manufacturer software version) to
         ** the message.
         */
         ABCC_SetMsgString( psNewMessage, EPL_IA_MANF_SW_VERSION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
      }
      break;
#endif
#if EPL_IA_DEVICE_TYPE_ENABLE
      case ABP_EPL_IA_DEVICE_TYPE:

         /*
         ** Copy the 10th Instance 1 attribute (Device type) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, EPL_IA_DEVICE_TYPE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_EPL_IA_DEVICE_TYPE_DS );
         break;
#endif
#if EPL_IA_MANF_NAME_ENABLE
      case ABP_EPL_IA_MANF_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( EPL_IA_MANF_NAME_VALUE );

         /*
         ** Copy the 14th Instance 1 attribute (Manufacturer name) to the
         ** message.
         */
         ABCC_SetMsgString( psNewMessage, EPL_IA_MANF_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
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
   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   }
}

/*------------------------------------------------------------------------------
** Processes commands to the EPL Object (Instance 0)
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
   ** This function processes commands to the Ethernet POWERLINK Object
   ** (Instance 0).
   */
   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:

      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( EPL_OA_NAME_VALUE );

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgString( psNewMessage, EPL_OA_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:

         /*
         ** Copy the attribute to a message.
         */

         ABCC_SetMsgData8( psNewMessage, epl_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, epl_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, epl_sObject.iHighestInstanceNo, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

      } /* End of switch( Attribute number ) */

      break;

   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( Command number ) */
}

void EPL_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Ethernet POWERLINK Object and
   ** its Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** Ethernet POWERLINK object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Ethernet POWERLINK instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

#endif
