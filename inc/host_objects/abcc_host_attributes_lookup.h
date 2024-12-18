/*******************************************************************************
** Copyright 2018-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Assembly Mapping Object 0xEB - Public interfaces
********************************************************************************
*/
#include "abcc.h"
#include "abp.h"
#include "abp_eip.h"
#include "abp_pnio.h"


#ifndef ABCC_HOST_ATTR_LOOKUP_H_
#define ABCC_HOST_ATTR_LOOKUP_H_

/* Application object, 255*/
#define ABCC_APPLICATION_OBJ_SERIAL_NR_GET_VALUE(x)    {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_SER_NUM, .bCommand = ABP_CMD_GET_ATTR,      .eServiceTag = SERVICE_UINT32, .uData.lUnsigned32    = (x)}
#define ABCC_APPLICATION_OBJ_SERIAL_NR_GET_CBFUNC      {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_SER_NUM, .bCommand = ABP_CMD_GET_ATTR,      .eServiceTag = SERVICE_UINT32, .uCbx.pnGetUint32Attr = ABCC_ImplApplicationObjSerialNum_Get}

#define ABCC_APPLICATION_OBJ_VENDOR_NAME_GET_VALUE(x)  {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_VENDOR_NAME, .bCommand = ABP_CMD_GET_ATTR,  .eServiceTag = SERVICE_STR,    .uData.pacString      = (x)}
#define ABCC_APPLICATION_OBJ_VENDOR_NAME_GET_CBFUNC    {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_VENDOR_NAME, .bCommand = ABP_CMD_GET_ATTR,  .eServiceTag = SERVICE_STR,    .uCbx.pnGetStrAttr    = ABCC_ImplApplicationObjVendortName_Get,       .uData.iAttrMaxDataSize = ABP_APP_IA_VENDOR_NAME_MAX_DS }

#define ABCC_APPLICATION_OBJ_PRODUCT_NAME_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_PRODUCT_NAME, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_STR,    .uData.pacString      = (x)}
#define ABCC_APPLICATION_OBJ_PRODUCT_NAME_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_PRODUCT_NAME, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_STR,    .uCbx.pnGetStrAttr    = ABCC_ImplApplicationObjProductName_Get,       .uData.iAttrMaxDataSize = ABP_APP_IA_PRODUCT_NAME_MAX_DS }

#define ABCC_APPLICATION_OBJ_FW_AVAILABLE_GET_CBFUNC     {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_FW_AVAILABLE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT8,  .uCbx.pnGetStrAttr    = ABCC_ImplApplicationObjFirmwareAvailable_Get, .uData.iAttrMaxDataSize = ABP_APP_IA_FW_AVAILABLE_DS }
#define ABCC_APPLICATION_OBJ_FW_AVAILABLE_SET_CBFUNC   {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_FW_AVAILABLE, .bCommand = ABP_CMD_SET_ATTR, .eServiceTag = SERVICE_UINT8,  .uCbx.pnGetStrAttr    = ABCC_ImplApplicationObjFirmwareAvailable_Set, .uData.iAttrMaxDataSize = ABP_APP_IA_FW_AVAILABLE_DS }

/* EtherNet/IP Host Object, 248 */
#define ABCC_ETHERNETIP_OBJ_VENDOR_ID_GET_VALUE(x)   {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_VENDOR_ID, .bCommand = ABP_CMD_GET_ATTR,   .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_ETHERNETIP_OBJ_VENDOR_ID_GET_CBFUNC     {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_VENDOR_ID, .bCommand = ABP_CMD_GET_ATTR,   .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplEthernetIpObjVendorId_Get}

#define ABCC_ETHERNETIP_OBJ_DEVICE_TYPE_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_DEVICE_TYPE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_ETHERNETIP_OBJ_DEVICE_TYPE_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_DEVICE_TYPE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplEthernetIpObjDeviceType_Get}


#define ABCC_ETHERNETIP_OBJ_PRODUCT_CODE_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_PRODUCT_CODE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_ETHERNETIP_OBJ_PRODUCT_CODE_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_PRODUCT_CODE, .bCommand = ABP_CMD_GET_ATTR, .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplEthernetIpObjProductCode_Get}


/* Profinet IO object, 246 */
#define ABCC_PROFINET_OBJ_DEVICE_ID_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_DEVICE_ID,   .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_PROFINET_OBJ_DEVICE_ID_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_DEVICE_ID,   .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplProfinetIoObjDeviceId_Get}

#define ABCC_PROFINET_OBJ_VENDOR_ID_GET_VALUE(x) {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_VENDOR_ID,   .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_PROFINET_OBJ_VENDOR_ID_GET_CBFUNC   {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_VENDOR_ID,   .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplProfinetIoObjVendorId_Get}

#define ABCC_PROFINET_OBJ_ORDER_ID_GET_VALUE(x)  {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_IM_ORDER_ID, .eServiceTag = SERVICE_STR,    .uData.pacString      = (x)}
#define ABCC_PROFINET_OBJ_ORDER_ID_GET_CBFUNC    {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_IM_ORDER_ID, .eServiceTag = SERVICE_STR,    .uCbx.pnGetStrAttr    = ABCC_ImplProfinetIoObjOrderId_Get, .uData.iAttrMaxDataSize = ABP_PNIO_IA_IM_ORDER_ID_DS }

/*------------------------------------------------------------------------------
** Predefined callback function prototypes used by attribute_table.
** **------------------------------------------------------------------------------
*/
/* Application object, 255*/
UINT32 ABCC_ImplApplicationObjSerialNum_Get( void );
UINT16 ABCC_ImplApplicationObjVendortName_Get( char* pPackedStrDest, UINT16 iBuffSize );
UINT16 ABCC_ImplApplicationObjProductName_Get( char* pPackedStrDest, UINT16 iBuffSize );
UINT8 ABCC_ImplApplicationObjFirmwareAvailable_Get( void );
void ABCC_ImplApplicationObjFirmwareAvailable_Set( UINT8 bValue );



/* EtherNet/IP Host Object, 248 */
UINT16 ABCC_ImplEthernetIpObjVendorId_Get( void );
UINT16 ABCC_ImplEthernetIpObjDeviceType_Get( void );
UINT16 ABCC_ImplEthernetIpObjProductCode_Get( void );


/* Profinet IO object, 246 */
UINT16 ABCC_ImplProfinetIoObjDeviceId_Get( void );
UINT16 ABCC_ImplProfinetIoObjVendorId_Get( void );
UINT16 ABCC_ImplProfinetIoObjOrderId_Get( char* pPackedStrDest, UINT16 iBuffSize );
#endif

