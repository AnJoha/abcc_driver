/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version edc67ee (2024-10-25)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2013-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** ABCC driver API used by the the application.
********************************************************************************
*/
#ifndef ABCC_H_
#define ABCC_H_

#include <inttypes.h>
#include <stdarg.h>
#include "abcc_config.h"
#include "abcc_port.h"
#include "abcc_types.h"
#include "abp.h"
#include "abcc_application_data_interface.h"

/*------------------------------------------------------------------------------
** Bit definitions of ABCC events.
** These bit definitions are used in the bit mask forwarded to the
** ABCC_CbfEvent() callback.
**------------------------------------------------------------------------------
*/
#define ABCC_ISR_EVENT_RDPD       0x01
#define ABCC_ISR_EVENT_RDMSG      0x02
#define ABCC_ISR_EVENT_WRMSG      0x04
#define ABCC_ISR_EVENT_STATUS     0x08

/*------------------------------------------------------------------------------
** Function types used by user to deliver messages to the application.
**------------------------------------------------------------------------------
*/
typedef void (*ABCC_MsgHandlerFuncType)( ABP_MsgType* psMsg );

/*------------------------------------------------------------------------------
** Function to indicate that a segmentation session has finished and the message
** has been completely sent.
** See description of ABCC_StartServerRespSegmentationSession().
**------------------------------------------------------------------------------
** Arguments:
**       pxObject - User defined (Supplied in
**                  ABCC_StartServerRespSegmentationSession())
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
typedef void (*ABCC_SegMsgHandlerDoneFuncType)( void* pxObject );

/*------------------------------------------------------------------------------
** Function callback to fetch next block of data to be sent by the segmentation
** handler. Note that the size of the block is unrelated to the segment size
** used by the driver i.e. the segment size does not need to be known by the
** callback function.
**
** See ABCC_StartServerRespSegmentationSession
**------------------------------------------------------------------------------
** Arguments:
**       pxObject - User defined (Supplied in
**                  ABCC_StartServerRespSegmentationSession)
**       plSize   - Pointer to size of returned data block (updated by user).
**                  Don't care if return buffer is NULL.
**
**
** Returns:
**       Pointer to next data block. NULL means no data.
**------------------------------------------------------------------------------
*/
typedef UINT8* (*ABCC_SegMsgHandlerNextBlockFuncType)( void* pxObject, UINT32* plSize );

/*------------------------------------------------------------------------------
** Driver severity codes indicated by ABCC_CbfDriverError.
**------------------------------------------------------------------------------
*/
typedef enum ABCC_SeverityType
{
   /*
   ** Information about an event that has occurred (e.g., serial message lost).
   */
   ABCC_SEV_INFORMATION = 0,

   /*
   ** An error of minor importance has occurred. The system can recover from
   ** this error.
   */
   ABCC_SEV_WARNING,

   /*
   ** A fatal event has occurred, the system cannot recover (e.g., driver is
   ** out of timers).
   */
   ABCC_SEV_FATAL,

   /*
   ** Force the compiler to use a 16-bit variable as enumeration.
   */
   ABCC_SEV_SET_ENUM_ANSI_SIZE = 0x7FFF

}
ABCC_SeverityType;

/*------------------------------------------------------------------------------
**  Driver error codes indicated by ABCC_CbfDriverError.
**------------------------------------------------------------------------------
*/
typedef enum ABCC_ErrorCodeType
{
   ABCC_EC_NO_ERROR,
   ABCC_EC_INTERNAL_ERROR,
   ABCC_EC_LINK_CMD_QUEUE_FULL,
   ABCC_EC_LINK_RESP_QUEUE_FULL,
   ABCC_EC_OUT_OF_MSG_BUFFERS,
   ABCC_EC_TRYING_TO_FREE_NULL_POINTER,
   ABCC_EC_INCORRECT_OPERATING_MODE,
   ABCC_EC_INCORRECT_STATE,
   ABCC_EC_RESP_MSG_E_BIT_SET,
   ABCC_EC_WRPD_SIZE_ERR,
   ABCC_EC_RDPD_SIZE_ERR,
   ABCC_EC_RDMSG_SIZE_ERR,
   ABCC_EC_INVALID_RESP_SOURCE_ID,
   ABCC_EC_MODULE_NOT_DECTECTED,
   ABCC_EC_PARAMETER_NOT_VALID,
   ABCC_EC_MODULE_ID_NOT_SUPPORTED,
   ABCC_EC_DEFAULT_MAP_ERR,
   ABCC_EC_ERROR_IN_READ_MAP_CONFIG,
   ABCC_EC_ERROR_IN_WRITE_MAP_CONFIG,
   ABCC_EC_INTSTATUS_NOT_SUPPORTED_BY_DRV_IMPL,
   ABCC_EC_MODCAP_NOT_SUPPORTED_BY_DRV_IMPL,
   ABCC_EC_SPI_OP_NOT_ALLOWED_DURING_SPI_TRANSACTION,
   ABCC_EC_WRMSG_SIZE_ERR,
   ABCC_EC_MSG_BUFFER_CORRUPTED,
   ABCC_EC_MSG_BUFFER_ALREADY_FREED,
   ABCC_EC_NO_RESOURCES,
   ABCC_EC_HW_INIT_FAILED,
   ABCC_EC_RCV_CMD_SIZE_EXCEEDS_BUFFER,
   ABCC_EC_RCV_RESP_SIZE_EXCEEDS_BUFFER,
   ABCC_EC_UNEXPECTED_NULL_PTR,
   ABCC_EC_OUT_OF_CMD_SEQ_RESOURCES,
   ABCC_EC_SYS_ADAPTION_ERR,
   ABCC_EC_APPLICATION_SPECIFIC,
   ABCC_EC_ERROR_IN_PD_MAP_CONFIG,
   ABCC_EC_ERROR_RESP_SEGMENTATION,
   ABCC_EC_BAD_ASSEMBLY_INSTANCE,
   ABCC_EC_UNSUPPORTED_DATA_TYPE,
   ABCC_EC_CHECKSUM_MISMATCH,
   ABCC_EC_SETUP_FAILED,
   ABCC_EC_SET_ENUM_ANSI_SIZE       = 0x7FFF
}
ABCC_ErrorCodeType;

/*------------------------------------------------------------------------------
**  Error information structure used by ABCC_GetDriverError()
**------------------------------------------------------------------------------
*/
typedef struct ABCC_ErrInfo
{
   ABCC_ErrorCodeType   eErrorCode;
   ABCC_SeverityType    eSeverity;
   UINT32               lAddInfo;
#if ABCC_CFG_DEBUG_ERR_ENABLED
   char*                pacSeverity;
   char*                pacErrorCode;
   char*                pacAddInfo;
   char*                pacFile;
   INT32                lLine;
#endif
}
ABCC_ErrInfoType;

/*------------------------------------------------------------------------------
** ABCC firmware version structure.
**------------------------------------------------------------------------------
*/
typedef struct ABCC_FwVersion
{
   UINT8 bMajor;
   UINT8 bMinor;
   UINT8 bBuild;
}
ABCC_FwVersionType;

/*------------------------------------------------------------------------------
** ABCC_CommunicationStateType:
**
** ABCC_NOT_READY_FOR_COMMUNICATION: Nothing is wrong but it
**                                   is not yet possible to
**                                   communicate with the ABCC.
** ABCC_READY_FOR_COMMUNICATION:     Possible to communicate with ABCC
** ABCC_STARTUP_TIMEOUT:             ABCC module did not start within the
**                                   expected time.
** ABCC_FW_UPDATE:                   The ABCC is assumed to be performing a
**                                   firmware update since it's not responding
**                                   within the expected time.
**------------------------------------------------------------------------------
*/
typedef enum ABCC_CommunicationState
{
   ABCC_NOT_READY_FOR_COMMUNICATION = 0,
   ABCC_READY_FOR_COMMUNICATION = 1,
   ABCC_STARTUP_TIMEOUT = 2,
#if ABCC_CFG_DRV_ASSUME_FW_UPDATE_ENABLED
   ABCC_ASSUME_FW_UPDATE = 3
#endif
}
ABCC_CommunicationStateType;


