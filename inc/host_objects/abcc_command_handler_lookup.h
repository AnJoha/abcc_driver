/*******************************************************************************
** Copyright 2018-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Command handler lookup macros and callback function definitions.
********************************************************************************
*/
#include "abcc.h"
#include "abp.h"
#include "abp_eip.h"
#include "abp_pnio.h"

#ifndef ABCC_HOST_ATTR_LOOKUP_H_
#define ABCC_HOST_ATTR_LOOKUP_H_

/*******************************************************************************
** Predefined macros to be registred into the user defined list 
** ABCC_CFG_COMMAND_RESPONSE_LIST to select what commands to be responded to and
** how (callback function of constant value).
**
** To use constat value:
**    The macros ending with _VALUE(x) shall be registred with x replaced by the
**    value of the datatype specified in .uData.
** 
** To use callback function:
**    The macros ending with _CBFUNC shall be registred and the function pointed
**    to by .uCbx shall be implemented.
** 
** While both constant values and callback functions can be mixed in
** ABCC_CFG_COMMAND_RESPONSE_LIST, both types for a specific command cannot be
** registred simultaneously.
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Application Object (0xFF)
**------------------------------------------------------------------------------
*/
/* Object attributes */
#define ABCC_APPLICATION_OBJ_OA_NAME_GET_VALUE(x)       {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_NAME,         .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_STR,    .uData.pacString   = (x)}
#define ABCC_APPLICATION_OBJ_OA_REV_GET_VALUE(x)        {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_REV,          .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT8,  .uData.bUnsigned8  = (x)}
#define ABCC_APPLICATION_OBJ_OA_NR_INST_GET_VALUE(x)    {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_NUM_INST,     .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16 = (x)}
#define ABCC_APPLICATION_OBJ_OA_HI_INST_NR_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_HIGHEST_INST, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16 = (x)}

/* Reset */
#define ABCC_APPLICATION_OBJ_OA_RESET_CBFUNC            {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x00,                                       .bCommand = ABP_CMD_RESET,                                                              .uCbx.pnResetObj = ABCC_ImplApplicationObj_Reset}

/* Serial number */
#define ABCC_APPLICATION_OBJ_SERIAL_NR_GET_VALUE(x)    {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_APP_IA_SER_NUM, .bCommand = ABP_CMD_GET_ATTR,      .eServiceTag = SERVICE_UINT32, .uData.lUnsigned32      = (x)}
#define ABCC_APPLICATION_OBJ_SERIAL_NR_GET_CBFUNC      {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_APP_IA_SER_NUM, .bCommand = ABP_CMD_GET_ATTR,      .eServiceTag = SERVICE_UINT32,                                                           .uCbx.pnGetUint32Attr = ABCC_ImplApplicationObjSerialNum_Get}

/* Vendor name */
#define ABCC_APPLICATION_OBJ_VENDOR_NAME_GET_VALUE(x)  {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_APP_IA_VENDOR_NAME, .bCommand = ABP_CMD_GET_ATTR,  .eServiceTag = SERVICE_STR,    .uData.pacString        = (x)}
#define ABCC_APPLICATION_OBJ_VENDOR_NAME_GET_CBFUNC    {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_APP_IA_VENDOR_NAME, .bCommand = ABP_CMD_GET_ATTR,  .eServiceTag = SERVICE_STR,    .uData.iAttrMaxDataSize = ABP_APP_IA_VENDOR_NAME_MAX_DS,  .uCbx.pnGetStrAttr    = ABCC_ImplApplicationObjVendorName_Get}

/* Product name */
#define ABCC_APPLICATION_OBJ_PRODUCT_NAME_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_APP_IA_PRODUCT_NAME, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_STR,    .uData.pacString        = (x)}
#define ABCC_APPLICATION_OBJ_PRODUCT_NAME_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_APP_IA_PRODUCT_NAME, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_STR,    .uData.iAttrMaxDataSize = ABP_APP_IA_PRODUCT_NAME_MAX_DS, .uCbx.pnGetStrAttr    = ABCC_ImplApplicationObjProductName_Get}

/* Firmware Available flag */
#define ABCC_APPLICATION_OBJ_FW_AVAILABLE_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_APP_IA_FW_AVAILABLE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT8,  .uData.iAttrMaxDataSize = ABP_APP_IA_FW_AVAILABLE_DS,     .uCbx.pnGetUint8Attr  = ABCC_ImplApplicationObjFirmwareAvailable_Get}
#define ABCC_APPLICATION_OBJ_FW_AVAILABLE_SET_CBFUNC   {.bObject = ABP_OBJ_NUM_APP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_APP_IA_FW_AVAILABLE, .bCommand = ABP_CMD_SET_ATTR, .eServiceTag = SERVICE_UINT8,  .uData.iAttrMaxDataSize = ABP_APP_IA_FW_AVAILABLE_DS,     .uCbx.pnSetUint8Attr  = ABCC_ImplApplicationObjFirmwareAvailable_Set}

