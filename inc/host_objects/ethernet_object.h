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
** This is the public header file for the Ethernet IO object.
********************************************************************************
*/

#ifndef ETN_H
#define ETN_H

#if ETN_IA_IP_CONFIGURATION_ENABLE
/*------------------------------------------------------------------------------
** Structure storing attribute #16 'IP configuration' data
**------------------------------------------------------------------------------
*/
typedef union ETN_IpConfigType
{
   UINT32 alIpConfig[ 3 ];
   struct
   {
      UINT32 lIpAddr;
      UINT32 lSnMask;
      UINT32 lGwAddr;
   }
   sAddr;
}
ETN_IpConfigType;
#endif /* #if ETN_IA_IP_CONFIGURATION_ENABLE */

/*------------------------------------------------------------------------------
** Processes commands sent to the Ethernet object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ETN_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#if ETN_IA_IP_CONFIGURATION_ENABLE
/*------------------------------------------------------------------------------
** Fetch the present content of the IP address cfg. structure.
**------------------------------------------------------------------------------
** Arguments:
**    psIpConfig  - Pointer to structure to store current IP configuration
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ETN_GetIpConfig( ETN_IpConfigType* psIpConfig );
#endif /* #if ETN_IA_IP_CONFIGURATION_ENABLE */

#if ETN_IA_NETWORK_STATUS_ENABLE
/*------------------------------------------------------------------------------
** Fetch the present content of the Network Status word.
**------------------------------------------------------------------------------
** Arguments:
**    piStatus    -  Pointer to storage for the status bitfield.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ETN_GetNwStatus( UINT16* piNwStatus );
#endif /* #if ETN_IA_NETWORK_STATUS_ENABLE */

#if ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK
/*------------------------------------------------------------------------------
** Optional user callback function that needs to be implemented by user
** if ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK is set to 1.
**------------------------------------------------------------------------------
** Arguments:
**    iInstance   - Object instance number
**    bAttribute  - Object instance attribute number
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ETN_SetAttrSuccessCallback( UINT16 iInstance, UINT8 bAttribute );
#endif /* #if ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK */

#endif /* Inclusion lock */