/*------------------------------------------------------------------------------
** Used for storing the data format of the field bus.
** NET_UNKNOWN means that the Anybus-CC has not yet responded to our command to
** read the fieldbus data format.
**------------------------------------------------------------------------------
*/
typedef enum NetFormatType
{
   NET_LITTLEENDIAN,
   NET_BIGENDIAN,
   NET_UNKNOWN
}
NetFormatType;

/*------------------------------------------------------------------------------
** Type for indicate if parameter support is available or not.
**------------------------------------------------------------------------------
*/
typedef enum ParameterSupportType
{
   NOT_PARAMETER_SUPPORT,
   PARAMETER_SUPPORT,
   PARAMETER_UNKNOWN
}
ParameterSupportType;

/*------------------------------------------------------------------------------
** Macros for basic endian swap. Used by conversion macros below.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_iEndianSwap
#define ABCC_iEndianSwap( iFoo )    (UINT16)( ( (UINT16)(iFoo) >> 8 ) | ( (UINT16)(iFoo) << 8 ) )
#endif

#ifndef ABCC_lEndianSwap
#define ABCC_lEndianSwap( lFoo )      (UINT32)( ( (UINT32)ABCC_iEndianSwap( (UINT16)( (UINT32)(lFoo) ) ) << 16 ) | (UINT32)ABCC_iEndianSwap( (UINT32)(lFoo) >> 16 ) )
#endif

#if ( ABCC_CFG_64BIT_ADI_SUPPORT_ENABLED || ABCC_CFG_DOUBLE_ADI_SUPPORT_ENABLED )
#ifndef ABCC_l64EndianSwap
#define ABCC_l64EndianSwap( lFoo )  (UINT64)( ( (UINT64)ABCC_lEndianSwap( (UINT32)( (UINT64)(lFoo) ) ) << 32 ) | (UINT64)ABCC_lEndianSwap( (UINT64)(lFoo) >> 32 ) )
#endif
#endif

/*------------------------------------------------------------------------------
** Macros for reading/writing byte to/from a 16 bit word.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_iSetLSB
#define ABCC_iSetLSB( iDest, iSrc )                               \
do                                                                \
{                                                                 \
   (iDest) &= (UINT16)0xFF00;                                     \
   (iDest) |= (UINT16)(iSrc) & (UINT16)0x00FF;                    \
}                                                                 \
while( 0 )
#endif

#ifndef ABCC_iSetMSB
#define ABCC_iSetMSB( iDest, iSrc )                               \
do                                                                \
{                                                                 \
   (iDest) &=  (UINT16)0x00FF;                                    \
   (iDest) |=  (UINT16)(iSrc) << 8;                               \
}                                                                 \
while( 0 )
#endif

#ifndef ABCC_iLSB
#define ABCC_iLSB( iFoo )           (UINT16)( (iFoo) & 0x00FF )
#endif

#ifndef ABCC_iMSB
#define ABCC_iMSB( iFoo )           (UINT16)( (UINT16)(iFoo) >> 8 )
#endif

/*------------------------------------------------------------------------------
** Endian dependent macros:
**------------------------------------------------------------------------------
** Macros to convert to/from native endian to/from specified endian:
**
** iBeTOi( iBeFoo )   - 16 bit big endian    -> native endian
** iTOiBe( iFoo )     - 16 bit native endian -> big endian
** iLeTOi( iLeFoo )   - 16 bit little endian -> native endian
** iTOiLe( iFoo )     - 16 bit native endian -> little endian
**                    -
** lBeTOl( lBeFoo )   - 32 bit big endian    -> native endian
** lTOlBe( lFoo )     - 32 bit native endian -> big endian
** lLeTOl( lLeFoo )   - 32 bit little endian -> native endian
** lTOlLe( lFoo )     - 32 bit native endian -> little endian
**                    -
** lBeTOl64( lBeFoo ) - 64 bit big endian    -> native endian
** lTOlBe64( lFoo )   - 64 bit native endian -> big endian
** lLeTOl64( lLeFoo ) - 64 bit little endian -> native endian
** lTOlLe64( lFoo )   - 64 bit native endian -> little endian
**------------------------------------------------------------------------------
** Macros to set/get low/high address octet from a word:
**
** ABCC_GetLowAddrOct( iFoo )
** ABCC_GetHighAddrOct( iFoo )
** ABCC_SetLowAddrOct( iDest, iSrc )
** ABCC_SetHighAddrOct( iDest, iSrc )
**------------------------------------------------------------------------------
*/
#ifdef ABCC_SYS_BIG_ENDIAN

   #define iBeTOi( iBeFoo )                     (UINT16)(iBeFoo)
   #define iTOiBe( iFoo )                       (UINT16)(iFoo)
   #define iLeTOi( iLeFoo )                     ABCC_iEndianSwap( iLeFoo )
   #define iTOiLe( iFoo )                       ABCC_iEndianSwap( iFoo )

   #define lBeTOl( lBeFoo )                     (UINT32)(lBeFoo)
   #define lTOlBe( lFoo )                       (UINT32)(lFoo)
   #define lLeTOl( lLeFoo )                     ABCC_lEndianSwap( lLeFoo )
   #define lTOlLe( lFoo )                       ABCC_lEndianSwap( lFoo )


   #define ABCC_GetLowAddrOct( iFoo )           ABCC_iMSB( iFoo )
   #define ABCC_GetHighAddrOct( iFoo )          ABCC_iLSB( iFoo )

   #define ABCC_SetLowAddrOct( iDest, iSrc )    ABCC_iSetMSB( iDest, iSrc )
   #define ABCC_SetHighAddrOct( iDest, iSrc )   ABCC_iSetLSB( iDest, iSrc )

#else
   #define iBeTOi( iBeFoo )                     ABCC_iEndianSwap( iBeFoo )
   #define iTOiBe( iFoo )                       ABCC_iEndianSwap( iFoo )
   #define iLeTOi( iLeFoo )                     (UINT16)(iLeFoo)
   #define iTOiLe( iFoo )                       (UINT16)(iFoo)
   #define lBeTOl( lBeFoo )                     ABCC_lEndianSwap( lBeFoo )
   #define lTOlBe( lFoo )                       ABCC_lEndianSwap( lFoo )
   #define lLeTOl( lLeFoo )                     (UINT32)(lLeFoo)
   #define lTOlLe( lFoo )                       (UINT32)(lFoo)

   #define ABCC_GetLowAddrOct( iFoo )           ABCC_iLSB( iFoo )
   #define ABCC_GetHighAddrOct( iFoo )          ABCC_iMSB( iFoo )

   #define ABCC_SetLowAddrOct( iDest, iSrc )    ABCC_iSetLSB( iDest, iSrc )
   #define ABCC_SetHighAddrOct( iDest, iSrc )   ABCC_iSetMSB( iDest, iSrc )
#endif

#if ( ABCC_CFG_64BIT_ADI_SUPPORT_ENABLED || ABCC_CFG_DOUBLE_ADI_SUPPORT_ENABLED )
#ifdef ABCC_SYS_BIG_ENDIAN
#define lBeTOl64( lBeFoo )  (UINT64)(lBeFoo)
#define lTOlBe64( lFoo )    (UINT64)(lFoo)
#define lLeTOl64( lLeFoo )  ABCC_l64EndianSwap( lLeFoo )
#define lTOlLe64( lFoo )    ABCC_l64EndianSwap( lFoo )
#else
#define lBeTOl64( lBeFoo )  ABCC_l64EndianSwap( lBeFoo )
#define lTOlBe64( lFoo )    ABCC_l64EndianSwap( lFoo )
#define lLeTOl64( lLeFoo )  (UINT64)(lLeFoo)
#define lTOlLe64( lFoo )    (UINT64)(lFoo)
#endif
#endif