/*------------------------------------------------------------------------------
** EtherNet/IP Host Object (0xF8)
**------------------------------------------------------------------------------
*/
/* Object attributes */
#define ABCC_ETHERNETIP_OBJ_OA_NAME_GET_VALUE(x)       {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_NAME,         .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_STR,    .uData.pacString   = (x)}
#define ABCC_ETHERNETIP_OBJ_OA_REV_GET_VALUE(x)        {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_REV,          .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT8,  .uData.bUnsigned8  = (x)}
#define ABCC_ETHERNETIP_OBJ_OA_NR_INST_GET_VALUE(x)    {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_NUM_INST,     .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16 = (x)}
#define ABCC_ETHERNETIP_OBJ_OA_HI_INST_NR_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_HIGHEST_INST, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16 = (x)}

/* Vendor ID */
#define ABCC_ETHERNETIP_OBJ_VENDOR_ID_GET_VALUE(x)   {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_EIP_IA_VENDOR_ID, .bCommand = ABP_CMD_GET_ATTR,   .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16   = (x)}
#define ABCC_ETHERNETIP_OBJ_VENDOR_ID_GET_CBFUNC     {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_EIP_IA_VENDOR_ID, .bCommand = ABP_CMD_GET_ATTR,   .eServiceTag = SERVICE_UINT16,                             .uCbx.pnGetUint16Attr = ABCC_ImplEthernetIpObjVendorId_Get}

/* Device Type */
#define ABCC_ETHERNETIP_OBJ_DEVICE_TYPE_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_EIP_IA_DEVICE_TYPE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16   = (x)}
#define ABCC_ETHERNETIP_OBJ_DEVICE_TYPE_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_EIP_IA_DEVICE_TYPE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16,                             .uCbx.pnGetUint16Attr = ABCC_ImplEthernetIpObjDeviceType_Get}

/* Product Code */
#define ABCC_ETHERNETIP_OBJ_PRODUCT_CODE_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_EIP_IA_PRODUCT_CODE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16 = (x)}
#define ABCC_ETHERNETIP_OBJ_PRODUCT_CODE_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_EIP, .bInstance = 0x01, .uCmdExt.bAttr = ABP_EIP_IA_PRODUCT_CODE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16,                           .uCbx.pnGetUint16Attr = ABCC_ImplEthernetIpObjProductCode_Get}

/*------------------------------------------------------------------------------
** PROFINET IO Object (0xF6)
**------------------------------------------------------------------------------
*/
/* Object attributes */
#define ABCC_PROFINET_OBJ_OA_NAME_GET_VALUE(x)       {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_NAME,         .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_STR,    .uData.pacString   = (x)}
#define ABCC_PROFINET_OBJ_OA_REV_GET_VALUE(x)        {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_REV,          .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT8,  .uData.bUnsigned8  = (x)}
#define ABCC_PROFINET_OBJ_OA_NR_INST_GET_VALUE(x)    {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_NUM_INST,     .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16 = (x)}
#define ABCC_PROFINET_OBJ_OA_HI_INST_NR_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x00, .uCmdExt.bAttr = ABP_OA_HIGHEST_INST, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16 = (x)}

/* Device ID */
#define ABCC_PROFINET_OBJ_DEVICE_ID_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x01, .uCmdExt.bAttr = ABP_PNIO_IA_DEVICE_ID,   .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16      = (x)}
#define ABCC_PROFINET_OBJ_DEVICE_ID_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x01, .uCmdExt.bAttr = ABP_PNIO_IA_DEVICE_ID,   .eServiceTag = SERVICE_UINT16,                                                       .uCbx.pnGetUint16Attr = ABCC_ImplProfinetIoObjDeviceId_Get}

/* Vendor ID */
#define ABCC_PROFINET_OBJ_VENDOR_ID_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x01, .uCmdExt.bAttr = ABP_PNIO_IA_VENDOR_ID,   .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16      = (x)}
#define ABCC_PROFINET_OBJ_VENDOR_ID_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x01, .uCmdExt.bAttr = ABP_PNIO_IA_VENDOR_ID,   .eServiceTag = SERVICE_UINT16,                                                       .uCbx.pnGetUint16Attr = ABCC_ImplProfinetIoObjVendorId_Get}

