/*******************************************************************************
** Copyright 2024-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Header file for ABCC command message handler.
********************************************************************************
*/

#ifndef ABCC_CMD_HANDLER_H_
#define ABCC_CMD_HANDLER_H_

#include "abp.h"


/*------------------------------------------------------------------------------
** Definitions and prototypes for attribute lookup table.
**------------------------------------------------------------------------------
*/
enum ABCC_ATTR_SERVICE_TAG
{
    SERVICE_UINT8,
    SERVICE_INT8,
    SERVICE_UINT16,
    SERVICE_INT16,
    SERVICE_UINT32,
    SERVICE_INT32,
    SERVICE_STR,
};

typedef UINT8 (*ABCC_UINT8_Get_AttrFuncType) (void);
typedef INT8  (*ABCC_INT8_Get_AttrFuncType)  (void);
typedef UINT16(*ABCC_UINT16_Get_AttrFuncType)(void);
typedef INT16 (*ABCC_INT16_Get_AttrFuncType) (void);
typedef UINT32(*ABCC_UINT32_Get_AttrFuncType)(void);
typedef INT32 (*ABCC_INT32_Get_AttrFuncType) (void);
typedef UINT16(*ABCC_STR_Get_AttrFuncType)   (char* pPackedStrDest, UINT16 iBuffSize);

typedef void (*ABCC_UINT8_Set_AttrFuncType) (UINT8);
typedef void (*ABCC_INT8_Set_AttrFuncType)  (INT8);
typedef void (*ABCC_UINT16_Set_AttrFuncType)(UINT16);
typedef void (*ABCC_INT16_Set_AttrFuncType) (INT16);
typedef void (*ABCC_UINT32_Set_AttrFuncType)(UINT32);
typedef void (*ABCC_INT32_Set_AttrFuncType) (INT32);
typedef void (*ABCC_STR_Set_AttrFuncType)   (char* pPackedStrSrc, UINT16 iBuffSize);

typedef struct
{
    UINT8 bObject;
    UINT8 bAttribute;
    enum ABP_MsgCmdType bCommand;
    enum ABCC_ATTR_SERVICE_TAG eServiceTag;
    union
    {
        ABCC_UINT8_Get_AttrFuncType  pnGetUint8Attr;
        ABCC_INT8_Get_AttrFuncType   pnGetInt8Attr;
        ABCC_UINT16_Get_AttrFuncType pnGetUint16Attr;
        ABCC_INT16_Get_AttrFuncType  pnGetInt16Attr;
        ABCC_UINT32_Get_AttrFuncType pnGetUint32Attr;
        ABCC_INT32_Get_AttrFuncType  pnGetInt32Attr;
        ABCC_STR_Get_AttrFuncType    pnGetStrAttr;
        ABCC_UINT8_Set_AttrFuncType  pnSetUint8Attr;
    }uCbx;
    union
    {
        UINT8  bUnsigned8;
        INT8   bSigned8;
        UINT16 iUnsigned16;
        INT16  iSigned16;
        UINT32 lUnsigned32;
        INT32  lSigned32;
        char*  pacString;

        UINT16 iAttrMaxDataSize;
    }uData;
}attr_lookup_type;

/*------------------------------------------------------------------------------
** Handle messages with the command bit set received from the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**    psReceivedMsg - Received message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_HandleCommandMessage( ABP_MsgType* psReceivedMsg );

#endif