/*------------------------------------------------------------------------------
** 8/16 bit char platform dependent macros for reading ABP message type members
**------------------------------------------------------------------------------
** ABCC_GetMsgDataSize( psMsg )               - Message data size (in octets)
** ABCC_SetMsgDataSize( psMsg, iDataSizeVal ) - Set message data size
**                                              (in octets)
** ABCC_GetMsgInstance( psMsg )               - Message instance
** ABCC_GetMsgSourceId( psMsg )               - Message source id
** ABCC_GetMsgDestObj( psMsg )                - Destination object
** ABCC_IsCmdMsg( psMsg )                     - Message command bit
** ABCC_GetMsgCmdBits( psMsg )                - Message command
** ABCC_GetMsgCmdExt0( psMsg )                - Command extension 0
** ABCC_SetMsgCmdExt0( psMsg, bCmdExt0Val )   - Set command extension 0
** ABCC_GetMsgCmdExt1( psMsg )                - Command extension 1
** ABCC_SetMsgCmdExt1( psMsg, bCmdExt1Val )   - Set command extension 1
** ABCC_GetMsgCmdExt( psMsg )                 - Get extension 0 and 1
**                                              16 bit type
** ABCC_SetMsgCmdExt( psMsg , iExtVal)        - Set extension 0 and 1
**                                              16 bit type
** ABCC_GetMsgDataPtr( psMsg )                - Message data pointer
** ABCC_GetErrorCode( psMsg )                 - Error code
**------------------------------------------------------------------------------
*/

#ifdef ABCC_SYS_16_BIT_CHAR
#define ABCC_GetMsgDataSize( psMsg               ) ( iLeTOi(               ((ABP_MsgHeaderType16*) (psMsg))->iDataSize                                ) )
#define ABCC_SetMsgDataSize( psMsg, iDataSizeVal ) (                       ((ABP_MsgHeaderType16*) (psMsg))->iDataSize ) = iTOiLe( iDataSizeVal )
#define ABCC_GetMsgInstance( psMsg               ) ( iLeTOi(               ((ABP_MsgHeaderType16*) (psMsg))->iInstance                                ) )
#define ABCC_SetMsgInstance( psMsg, iInstanceVal ) (                       ((ABP_MsgHeaderType16*) (psMsg))->iInstance ) = iTOiLe( iInstanceVal )
#define ABCC_GetMsgSourceId( psMsg               ) ( ABCC_GetLowAddrOct(   ((ABP_MsgHeaderType16*) (psMsg))->iSourceIdDestObj                         ) )
#define ABCC_GetMsgDestObj(  psMsg               ) ( ABCC_GetHighAddrOct(  ((ABP_MsgHeaderType16*) (psMsg))->iSourceIdDestObj                         ) )
#define ABCC_IsCmdMsg(       psMsg               ) ( ABCC_GetLowAddrOct(   ((ABP_MsgHeaderType16*) (psMsg))->iCmdReserved ) & ABP_MSG_HEADER_C_BIT    )
#define ABCC_GetMsgCmdBits(  psMsg               ) ( ABCC_GetLowAddrOct(   ((ABP_MsgHeaderType16*) (psMsg))->iCmdReserved ) & ABP_MSG_HEADER_CMD_BITS )
#define ABCC_GetMsgCmdExt0(  psMsg               ) ( ABCC_GetLowAddrOct(   ((ABP_MsgHeaderType16*) (psMsg))->iCmdExt0CmdExt1                          ) )
#define ABCC_SetMsgCmdExt0(  psMsg, bCmdExt0Val  )   ABCC_SetLowAddrOct(   ((ABP_MsgHeaderType16*) (psMsg))->iCmdExt0CmdExt1, bCmdExt0Val             )
#define ABCC_GetMsgCmdExt1(  psMsg               ) ( ABCC_GetHighAddrOct(  ((ABP_MsgHeaderType16*) (psMsg))->iCmdExt0CmdExt1                          ) )
#define ABCC_SetMsgCmdExt1(  psMsg, bCmdExt1Val  )   ABCC_SetHighAddrOct(  ((ABP_MsgHeaderType16*) (psMsg))->iCmdExt0CmdExt1, bCmdExt1Val             )
#define ABCC_GetMsgCmdExt(   psMsg               ) (              iLeTOi(  ((ABP_MsgHeaderType16*) (psMsg))->iCmdExt0CmdExt1                          ) )
#define ABCC_SetMsgCmdExt(   psMsg, iExtVal      ) (                       ((ABP_MsgHeaderType16*) (psMsg))->iCmdExt0CmdExt1 = iTOiLe( iExtVal        ) )
#define ABCC_GetMsgDataPtr(  psMsg               ) (                                               (psMsg)->aiData                                    )
#define ABCC_GetErrorCode(   psMsg               ) ( ABCC_GetLowAddrOct(                           (psMsg)->aiData[ 0 ]                               ) )
#else
#define ABCC_GetMsgDataSize( psMsg               ) ( iLeTOi( ((ABP_MsgHeaderType*)   (psMsg))->iDataSize                          ) )
#define ABCC_SetMsgDataSize( psMsg, iDataSizeVal ) (         ((ABP_MsgHeaderType*)   (psMsg))->iDataSize ) = iTOiLe( iDataSizeVal )
#define ABCC_GetMsgInstance( psMsg               ) ( iLeTOi( ((ABP_MsgHeaderType*)   (psMsg))->iInstance                          ) )
#define ABCC_SetMsgInstance( psMsg, iInstanceVal ) (         ((ABP_MsgHeaderType*)   (psMsg))->iInstance ) = iTOiLe( iInstanceVal )
#define ABCC_GetMsgSourceId( psMsg               ) (         ((ABP_MsgHeaderType*)   (psMsg))->bSourceId                          )
#define ABCC_GetMsgDestObj(  psMsg               ) (         ((ABP_MsgHeaderType*)   (psMsg))->bDestObj                           )
#define ABCC_IsCmdMsg(       psMsg               ) (         ((ABP_MsgHeaderType*)   (psMsg))->bCmd & ABP_MSG_HEADER_C_BIT        )
#define ABCC_GetMsgCmdBits(  psMsg               ) (         ((ABP_MsgHeaderType*)   (psMsg))->bCmd & ABP_MSG_HEADER_CMD_BITS     )
#define ABCC_GetMsgCmdExt0(  psMsg               ) (         ((ABP_MsgHeaderType*)   (psMsg))->bCmdExt0                           )
#define ABCC_SetMsgCmdExt0(  psMsg, bCmdExt0Val  ) (         ((ABP_MsgHeaderType*)   (psMsg))->bCmdExt0        = (bCmdExt0Val)    )
#define ABCC_GetMsgCmdExt1(  psMsg               ) (         ((ABP_MsgHeaderType*)   (psMsg))->bCmdExt1                           )
#define ABCC_SetMsgCmdExt1(  psMsg, bCmdExt1Val  ) (         ((ABP_MsgHeaderType*)   (psMsg))->bCmdExt1        = (bCmdExt1Val)    )
#define ABCC_GetMsgCmdExt(   psMsg               ) ( iLeTOi( ((ABP_MsgHeaderType16*) (psMsg))->iCmdExt0CmdExt1                    ) )
#define ABCC_SetMsgCmdExt(   psMsg, iExtVal      ) (         ((ABP_MsgHeaderType16*) (psMsg))->iCmdExt0CmdExt1 = iTOiLe( iExtVal  ) )
#define ABCC_GetMsgDataPtr(  psMsg               ) (                                 (psMsg)->abData                              )
#define ABCC_GetErrorCode(   psMsg               ) (                                 (psMsg)->abData[ 0 ]                         )
#endif


