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
** Interface for error handling and debugging.
********************************************************************************
*/

#ifndef ABCC_DEBUG_ERR_H_
#define ABCC_DEBUG_ERR_H_

#include <stdarg.h>
#include "abcc_config.h"
#include "abcc_types.h"
#include "abp.h"
#include "abcc_system_adaptation.h"
#include "abcc.h"
#include "abcc_port.h"
#include "abcc_handler.h"

/*
** DEBUG Level macros.
*/
#if ABCC_CFG_DEBUG_EVENT_ENABLED
#define DEBUG_EVENT( ... ) ABCC_DebugPrintf( __VA_ARGS__ )
#else
#define DEBUG_EVENT( ... )
#endif

#if ( ABCC_CFG_DRV_SPI_ENABLED || ABCC_CFG_DRV_SERIAL_ENABLED )
#if ABCC_CFG_DEBUG_CRC_ERROR_CNT_ENABLED
/*------------------------------------------------------------------------------
** CRC error counter for the SPI and UART application interfaces. It will be
** incremented every time a CRC mismatch is seen, it will roll over to zero
** when it reaches 0xFFFF, and is not cleared in case of a HW reset of the ABCC.
** The application can clear it at runtime by writing zero to it.
**------------------------------------------------------------------------------
*/
EXTVAR UINT16 DEBUG_iCrcErrorCnt;
#endif
#endif

#if ABCC_CFG_DEBUG_MESSAGING_ENABLED
/*------------------------------------------------------------------------------
** Prints ABCC message content using ABCC_DebugPrintf().
** Prints: Message buffer address, message header and message data
**------------------------------------------------------------------------------
** Arguments:
**    pcInfo - General information about the debug print.
**    psMsg  - ABCC message
** Returns:
**    None
**------------------------------------------------------------------------------
*/
void ABCC_DebugPrintMsg( char* pcInfo, ABP_MsgType* psMsg );

/*------------------------------------------------------------------------------
** Prints buffer address and source id for an ABCC message.
**------------------------------------------------------------------------------
** Arguments:
**    pcInfo - General information about the debug print.
**    psMsg  - ABCC message
** Returns:
**    None
**------------------------------------------------------------------------------
*/
void ABCC_DebugPrintMsgEvent( char* pcInfo, ABP_MsgType* psMsg );

#define ABCC_DEBUG_MSG_DATA( pcInfo, psMsg )  ABCC_DebugPrintMsg( pcInfo, psMsg )
#define ABCC_DEBUG_MSG_EVENT( pcInfo, psMsg ) ABCC_DebugPrintMsgEvent( pcInfo, psMsg )
#define ABCC_DEBUG_MSG_GENERAL( ... )         ABCC_DebugPrintf( __VA_ARGS__ )
#else
#define ABCC_DEBUG_MSG_DATA( pcInfo, psMsg )
#define ABCC_DEBUG_MSG_EVENT( pcInfo, psMsg )
#define ABCC_DEBUG_MSG_GENERAL( ... )
#endif

/*------------------------------------------------------------------------------
** Prints hexdumps of message, SPI, and UART data.
**------------------------------------------------------------------------------
** Arguments:
**    pcInfo - Tag to prepend to the output.
**
**    psMsg - Pointer to ABCC message buffer.
**
**    piData       - Pointer to SPI frame data.
**    iSizeInWords - SPI frame size in words.
**
**    pbData       - Pointer to UART telegram data.
**    iSizeInBytes - UART telegram size in bytes.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_DEBUG_HEXDUMP_MSG_ENABLED
EXTFUNC void ABCC_DebugHexdumpMsg( char* pcInfo, ABP_MsgType* psMsg );
#define ABCC_DEBUG_HEXDUMP_MSG( pcInfo, psMsg ) ABCC_DebugHexdumpMsg( pcInfo, psMsg )
#else
#define ABCC_DEBUG_HEXDUMP_MSG( pcInfo, psMsg )
#endif

#if ABCC_CFG_DEBUG_HEXDUMP_SPI_ENABLED
EXTFUNC void ABCC_DebugHexdumpSpi( char* pcInfo, UINT16* piData, UINT16 iSizeInWords );
#define ABCC_DEBUG_HEXDUMP_SPI( pcInfo, piData, iSizeInWords ) ABCC_DebugHexdumpSpi( pcInfo, piData, iSizeInWords )
#else
#define ABCC_DEBUG_HEXDUMP_SPI( pcInfo, piData, iSizeInWords )
#endif

#if ABCC_CFG_DEBUG_HEXDUMP_UART_ENABLED
EXTFUNC void ABCC_DebugHexdumpUart( char* pcInfo, UINT8* pbData, UINT16 iSizeInBytes );
#define ABCC_DEBUG_HEXDUMP_UART( pcInfo, pbData, iSizeInBytes ) ABCC_DebugHexdumpUart( pcInfo, pbData, iSizeInBytes )
#else
#define ABCC_DEBUG_HEXDUMP_UART( pcInfo, pbData, iSizeInBytes )
#endif

#endif
