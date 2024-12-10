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
#define ABCC_APPLICATION_OBJ_SERIAL_NR_CBFUNC      {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_SER_NUM,      .eServiceTag = SERVICE_UINT32, .uCbx.pnGetUint32Attr = ABCC_ImplApplicationObjSerialNum}

#define ABCC_APPLICATION_OBJ_VENDOR_NAME_VALUE(x)  {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_VENDOR_NAME,  .eServiceTag = SERVICE_STR,    .uData.pacString      = (x)}
#define ABCC_APPLICATION_OBJ_VENDOR_NAME_CBFUNC    {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_VENDOR_NAME,  .eServiceTag = SERVICE_STR,    .uCbx.pnGetStrAttr    = ABCC_ImplApplicationObjVendortName, .uData.iAttrMaxDataSize = ABP_APP_IA_VENDOR_NAME_MAX_DS }

#define ABCC_APPLICATION_OBJ_PRODUCT_NAME_VALUE(x) {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_PRODUCT_NAME, .eServiceTag = SERVICE_STR,    .uData.pacString      = (x)}
#define ABCC_APPLICATION_OBJ_PRODUCT_NAME_CBFUNC   {.bObject = ABP_OBJ_NUM_APP, .bAttribute = ABP_APP_IA_PRODUCT_NAME, .eServiceTag = SERVICE_STR,    .uCbx.pnGetStrAttr    = ABCC_ImplApplicationObjProductName, .uData.iAttrMaxDataSize = ABP_APP_IA_PRODUCT_NAME_MAX_DS }


/* EtherNet/IP Host Object, 248 */
#define ABCC_ETHERNETIP_OBJ_VENDOR_ID_VALUE(x)   {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_VENDOR_ID,   .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_ETHERNETIP_OBJ_VENDOR_ID_CBFUNC     {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_VENDOR_ID,   .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplEthernetIpObjVendorId}

#define ABCC_ETHERNETIP_OBJ_DEVICE_TYPE_VALUE(x) {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_DEVICE_TYPE, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_ETHERNETIP_OBJ_DEVICE_TYPE_CBFUNC   {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_DEVICE_TYPE, .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplEthernetIpObjDeviceType}


#define ABCC_ETHERNETIP_OBJ_PRODUCT_CODE_VALUE(x) {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_PRODUCT_CODE, .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_ETHERNETIP_OBJ_PRODUCT_CODE_CBFUNC   {.bObject = ABP_OBJ_NUM_EIP, .bAttribute = ABP_EIP_IA_PRODUCT_CODE, .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplEthernetIpObjProductCode}


/* Profinet IO object, 246 */
#define ABCC_PROFINET_OBJ_DEVICE_ID_VALUE(x) {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_DEVICE_ID,   .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_PROFINET_OBJ_DEVICE_ID_CBFUNC   {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_DEVICE_ID,   .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplProfinetIoObjDeviceId}

#define ABCC_PROFINET_OBJ_VENDOR_ID_VALUE(x) {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_VENDOR_ID,   .eServiceTag = SERVICE_UINT16, .uData.iUnsigned16    = (x)}
#define ABCC_PROFINET_OBJ_VENDOR_ID_CBFUNC   {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_VENDOR_ID,   .eServiceTag = SERVICE_UINT16, .uCbx.pnGetUint16Attr = ABCC_ImplProfinetIoObjVendorId}

#define ABCC_PROFINET_OBJ_ORDER_ID_VALUE(x)  {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_IM_ORDER_ID, .eServiceTag = SERVICE_STR,    .uData.pacString      = (x)}
#define ABCC_PROFINET_OBJ_ORDER_ID_CBFUNC    {.bObject = ABP_OBJ_NUM_PNIO, .bAttribute = ABP_PNIO_IA_IM_ORDER_ID, .eServiceTag = SERVICE_STR,    .uCbx.pnGetStrAttr    = ABCC_ImplProfinetIoObjOrderId, .uData.iAttrMaxDataSize = ABP_PNIO_IA_IM_ORDER_ID_DS }

/*------------------------------------------------------------------------------
** Predefined callback function prototypes used by attribute_table.
** **------------------------------------------------------------------------------
*/
/* Application object, 255*/
UINT32 ABCC_ImplApplicationObjSerialNum( void );
UINT16 ABCC_ImplApplicationObjVendortName( char* pPackedStrDest, UINT16 iBuffSize );
UINT16 ABCC_ImplApplicationObjProductName( char* pPackedStrDest, UINT16 iBuffSize );



/* EtherNet/IP Host Object, 248 */
UINT16 ABCC_ImplEthernetIpObjVendorId( void );
UINT16 ABCC_ImplEthernetIpObjDeviceType( void );
UINT16 ABCC_ImplEthernetIpObjProductCode( void );


/* Profinet IO object, 246 */
UINT16 ABCC_ImplProfinetIoObjDeviceId( void );
UINT16 ABCC_ImplProfinetIoObjVendorId( void );
UINT16 ABCC_ImplProfinetIoObjOrderId( char* pPackedStrDest, UINT16 iBuffSize );
#endif