/*------------------------------------------------------------------------------
** 8/16 bit char platform dependent macros to read and write message data.
** Two version of each get/set function exist:
** ABCC_GetMsgData<x> takes the abcc message pointer as input.
** ABCC_GetData<x> takes the abcc message payload pointer as input.
**------------------------------------------------------------------------------
** ABCC_Set<x>String()  - Copy native string to ABCC message payload
** ABCC_Get<x>String()  - Copy ABCC message payload string to native string
** ABCC_Set<x>Data8()   - Write 8 bit data to ABCC message payload
** ABCC_Set<x>Data16()  - Write 16 bit data to ABCC message payload
** ABCC_Set<x>Data32()  - Write 32 bit data to ABCC message payload
** ABCC_Set<x>Data64()  - Write 64 bit data to ABCC message payload
** ABCC_Get<x>Data8()   - Read 8 bit data from an ABCC message payload
** ABCC_Get<x>Data16()  - Read 16 bit data from an ABCC message payload
** ABCC_Get<x>Data32()  - Read 32 bit data from an ABCC message payload
** ABCC_Get<x>Data64()  - Read 64 bit data from an ABCC message payload
**------------------------------------------------------------------------------
** ABCC_SetMsgString( psMsg, pcString, iNumChar, iOctetOffset )
**    psMsg - Pointer to message
** ABCC_SetString( pxDst, pcString, iNumChar, iOctetOffset )
**    pxDst - Pointer to message payload
**    pcString - String to be written
**    iNumChar - Number of chars in the string
**    iOctetOffset - Offset to where the string shall be written.
**
** ABCC_GetMsgString( pxSrc, pcString, iNumChar, iOctetOffset )
**    psMsg - Pointer to message
** ABCC_GetString( psMsg, pcString, iNumChar, iOctetOffset )
**    pxSrc - Pointer to message payload
**    pcString - String to be written
**    iNumChar - Number of chars in the string
**    iOctetOffset - Offset to where the string shall be read.
**
** ABCC_SetMsgDataX( psMsg, Data, iOctetOffset )
**    psMsg - Pointer to message
** ABCC_SetDataX( pxDst, Data, iOctetOffset )
**    pxDst - Pointer to message payload
**    Data  - Data to be set
**    iOctetOffset - Offset to where data shall be written.
**
** ABCC_GetMsgDataX( psMsg, Data, iOctetOffset )
**    psMsg - Pointer to message
** ABCC_GetDataX( pxSrc, Data, iOctetOffset )
**    pxSrc - Pointer to message
**    Data  - Read data variable
**    iOctetOffset - Offset to where data shall be read.
**------------------------------------------------------------------------------
*/
void ABCC_SetString( void* pxDst, const char* pcString, UINT16 iNumChar, UINT16 iOctetOffset );
#define ABCC_SetMsgString( psMsg, pcString, iNumChar, iOctetOffset ) \
   ABCC_SetString( ABCC_GetMsgDataPtr( psMsg ), pcString, iNumChar, iOctetOffset )

void ABCC_GetString( void* pxSrc, char* pcString, UINT16 iNumChar, UINT16 iOctetOffset );
#define  ABCC_GetMsgString( psMsg, pcString, iNumChar, iOctetOffset ) \
   ABCC_GetString( ABCC_GetMsgDataPtr( psMsg ), pcString, iNumChar, iOctetOffset )

void ABCC_GetData8( void* pxSrc, UINT8* pbData, UINT16 iOctetOffset );
#define ABCC_GetMsgData8( psMsg, pbData, iOctetOffset ) \
   ABCC_GetData8( ABCC_GetMsgDataPtr( psMsg ), pbData, iOctetOffset )

void ABCC_SetData8( void* pxDst, UINT8 bData, UINT16 iOctetOffset );
#define ABCC_SetMsgData8( psMsg, bData, iOctetOffset ) \
   ABCC_SetData8( ABCC_GetMsgDataPtr( psMsg ), bData, iOctetOffset )

void ABCC_GetData16( void* pxSrc, UINT16* piData, UINT16 iOctetOffset );
#define ABCC_GetMsgData16( psMsg, piData, iOctetOffset ) \
   ABCC_GetData16( ABCC_GetMsgDataPtr( psMsg ), piData, iOctetOffset )

void ABCC_SetData16( void* pxDst, UINT16 iData, UINT16 iOctetOffset );
#define ABCC_SetMsgData16( psMsg, iData, iOctetOffset ) \
   ABCC_SetData16( ABCC_GetMsgDataPtr( psMsg ), iData, iOctetOffset )

void ABCC_GetData32( void* pxSrc, UINT32* plData, UINT16 iOctetOffset );
#define ABCC_GetMsgData32( psMsg, plData, iOctetOffset ) \
   ABCC_GetData32( ABCC_GetMsgDataPtr( psMsg ), plData, iOctetOffset )

void ABCC_SetData32( void* pxDst, UINT32 lData, UINT16 iOctetOffset );
#define ABCC_SetMsgData32( psMsg, lData, iOctetOffset ) \
   ABCC_SetData32( ABCC_GetMsgDataPtr( psMsg ), lData, iOctetOffset )

#if ( ABCC_CFG_64BIT_ADI_SUPPORT_ENABLED || ABCC_CFG_DOUBLE_ADI_SUPPORT_ENABLED )
void ABCC_GetData64( void* pxSrc, UINT64* plData, UINT16 iOctetOffset );
#define ABCC_GetMsgData64( psMsg, plData, iOctetOffset ) \
   ABCC_GetData64( ABCC_GetMsgDataPtr( psMsg ), plData, iOctetOffset )

void ABCC_SetData64( void* pxDst, UINT64 lData, UINT16 iOctetOffset );
#define ABCC_SetMsgData64( psMsg, lData, iOctetOffset ) \
   ABCC_SetData64( ABCC_GetMsgDataPtr( psMsg ), lData, iOctetOffset )
#endif

/*------------------------------------------------------------------------------
** Copy a number of bytes, from the source pointer to the destination pointer.
** This function can be modified to use performance enhancing platform specific
** instructions. The default implementation is memcpy().
**------------------------------------------------------------------------------
** Arguments:
**    pbDestinationPtr - Pointer to the destination.
**    pbSourcePtr      - Pointer to source data.
**    iNbrOfBytes      - The number of bytes that shall be copied.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#define ABCC_MemCpy( pbDestinationPtr, pbSourcePtr, iNbrOfBytes )              \
        ABCC_PORT_MemCpy( pbDestinationPtr, pbSourcePtr, iNbrOfBytes )

/*------------------------------------------------------------------------------
** Print debug information to output defined by ABCC_PORT_printf()
**------------------------------------------------------------------------------
** Arguments:
**    printf style with a format string followed by a variable number of
**    arguments.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_DEBUG_TIMESTAMPS_ENABLED
void ABCC_DebugPrintf( const char *fmt, ... );
#else
#define ABCC_DebugPrintf( ... ) ABCC_PORT_printf( __VA_ARGS__ )
#endif

/*------------------------------------------------------------------------------
** Called when an error is detected. Used by the following support macros:
** ABCC_ASSERT()
** ABCC_ASSERT_ERR()
** ABCC_ERROR()
**
** The function will log and report error to application and take necessary
** action depending on severity. More human readable information will be
** available if ABCC_CFG_DEBUG_ERR_ENABLED is set to 1 (see ABCC_ErrInfoType)
** but this will have significant impact on the code size.
**------------------------------------------------------------------------------
** Arguments:
**    eSeverity         - Severity (see ABCC_SeverityType)
**    eErrorCode        - Error code (see ABCC_ErrorCodeType)
**    lAddInfo          - Additional info relevant for error
**    pacSeverity       - Pointer to severity in text format
**    pacErrorCode      - Pointer to error code in text format
**    pacAddInfo        - Pointer to additional info in text format
**    pacLoaction       - Pointer to file and line info in text format
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_DEBUG_ERR_ENABLED
void ABCC_ErrorHandler( ABCC_SeverityType eSeverity,
                        ABCC_ErrorCodeType eErrorCode,
                        UINT32 lAddInfo,
                        char* pacSeverity,
                        char* pacErrorCode,
                        char* pacAddInfo,
                        char* pacFile,
                        INT32 lLine );
#else
void ABCC_ErrorHandler( ABCC_SeverityType eSeverity,
                        ABCC_ErrorCodeType eErrorCode,
                        UINT32 lAddInfo );
#endif

/*
** Create constant strings for file and line information.
** Used by error macros
*/
#define STR( x )        #x
#define XSTR( x )       STR( x )
#define FileLine        "File: " __FILE__" (Line:" XSTR( __LINE__ )")"