/* Order ID */
#define ABCC_PROFINET_OBJ_ORDER_ID_GET_VALUE(x)  {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x01, .uCmdExt.bAttr = ABP_PNIO_IA_IM_ORDER_ID, .eServiceTag = SERVICE_STR,    .uData.pacString        = (x)}
#define ABCC_PROFINET_OBJ_ORDER_ID_GET_CBFUNC    {.bObject = ABP_OBJ_NUM_PNIO, .bInstance = 0x01, .uCmdExt.bAttr = ABP_PNIO_IA_IM_ORDER_ID, .eServiceTag = SERVICE_STR,    .uData.iAttrMaxDataSize = ABP_PNIO_IA_IM_ORDER_ID_DS, .uCbx.pnGetStrAttr    = ABCC_ImplProfinetIoObjOrderId_Get}

/*******************************************************************************
** Predefined callback function prototypes used by command_handler_lookup_table.
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Application Object (0xFF)
**------------------------------------------------------------------------------
*/
/*------------------------------------------------------------------------------
** Callback function to perform a reset of the device, requested by the network.
**------------------------------------------------------------------------------
** Arguments:
**       eResetType - The type of reset to be performed.
**          ABP_RESET_POWER_ON                 - Power-on reset
**          ABP_RESET_FACTORY_DEFAULT          - Factory default reset
**          ABP_RESET_POWER_ON_FACTORY_DEFAULT - Power-on + Factory reset
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
void ABCC_ImplApplicationObj_Reset( ABP_ResetType eResetType );

/*------------------------------------------------------------------------------
** Callback function to provide the device's serial number to the CompactCom.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Serial number of the device.
**------------------------------------------------------------------------------
*/
UINT32 ABCC_ImplApplicationObjSerialNum_Get( void );

/*------------------------------------------------------------------------------
** Callback function to provide the network-independent vendor name to the 
** CompactCom.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Vendor name of the device.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_ImplApplicationObjVendorName_Get( char* pPackedStrDest, UINT16 iBuffSize );

/*------------------------------------------------------------------------------
** Callback function to provide the network-independent product name to the 
** CompactCom.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Product name of the device.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_ImplApplicationObjProductName_Get( char* pPackedStrDest, UINT16 iBuffSize );

/*------------------------------------------------------------------------------
** Callback function to provide the Firmware Available flag that is stored in
** non-volatile storage on the device.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Firmware Available flag.
**------------------------------------------------------------------------------
*/
UINT8 ABCC_ImplApplicationObjFirmwareAvailable_Get( void );

/*------------------------------------------------------------------------------
** Callback function to set the Firmware Available flag in non-volatile storage
** on the device.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Firmware Available flag.
**          0 - no new firmware image in the CompactCom candidate area.
**          1 - new firmware image in the CompactCom candidate area.
**------------------------------------------------------------------------------
*/
void ABCC_ImplApplicationObjFirmwareAvailable_Set( UINT8 bValue );

/*------------------------------------------------------------------------------
** EtherNet/IP Host Object (0xF8)
**------------------------------------------------------------------------------
*/
/*------------------------------------------------------------------------------
** Callback function to provide the EtherNet/IP Vendor ID to the CompactCom.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       EtherNet/IP Vendor ID of the device.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_ImplEthernetIpObjVendorId_Get( void );

/*------------------------------------------------------------------------------
** Callback function to provide the EtherNet/IP Device Type to the CompactCom.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       EtherNet/IP Device Type of the device.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_ImplEthernetIpObjDeviceType_Get( void );

/*------------------------------------------------------------------------------
** Callback function to provide the EtherNet/IP Product Code to the CompactCom.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       EtherNet/IP Product Code of the device.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_ImplEthernetIpObjProductCode_Get( void );

/*------------------------------------------------------------------------------
** PROFINET IO Object (0xF6)
**------------------------------------------------------------------------------
*/
/*------------------------------------------------------------------------------
** Callback function to provide the PROFINET Device ID to the CompactCom.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       PROFINET Device ID of the device.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_ImplProfinetIoObjDeviceId_Get( void );

/*------------------------------------------------------------------------------
** Callback function to provide the PROFINET Vendor ID to the CompactCom.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       PROFINET Vendor ID of the device.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_ImplProfinetIoObjVendorId_Get( void );

/*------------------------------------------------------------------------------
** Callback function to provide the PROFINET Order ID to the CompactCom.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       PROFINET Order ID of the device.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_ImplProfinetIoObjOrderId_Get( char* pPackedStrDest, UINT16 iBuffSize );
#endif