/*
** Call ABCC_ErrorHandler with different arguments depending on
** ABCC_CFG_DEBUG_ERR_ENABLED.
*/
#if ABCC_CFG_ERR_REPORTING_ENABLED
#if ABCC_CFG_DEBUG_ERR_ENABLED
#define ABCC_ERROR_HANDLER( eSeverity, eErrorCode, lAddInfo )                  \
        ABCC_ErrorHandler( eSeverity, eErrorCode, lAddInfo,                    \
                           #eSeverity, #eErrorCode, #lAddInfo, __FILE__, __LINE__ )
#else
#define ABCC_ERROR_HANDLER( eSeverity, eErrorCode, lAddInfo )                  \
        ABCC_ErrorHandler( eSeverity, eErrorCode, lAddInfo )
#endif
#else
#define ABCC_ERROR_HANDLER( eSeverity, eErrorCode, lAddInfo )
#endif

/*------------------------------------------------------------------------------
** Assert boolean expression. ABCC_SEV_FATAL will be reported if assertion
** fails.
**------------------------------------------------------------------------------
** Arguments:
**    x                 - Boolean expression to be asserted.
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#define ABCC_ASSERT( x )                                                       \
   if( !( x ) )                                                                \
   {                                                                           \
      ABCC_ERROR_HANDLER( ABCC_SEV_FATAL, ABCC_EC_INTERNAL_ERROR, 0 );         \
   }

/*------------------------------------------------------------------------------
** Assert boolean expression with user defined error information if the
** assertion fails.
**------------------------------------------------------------------------------
** Arguments:
**    x                 - Boolean expression to be asserted.
**    eSeverity         - Severity (see ABCC_SeverityType)
**    eErrorCode        - Error code (see ABCC_ErrorCodeType)
**    lAddInfo          - Additional info relevant for error
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#define ABCC_ASSERT_ERR( x, eSeverity, eErrorCode, lAddInfo )                  \
   if( !( x ) )                                                                \
   {                                                                           \
      ABCC_ERROR_HANDLER( eSeverity, eErrorCode, lAddInfo );                   \
   }

/*------------------------------------------------------------------------------
** Report error with user defined error information.
**------------------------------------------------------------------------------
** Arguments:
**    eSeverity         - Severity (see ABCC_SeverityType)
**    eErrorCode        - Error code (see ABCC_ErrorCodeType)
**    lAddInfo          - Additional info relevant for error
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#define ABCC_ERROR( eSeverity, eErrorCode, lAddInfo )                          \
        ABCC_ERROR_HANDLER( eSeverity, eErrorCode, lAddInfo )

/*------------------------------------------------------------------------------
** Debug user defined error information. Set ABCC_CFG_DEBUG_ERR_ENABLED to
** enable debug print.
**------------------------------------------------------------------------------
** Arguments:
**    printf style function
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_DEBUG_ERR_ENABLED
#define ABCC_DEBUG_ERR( ... ) ABCC_DebugPrintf( __VA_ARGS__ )
#else
#define ABCC_DEBUG_ERR( ... )
#endif

/*------------------------------------------------------------------------------
** Get the error log for errors that have occurred so far. The maximum
** number of errors is controlled by ABCC_DRV_CFG_MAX_NUM_ERR_LOG. If the log is
** full the the last error will be overwritten i.e. the last error in the array
** will always be the latest error.
** More human readable information will be available if
** ABCC_CFG_DEBUG_ERR_ENABLED is set to 1 (see ABCC_ErrInfoType) but this
** will have significant impact on the code size.
**------------------------------------------------------------------------------
** Arguments:
**    ppacErrInfo       - Pointer to array of ABCC_ErrInfoType
** Returns:
**    Number of errors in array.
**------------------------------------------------------------------------------
*/
UINT8 ABCC_GetDriverError( ABCC_ErrInfoType** ppacErrInfo );

/*------------------------------------------------------------------------------
** This function is used to measure sync timings.
** ABCC_CFG_SYNC_MEASUREMENT_OP_ENABLED is used when measuring the output
** processing time and ABCC_CFG_SYNC_MEASUREMENT_IP_ENABLED is used to measure
** the input processing time.
**------------------------------------------------------------------------------
** Arguments:
**    None
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ( ABCC_CFG_SYNC_MEASUREMENT_OP_ENABLED || ABCC_CFG_SYNC_MEASUREMENT_IP_ENABLED )
EXTFUNC void ABCC_GpioReset( void );
#endif

/*------------------------------------------------------------------------------
** This function is used to measure sync timings.
** ABCC_CFG_SYNC_MEASUREMENT_OP_ENABLED is used when measuring the output
** processing time and ABCC_CFG_SYNC_MEASUREMENT_IP_ENABLED is used to measure
** the input processing time.
**------------------------------------------------------------------------------
** Arguments:
**    None
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ( ABCC_CFG_SYNC_MEASUREMENT_OP_ENABLED || ABCC_CFG_SYNC_MEASUREMENT_IP_ENABLED )
EXTFUNC void ABCC_GpioSet( void );
#endif

/*------------------------------------------------------------------------------
** This function will initiate the hardware required to communicate with the
** ABCC. This interface shall be called once during the power up initialization.
** The driver can be restarted without calling this interface again.
**------------------------------------------------------------------------------
** Arguments:
**    None
** Returns:
**    ABCC_ErrorCodeType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_ErrorCodeType ABCC_HwInit( void );

/*------------------------------------------------------------------------------
** This function will initiate the driver, enable interrupt, and set the
** operation mode. If a firmware update is pending a delay, iMaxStartupTime, can
** be defined describing how long the driver is to wait for the startup
** interrupt. iMaxStartupTime set to Zero (0) makes the driver use the
** ABCC_CFG_STARTUP_TIME_MS time.
** When this function has been called the timer system could be started,
** see ABCC_RunTimerSystem().
** Note! This function will NOT release the reset of the ABCC.
** To release reset, ABCC_HwReleaseReset() has to be called by the application.
**------------------------------------------------------------------------------
** Arguments:
**    lMaxStartupTimeMs -    Max startup time for ABCC
**
** Returns:
**    ABCC_ErrorCodeType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_ErrorCodeType ABCC_StartDriver( UINT32 lMaxStartupTimeMs );

/*------------------------------------------------------------------------------
** Stops the driver and puts it into SHUTDOWN state. The ABCC will be reset.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_ShutdownDriver( void );

#if ABCC_CFG_DRV_ASSUME_FW_UPDATE_ENABLED
/*------------------------------------------------------------------------------
** If the driver failed the initial communication with the ABCC it can be
** assumed that the ABCC is performing a firmware update. This function should
** be called if ABCC_isReadyForCommunication has returned ABCC_ASSUME_FW_UPDATE
** if interrupts are enabled or after initial communication has been attempted
** but the watchdog timeout has been triggered if interrupts are disabled.
**
** This can only be initiated once to avoid waiting endlessly for a firmware
** update to complete in case something else is preventing the ABCC from
** starting.
**------------------------------------------------------------------------------
** Arguments:
**    lTimeoutMs - Time in milliseconds to wait for firmware update to complete.
**
**
** Returns:
**    TRUE if this is the first attempt to wait for firmware update, FALSE if
**    firmware update has been assumed at least once already.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_WaitForFwUpdate( UINT32 lTimeoutMs );
#endif

/*------------------------------------------------------------------------------
** This function shall be polled after ABCC_StartDriver() has been executed
** until ABCC_READY_FOR_COMMUNICATION is returned. This indicates that the ABCC
** is ready for communication and the ABCC setup sequence is started.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    ABCC_CommunicationStateType:
**    ( see description of ABCC_CommunicationStateType )
**
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_CommunicationStateType ABCC_isReadyForCommunication( void );

/*------------------------------------------------------------------------------
** This function should be called from inside the ABCC interrupt routine to
** acknowledge and handle received ABCC events (Triggered by IRQ_n on the
** abcc application interface).
** The user configuration defines, ABCC_CFG_INT_ENABLE_MASK and
** ABCC_CFG_HANDLE_IN_ABCC_ISR_MASK, allows configuration of which events to
** handle by the ISR and which events to pass on to the application
** (ABCC_CbfEvent()).
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ( *ABCC_ISR )( void );

/*------------------------------------------------------------------------------
** This function is responsible for handling all timers for the ABCC-driver. It
** is recommended to call this function on a regular basis from a timer
** interrupt. Without this function no timeout and watchdog functionality will
** work. This function can be called after ABCC_StartDriver() has been called.
**------------------------------------------------------------------------------
** Arguments:
**    iDeltaTimeMs - Milliseconds since last call.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_RunTimerSystem( const INT16 iDeltaTimeMs );

/*------------------------------------------------------------------------------
** This function returns the uptime of the driver.
**
** Note! ABCC_RunTimerSystem() must be called regularly for this function to
** return a proper time.
** Note! The uptime counter will reset if the driver is restarted.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Uptime of the driver in milliseconds.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT64 ABCC_GetUptimeMs( void );

/*------------------------------------------------------------------------------
** ABCC hardware reset.
** Note! This function will only set reset pin to low. It the responsibility of
** the caller to make sure that the reset time is long enough.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_HWReset( void );

/*------------------------------------------------------------------------------
** Releases the ABCC reset.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_HWReleaseReset( void );

/*------------------------------------------------------------------------------
** This function drives the ABCC driver sending and receiving mechanism.
** The driver must be ready for communication before this function is called
** (ABCC_isReadyForCommunication() must be TRUE). This function could be called
** cyclically or be based on events from the ABCC. If all events are handled in
** the interrupt context then there is no need to call this function.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    ABCC_ErrorCodeType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_ErrorCodeType (*ABCC_RunDriver)( void );

/*------------------------------------------------------------------------------
** This function should be called by the application when the last response from
** the user specific setup has been received. This will end the ABCC setup
** sequence and ABCC_SETUP_COMPLETE will be sent. The user specific setup is a
** part of the ABCC setup sequence and it is initiated by the driver by calling
** the ABCC_CbfUserInitReq() function.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_UserInitComplete( void );

/*------------------------------------------------------------------------------
** Sends a command message to the ABCC.
** The function ABCC_GetCmdMsgBuffer() must be used to allocate the message
** buffer. It is OK to re-use a previously received response buffer
** The driver will use the sourceId to map the response to the correct response
** handler. ABCC_GetNewSourceId() could be used to provide an new source id.
** Example where ABCC_CbfMessageReceived() function is used as response handler:
**
** eResp = ABCC_SendCmdMsg( psMsg, ABCC_CbfMessageReceived );
**------------------------------------------------------------------------------
** Arguments:
**    psCmdMsg     - Pointer to the command message.
**    pnMsgHandler - Pointer to the function to handle the response
**                   message.
**
** Returns:
**    ABCC_ErrorCodeType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_ErrorCodeType ABCC_SendCmdMsg( ABP_MsgType* psCmdMsg,
                                            ABCC_MsgHandlerFuncType pnMsgHandler );

/*------------------------------------------------------------------------------
** Retrieves the number of entries left in the command queue.
** Note! When sending a message the returned status must always be checked to
** verify that the message has in fact been sent.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Number of queue entries left in the command queue
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_GetCmdQueueSize( void );

/*------------------------------------------------------------------------------
** Sends a response message to the ABCC.
** Note! The received command buffer can be reused as a response buffer. If a
** new buffer is used then the function ABCC_GetCmdMsgBuffer() must be used to
** allocate the buffer.
**------------------------------------------------------------------------------
** Arguments:
**    psMsgResp - Pointer to the message.
**
** Returns:
**    ABCC_ErrorCodeType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_ErrorCodeType ABCC_SendRespMsg( ABP_MsgType* psMsgResp );

/*------------------------------------------------------------------------------
** Sends a segmented response message to the ABCC.
** Either the supplied payload buffer holds the entire message data to be sent or
** the caller can implement a callback function to supply data blocks when
** requested. A combination of both methods are also allowed where pxData holds
** the first block to be sent.
** The callback function could be useful if the data to be sent is located on
** different parts in the memory. Memory is saved since a separate buffer holding
** the entire message is not required.
**------------------------------------------------------------------------------
** Arguments:
**    psReqMsgHeader - Pointer to request message header.
**    bRspCmdExt0    - Value of command extension 0 to use in response segments.
**                     Set to same value as command extension 0 in
**                     psReqMsgHeader if there are no valid values defined for
**                     this command response.
**    pxData         - Pointer to first message payload block.
**    lSize          - Size of first payload block.
**    pnNext         - Callback to request the next data block.
**                     NULL if all data is supplied in pxData.
**    pnDone         - Callback to indicate that the entire message is sent.
**    pxObject       - User defined object. Forwarded as parameter in pnDone and
**                     pnNext callback functions.
**
** Returns:
**    ABCC_ErrorCodeType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_ErrorCodeType ABCC_StartServerRespSegmentationSession( const ABP_MsgHeaderType* psReqMsgHeader,
                                                                    UINT8 bRspCmdExt0,
                                                                    const void* pxData,
                                                                    UINT32 lSize,
                                                                    ABCC_SegMsgHandlerNextBlockFuncType pnNext,
                                                                    ABCC_SegMsgHandlerDoneFuncType pnDone,
                                                                    const void* pxObject );

/*------------------------------------------------------------------------------
** Sends a remap response to the ABCC. When the response is sent the new process
** data sizes will be set and the function ABCC_CbfRemapDone() will be called to
** indicate the change.
**------------------------------------------------------------------------------
** Arguments:
**    psMsgResp       - Pointer to the response message.
**    iNewReadPdSize  - RdPd size when the remap is done.
**    iNewWritePdSize - WrPd size when the remap is done.
**
** Returns:
**    ABCC_ErrorCodeType
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_REMAP_SUPPORT_ENABLED
EXTFUNC ABCC_ErrorCodeType ABCC_SendRemapRespMsg( ABP_MsgType* psMsgResp,
                                                  UINT16 iNewReadPdSize,
                                                  const UINT16 iNewWritePdSize );
#endif

/*------------------------------------------------------------------------------
** Get the current application status.
** Note! This information is only supported in SPI and parallel operating mode.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    The application status of the ABCC
**------------------------------------------------------------------------------
*/
EXTFUNC ABP_AppStatusType ABCC_GetAppStatus( void );

/*------------------------------------------------------------------------------
** Sets the current application status.
** Note! This information is only supported in SPI and parallel operating mode.
** When used for other operating modes the call has no effect.
**------------------------------------------------------------------------------
** Arguments:
**    eAppStatus        - Current application status
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SetAppStatus( ABP_AppStatusType eAppStatus  );

/*------------------------------------------------------------------------------
** Retrieves a message buffer. This function MUST be used when allocating
** message buffers. The size of the buffer is controlled by
** ABCC_CFG_MAX_MSG_SIZE.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    ABP_MsgType* - Pointer to the message buffer.
**                   NULL is returned if no resource is available.
**------------------------------------------------------------------------------
*/
EXTFUNC ABP_MsgType* ABCC_GetCmdMsgBuffer( void );

/*------------------------------------------------------------------------------
** Returns the message buffer to the driver's message pool.
** Note! This function may only be used in combination with
** ABCC_TakeMsgBufferOwnership().
**------------------------------------------------------------------------------
** Arguments:
**    ppsBuffer - Pointer to the message buffer that was freed.
**                The buffer pointer will be set to NULL.
**
** Returns:
**    ABCC_ErrorCodeType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_ErrorCodeType ABCC_ReturnMsgBuffer( ABP_MsgType** ppsBuffer );

/*------------------------------------------------------------------------------
** Takes the ownership of the message buffer. The driver will not free this
** buffer when returning from e.g. a response callback. It is the user's
** responsibility to free this buffer when it is not needed anymore by using
** ABCC_ReturnMsgBuffer().
**------------------------------------------------------------------------------
** Arguments:
**   psMsg - Pointer to the message buffer to take ownership of
**
** Returns:
**    ABCC_StatusType
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_TakeMsgBufferOwnership( ABP_MsgType* psMsg );

/*------------------------------------------------------------------------------
** Reads the module ID.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    ModuleId
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_ReadModuleId( void );

/*------------------------------------------------------------------------------
** Detects if a module is present. If the ABCC Module detect pins on the host
** connector is not connected (ABCC_CFG_MOD_DETECT_PINS_CONN shall be defined)
** this interface will always return TRUE.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    TRUE  - Module detected.
**    FALSE - No module detected
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_ModuleDetect( void );

/*------------------------------------------------------------------------------
** Reads the module capability. This function is only supported by the ABCC40
** parallel operating mode.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Module capability
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_DRV_PARALLEL_ENABLED
EXTFUNC UINT16 ABCC_ModCap( void );
#endif

/*------------------------------------------------------------------------------
** Reads the LED status. Only supported in SPI and parallel operating mode.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    LED status according to the software design guide.
**------------------------------------------------------------------------------
*/
#if ( ABCC_CFG_DRV_PARALLEL_ENABLED || ABCC_CFG_DRV_SPI_ENABLED )
EXTFUNC UINT16 ABCC_LedStatus( void );
#endif

/*------------------------------------------------------------------------------
** Returns if the first application to ABCC command is waiting for a response
** or not.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    TRUE if first command is waiting for a response, else FALSE.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_IsFirstCommandPending( void );

/*------------------------------------------------------------------------------
** Reads the current Anybus state.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Anybus state
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_AnbState( void );

/*------------------------------------------------------------------------------
** Returns the current status of the supervision bit.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    TRUE  - Is supervised by another network device.
**    FALSE - Not supervised.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_IsSupervised( void );

/*------------------------------------------------------------------------------
** Retrieves the ABCC firmware version.
** This function will return a valid value after ABCC_CbfAdiMappingReq has been
** called by the driver. If called earlier the function will return 0xFF for
** each version field which indicates that the version is unknown.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    ABCC firmware version
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_FwVersionType ABCC_FirmwareVersion( void );

/*------------------------------------------------------------------------------
** Retrieves the network type.
** This function will return a valid value after ABCC_CbfAdiMappingReq has been
** called by the driver. If called earlier the function will return 0xFFFF which
** indicates that the network is unknown. The different newtwork types could
** be found in abp.h.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Network type (0xFFFF if the network is unknown)
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_NetworkType( void );

/*------------------------------------------------------------------------------
** Retrieves the module type.
** This function will return a valid value after ABCC_CbfAdiMappingReq has been
** called by the driver. If called earlier the function will return 0xFFFF which
** indicates that the moduleType is unknown. The different module types types
** could be found in abp.h.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Module type (0x04XX for Anybus-CC modules).
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_ModuleType( void );

/*------------------------------------------------------------------------------
** Retrieves the network format.
** This function will return a valid value after ABCC_CbfAdiMappingReq has been
** called by the driver. If called earlier the function will return NET_UNKNOWN.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Network format type (NET_LITTLEENDIAN, NET_BIGENDIAN).
**------------------------------------------------------------------------------
*/
EXTFUNC NetFormatType ABCC_NetFormatType( void );

/*------------------------------------------------------------------------------
** Retrieves the parameter support.
** This function will return a valid value after ABCC_CbfAdiMappingReq has been
** called by the driver. If called earlier PARAMETR_UNKNOWN will be returned.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    ParamemterSupportType
**------------------------------------------------------------------------------
*/
EXTFUNC ParameterSupportType ABCC_ParameterSupport( void );

/*------------------------------------------------------------------------------
** This function will call ABCC_SYS_GetOpmode() to read the operating mode from
** HW. If the operation is known and fixed or in any other way decided by the
** application this function could be ignored.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    The ABCC40 4 bit operating mode according to abp.h
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_GetOpmode( void );

/*******************************************************************************
** Callback Functions.
** These function must be implemented by the application. The context of the
** callback will differ depending on implementation.
** If, for example, the read process data is chosen to be interrupt driven and
** the message handling chosen to be polled ( see ABCC_CFG_INT_ENABLE_MASK and
** HANDLE_ABCC_CFG_IN_ABCC_ISR in ABCC_CFG_def.h ), the  ABCC_CbfNewReadPd()
** will be called from interrupt context and ABCC_CbfReceiveMsg() will be called
** from the same context as ABCC_RunDriver().
********************************************************************************
*/

/*------------------------------------------------------------------------------
** This function is called from ABCC_ISR() when events specified in
** ABCC_CFG_INT_ENABLE_MASK_X have occurred. The function returns a mask of
** ABCC_ISR_EVENT_X bits with the currently active events that has not already
** been handled by the ISR itself. What interrupt to be handled by the ISR is
** defined in the ABCC_CFG_HANDLE_INT_IN_ISR_MASK.
** This function is always called from interrupt context.
**------------------------------------------------------------------------------
** Arguments:
**    iEvents - Mask according to the ISR event bits ABCC_ISR_EVENT_X
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfEvent( UINT16 iEvents );

/*------------------------------------------------------------------------------
** If sync is supported this function will be invoked at the sync event.
** The function is executed in interrupt context. If the separate sync pin in
** the ABCC interface is used this function shall be called from the interrupt
** handler. If the ABCC interrupt is used the driver will call this function.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_SYNC_ENABLED
EXTFUNC void ABCC_CbfSyncIsr( void );
#endif


/*------------------------------------------------------------------------------
** This function needs to be implemented by the application. The function is
** called to trigger a user specific setup during the ABCC setup state. The ABCC
** driver will remain in ABCC_CFG_INIT state until ABCC_UserInitComplete() is called
** by the application.  If no user specific setup is required,
** ABCC_UserInitComplete() must be called inside this function otherwise setup
** complete will never be sent.
**
** This function call will be invoked in same context as the read message handling.
** (See comment for callback section above)
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfUserInitReq( void );

/*------------------------------------------------------------------------------
** A message has been received from the ABCC. This is the receive function for
** all received commands from the ABCC. It could also be used as a response
** handler if passed on as an argument to the ABCC_SendCmdMsg() function.
** Regarding callback context, see comment for callback section above.
**------------------------------------------------------------------------------
** Arguments:
**    psReceivedMsg       - Pointer to received message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfReceiveMsg( ABP_MsgType* psReceivedMsg );

/*------------------------------------------------------------------------------
** This function needs to be implemented by the application. The function
** updates the current write process data. The data must be copied into the
** buffer before returning from the function.
** The data will only be sent to the ABCC if the return value is TRUE.
** Regarding callback context, see comment for callback section above.
**------------------------------------------------------------------------------
** Arguments:
**    pxWritePd - Pointer to the process data to be sent.
**
** Returns:
**    TRUE  - If the process data has been changed since last call.
**    FALSE - Process data not changed.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_CbfUpdateWriteProcessData( void* pxWritePd );

/*------------------------------------------------------------------------------
** This function needs to be implemented by the application. The function is
** called when new process data has been received. The process data needs to
** be copied to the application ADI:s before returning from the function. Note
** that the data may not be changed since last time.
** Regarding callback context, see comment for callback section above.
**------------------------------------------------------------------------------
** Arguments:
**    pxReadPd - Pointer to the received process data.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfNewReadPd( void* pxReadPd );

/*------------------------------------------------------------------------------
** This function needs to be implemented by the application. The function is
** called when communication with the ABCC module has been lost. The watchdog
** timeout is defined by ABCC_CFG_WD_TIMEOUT_MS.
** Note! No watch functionality is provided for parallel 8/16 bit operation
** mode.
** This function is invoked in the same context as ABCC_RunTimerSystem().
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ( ABCC_CFG_DRV_SPI_ENABLED || ABCC_CFG_DRV_SERIAL_ENABLED )
EXTFUNC void ABCC_CbfWdTimeout( void );
#endif

/*------------------------------------------------------------------------------
** This function needs to be implemented by the application. The function
** indicates that we recently had an ABCC watchdog timeout but now the
** communication is working again.
** This function will be invoked from same context as the receive handling.
** (see comment for callback section above).
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ( ABCC_CFG_DRV_SPI_ENABLED || ABCC_CFG_DRV_SERIAL_ENABLED )
EXTFUNC void ABCC_CbfWdTimeoutRecovered( void );
#endif

/*------------------------------------------------------------------------------
** This function need to be implemented by the application. The function is
** called when the driver is about to start the automatic process data mapping.
** If no automatic configuration is desired then the pointers are set to NULL.
** Otherwise the pointers are set to point at the structures containing mapping
** information. The mapping structures are defined in
** abcc_application_data_interface.h. This function will be invoked in same
** context as the read message handling. (See comment for callback section
** above)
**------------------------------------------------------------------------------
** Arguments:
**    ppsAdiEntry   - Pointer to the requested configuration structure pointer.
**    ppsDefaultMap - Pointer to default mapping table.
**
** Returns:
**    Number of Adi:s in the psAdiEntry table.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_CbfAdiMappingReq( const AD_AdiEntryType** const ppsAdiEntry,
                                      const AD_MapType**      const ppsDefaultMap );

/*------------------------------------------------------------------------------
** This function needs to be implemented by the application.
** The context of the call is depending on where the error has occured.
**------------------------------------------------------------------------------
** Arguments:
**    eSeverity  - Severity of the event (see ABCC_SeverityType).
**    iErrorCode - Error code.
**    lAddInfo   - Depending on error different additional information can be
**                 added.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfDriverError(  ABCC_SeverityType eSeverity,
                                   ABCC_ErrorCodeType  iErrorCode,
                                   UINT32  lAddInfo );

/*------------------------------------------------------------------------------
** This callback is invoked if the anybus changes state
** See ABP_AnbStateType in abp.h for more information.
**
** Regarding callback context, see comment for callback section above.
**------------------------------------------------------------------------------
** Arguments:
**    bNewAnbState   - New anybus state
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfAnbStateChanged( ABP_AnbStateType bNewAnbState );

/*******************************************************************************
** REMAP Related functions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** This callback is invoked when REMAP response is successfully sent to the
** ABCC.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_REMAP_SUPPORT_ENABLED
EXTFUNC void ABCC_CbfRemapDone( void );
#endif

/*******************************************************************************
** Event related functions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** This function is called to trigger a RdPd read. If the read process data is
** available then a call to the function ABCC_CbfNewReadPd() will be triggered.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_TriggerRdPdUpdate( void );

/*------------------------------------------------------------------------------
** This function is called to trigger a message receive read. If a read message
** is available then the corresponding message handler will be called.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_TriggerReceiveMessage( void );

/*------------------------------------------------------------------------------
** This function indicates that new process data from the application is
** available and will be sent to the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void (*ABCC_TriggerWrPdUpdate)( void );

/*------------------------------------------------------------------------------
** Check if current anybus status has changed.
** If the status is changed ABCC_CbfAnbStateChanged() will be invoked.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_TriggerAnbStatusUpdate( void );

/*------------------------------------------------------------------------------
** Checks if there are any messages to send.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_TriggerTransmitMessage( void );

/*******************************************************************************
** Message support functions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** This function fills an ABCC message with parameters to get an attribute.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg      - Pointer to message buffer.
**    bObject    - Object number.
**    iInstance  - Instance number.
**    bAttribute - Attribute number.
**    bSourceId  - Source identifier
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_GetAttribute( ABP_MsgType* psMsg,
                                UINT8 bObject,
                                UINT16 iInstance,
                                UINT8 bAttribute,
                                UINT8 bSourceId );

/*------------------------------------------------------------------------------
** This function fills an ABCC message with parameters in order to set an
** attribute.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg      - Pointer to message buffer.
**    bObject    - Object number.
**    iInstance  - Instance number.
**    bAttribute - Attribute number.
**    bVal       - Value to set.
**    bSourceId  - Source identifier.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SetByteAttribute( ABP_MsgType* psMsg,
                                    UINT8 bObject,
                                    UINT16 iInstance,
                                    UINT8 bAttribute,
                                    UINT8 bVal,
                                    UINT8 bSourceId );

/*------------------------------------------------------------------------------
** This function sets the input arguments to the ABCC message header correctly.
** The data must be copied to message data buffer separately.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg      - Pointer to message buffer.
**    bObject    - Object number.
**    iInstance  - Instance number.
**    bAttribute - Attribute number.
**    eService   - Message command
**    iDataSize  - Size of the message data in bytes
**    bSourceId  - Source identifier.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SetMsgHeader( ABP_MsgType* psMsg,
                                UINT8 bObject,
                                UINT16 iInstance,
                                UINT8 bAttribute,
                                ABP_MsgCmdType eService,
                                UINT16 iDataSize,
                                UINT8 bSourceId );

/*------------------------------------------------------------------------------
** This function verifies an ABCC response message.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg - Pointer to message buffer.
**
** Returns:
**    ABCC_ErrorCodeType.
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_ErrorCodeType ABCC_VerifyMessage( const ABP_MsgType* psMsg );

/*------------------------------------------------------------------------------
** This function returns a new source id that could used when sending a command
** message. It is guaranteed be unique if this function is used every time a new
** command is sent. The alternative would be that the user uses fixed source
** id:s.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    New SourceId
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_GetNewSourceId( void );

/*------------------------------------------------------------------------------
** This function returns the size of an ABP data type.
**------------------------------------------------------------------------------
** Arguments:
**    bDataType - Data type number.
**
** Returns:
**    Data type size in bytes.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_GetDataTypeSize( UINT8 bDataType );

/*------------------------------------------------------------------------------
** This function returns the size of an ABP data type in bits.
**------------------------------------------------------------------------------
** Arguments:
**    bDataType - Data type number.
**
** Returns:
**    Data type size in bits.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_GetDataTypeSizeInBits( UINT8 bDataType );

/*------------------------------------------------------------------------------
** Get size of the message channel in number of octets.
**
** Note that his isn't necessarily the same as the maximum message size as the
** user can configure the driver to use a smaller message size, use
** ABCC_GetMaxMessageSize() for that purpose.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Size of the message channel
**    ABP_MAX_MSG_255_DATA_BYTES or ABP_MAX_MSG_DATA_BYTES
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_GetMessageChannelSize( void );

/*------------------------------------------------------------------------------
** Get maximum data size that can be used in the data portion of a message in
** number of octets.
**
** The useable message channel size is limited by the minimum value of the
** message size supported by the operating mode and ABCC_CFG_MAX_MSG_SIZE.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Maximum message size
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_GetMaxMessageSize( void );

#endif  /* inclusion lock */
