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
** ABCC driver error and debug functions.
********************************************************************************
*/

#include <stdarg.h>

#include "abcc_config.h"
#include "abcc.h"
#include "abcc_port.h"
#include "abcc_software_port.h"
#include "abcc_types.h"
#include "abp.h"
#include "abcc_debug_error.h"

/*
** Max number of errors to log
*/
#ifndef ABCC_DRV_CFG_MAX_NUM_ERR_LOG
#define ABCC_DRV_CFG_MAX_NUM_ERR_LOG 1
#endif

/*------------------------------------------------------------------------------
** ANSI color codes for prettier prints
**------------------------------------------------------------------------------
*/
#ifdef ABCC_CFG_DEBUG_LOG_COLORS_ENABLED
   #define ABCC_DEBUG_ANSI_COLOR_BLACK   "\x1b[30m"
   #define ABCC_DEBUG_ANSI_COLOR_RED     "\x1b[31m"
   #define ABCC_DEBUG_ANSI_COLOR_GREEN   "\x1b[32m"
   #define ABCC_DEBUG_ANSI_COLOR_YELLOW  "\x1b[33m"
   #define ABCC_DEBUG_ANSI_COLOR_BLUE    "\x1b[34m"
   #define ABCC_DEBUG_ANSI_COLOR_MAGENTA "\x1b[35m"
   #define ABCC_DEBUG_ANSI_COLOR_CYAN    "\x1b[36m"
   #define ABCC_DEBUG_ANSI_COLOR_WHITE   "\x1b[37m"
   #define ABCC_DEBUG_ANSI_COLOR_RESET   "\x1b[0m"
#else
   #define ABCC_DEBUG_ANSI_COLOR_BLACK   ""
   #define ABCC_DEBUG_ANSI_COLOR_RED     ""
   #define ABCC_DEBUG_ANSI_COLOR_GREEN   ""
   #define ABCC_DEBUG_ANSI_COLOR_YELLOW  ""
   #define ABCC_DEBUG_ANSI_COLOR_BLUE    ""
   #define ABCC_DEBUG_ANSI_COLOR_MAGENTA ""
   #define ABCC_DEBUG_ANSI_COLOR_CYAN    ""
   #define ABCC_DEBUG_ANSI_COLOR_WHITE   ""
   #define ABCC_DEBUG_ANSI_COLOR_RESET   ""
#endif

/*------------------------------------------------------------------------------
** Help macros for debugging and error reporting.
**------------------------------------------------------------------------------
*/

/*
** Report error to application depending on ABCC_CFG_ERR_REPORTING_ENABLED
*/
#if ABCC_CFG_ERR_REPORTING_ENABLED
#define ABCC_REPORT_ERROR( eSeverity, eErrorCode, lAddInfo ) ABCC_CbfDriverError( eSeverity, eErrorCode, lAddInfo )
#else
#define ABCC_REPORT_ERROR( eSeverity, eErrorCode, lAddInfo )
#endif

#if ( ABCC_CFG_DRV_SPI_ENABLED || ABCC_CFG_DRV_SERIAL_ENABLED )
#if ABCC_CFG_DEBUG_CRC_ERROR_CNT_ENABLED
UINT16   DEBUG_iCrcErrorCnt = 0;
#endif
#endif

/*
** Array to hold the logged error information.
*/
static UINT8 abcc_bErrCount = 0;
static ABCC_ErrInfoType abcc_asErrInfo[ ABCC_DRV_CFG_MAX_NUM_ERR_LOG ];

#ifdef ABCC_SYS_16_BIT_CHAR
#define ABCC_GetMsgCmdField( psMsg )   ( ABCC_GetLowAddrOct( (psMsg)->sHeader.iCmdReserved ) )
#else
#define ABCC_GetMsgCmdField( psMsg )   ( (psMsg)->sHeader.bCmd )
#endif

/*------------------------------------------------------------------------------
** Log error to memory.
** More human readable information will be available if
** ABCC_CFG_DEBUG_ERR_ENABLED is set to 1 (see ABCC_ErrInfoType) but this
** will have significant impact on the code size.
**------------------------------------------------------------------------------
** Arguments:
**    eSeverity         - Severity (see ABCC_SeverityType)
**    eErrorCode        - Error code (see ABCC_ErrorCodeType)
**    lAddInfo          - Additional info relevant for error
**    pacSeverity       - Pointer to severity in text format
**    pacErrorCode      - Pointer to error code in text format
**    pacAddInfo        - Pointer to additional info in text format
**    pacLoaction       - Pointer to file and line info text format
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_DEBUG_ERR_ENABLED
static void LogError( ABCC_SeverityType eSeverity,
                      ABCC_ErrorCodeType eErrorCode,
                      UINT32 lAddInfo,
                      char* pacSeverity,
                      char* pacErrorCode,
                      char* pacAddInfo,
                      char* pacFile,
                      INT32 lLine )
#else
static void LogError( ABCC_SeverityType eSeverity,
                      ABCC_ErrorCodeType eErrorCode,
                      UINT32 lAddInfo )
#endif
{
   abcc_asErrInfo[ abcc_bErrCount ].eErrorCode = eErrorCode;
   abcc_asErrInfo[ abcc_bErrCount ].eSeverity  = eSeverity;
   abcc_asErrInfo[ abcc_bErrCount ].lAddInfo   = lAddInfo;
#if ABCC_CFG_DEBUG_ERR_ENABLED
   abcc_asErrInfo[ abcc_bErrCount ].pacSeverity = pacSeverity;
   abcc_asErrInfo[ abcc_bErrCount ].pacErrorCode = pacErrorCode;
   abcc_asErrInfo[ abcc_bErrCount ].pacAddInfo = pacAddInfo;
   abcc_asErrInfo[ abcc_bErrCount ].pacFile = pacFile;
   abcc_asErrInfo[ abcc_bErrCount ].lLine = lLine;
#endif

   if( abcc_bErrCount != ( ABCC_DRV_CFG_MAX_NUM_ERR_LOG - 1 ) )
   {
      abcc_bErrCount++;
   }
}

#if ABCC_CFG_DEBUG_TIMESTAMPS_ENABLED
static void ABCC_DebugPrintTimestamp()
{
   UINT64 llUptime = ABCC_GetUptimeMs();

   ABCC_PORT_printf( ABCC_DEBUG_ANSI_COLOR_GREEN
                     "%02u:%02u:%02u.%03u: "
                     ABCC_DEBUG_ANSI_COLOR_RESET,
         (UINT16)( llUptime / 1000 / 60 / 60 % 24 ),
         (UINT8)( llUptime / 1000 / 60 % 60 ),
         (UINT8)( llUptime / 1000 % 60 ),
         (UINT16)( llUptime % 1000 ) );
}
#else
#define ABCC_DebugPrintTimestamp()
#endif

#if ABCC_CFG_DEBUG_TIMESTAMPS_ENABLED
void ABCC_DebugPrintf( const char *format, ... )
{
    va_list argp;

    ABCC_DebugPrintTimestamp();

    va_start(argp, format);
    ABCC_PORT_vprintf(format, argp);
    va_end(argp);
}
#endif

#if ABCC_CFG_DEBUG_MESSAGING_ENABLED
void ABCC_DebugPrintMsg( char* pcInfo, ABP_MsgType* psMsg )
{
   UINT16 i;
   UINT16 iDataSize;
   UINT8 bData;

   iDataSize = ABCC_GetMsgDataSize( psMsg );

   ABCC_DebugPrintTimestamp();

   ABCC_PORT_printf( "\n%s:\n", pcInfo );
   ABCC_PORT_printf( "[ MsgBuf:0x%p Size:0x%04x SrcId  :0x%02x DestObj:0x%02x\n  Inst  :0x%04x     Cmd :0x%02x   CmdExt0:0x%02x CmdExt1:0x%02x ]\n",
                     (void*)psMsg,
                     ABCC_GetMsgDataSize( psMsg ),
                     ABCC_GetMsgSourceId( psMsg ),
                     ABCC_GetMsgDestObj( psMsg ),
                     ABCC_GetMsgInstance( psMsg ),
                     ABCC_GetMsgCmdField( psMsg ),
                     ABCC_GetMsgCmdExt0( psMsg ),
                     ABCC_GetMsgCmdExt1( psMsg ) );

   ABCC_PORT_printf( ("[ ") );
   for( i = 0; i < iDataSize; i++ )
   {
      if( ( i % 16 ) == 15 )
      {
         ABCC_PORT_printf( ("\n  ") );
      }

      ABCC_GetMsgData8( psMsg, &bData, i );
      ABCC_PORT_printf( "0x%02x ", bData );
   }

   ABCC_PORT_printf( ( "]\n\n" ) );
}

void ABCC_DebugPrintMsgEvent( char* pcInfo, ABP_MsgType* psMsg )
{
   ABCC_DebugPrintf( "%s: MsgBuf:0x%p SrcId:0x%02x\n",
                     pcInfo, (void*)psMsg,
                     ABCC_GetMsgSourceId( psMsg ) );
}
#endif


UINT8 ABCC_GetDriverError( ABCC_ErrInfoType** ppacErrInfo )
{
   if( ppacErrInfo != NULL )
   {
      *ppacErrInfo = abcc_asErrInfo;
   }
   return( abcc_bErrCount );
}

#if ABCC_CFG_DEBUG_ERR_ENABLED
void ABCC_ErrorHandler( ABCC_SeverityType eSeverity,
                        ABCC_ErrorCodeType eErrorCode,
                        UINT32 lAddInfo,
                        char* pacSeverity,
                        char* pacErrorCode,
                        char* pacAddInfo,
                        char* pacFile,
                        INT32 lLine )
#else
void ABCC_ErrorHandler( ABCC_SeverityType eSeverity,
                        ABCC_ErrorCodeType eErrorCode,
                        UINT32 lAddInfo )
#endif
{
#if ABCC_CFG_DEBUG_ERR_ENABLED
   LogError( eSeverity, eErrorCode, lAddInfo,
             pacSeverity, pacErrorCode, pacAddInfo, pacFile, lLine );

   ABCC_DebugPrintf( "Severity  : %s\n"
                     "Error code: %s (ErrNo: %d)\n"
                     "Add info  : %s (0x%" PRIx32 ")\n"
                     "File      : %s (Line:%" PRId32 ")\n",
                     pacSeverity, pacErrorCode, eErrorCode,
                     pacAddInfo, lAddInfo, pacFile, lLine );
#else
   LogError( eSeverity, eErrorCode, lAddInfo );
#endif
   ABCC_REPORT_ERROR( eSeverity, eErrorCode, lAddInfo );

   if( eSeverity != ABCC_SEV_INFORMATION )
   {
     ABCC_SetMainStateError();
   }

   if( eSeverity == ABCC_SEV_FATAL )
   {
      volatile UINT8 bErrCount;
      volatile ABCC_ErrInfoType* pasErrInfo;
      bErrCount = ABCC_GetDriverError( (ABCC_ErrInfoType**)&pasErrInfo );
      ABCC_DebugPrintf( "FATAL: Enter endless loop\n%s", FileLine );
      (void)bErrCount;
      while( 1 ){}
   }
}


#if ABCC_CFG_DEBUG_HEXDUMP_MSG_ENABLED
void ABCC_DebugHexdumpMsg( char* pcInfo, ABP_MsgType* psMsg )
{
   UINT16   iIndex;
   UINT16   iSizeInBytes;

   ABCC_DebugPrintTimestamp();

   ABCC_PORT_printf( "%s", pcInfo );

   iSizeInBytes = ABCC_GetMsgDataSize( psMsg );

#ifdef ABCC_SYS_16_BIT_CHAR
   iSizeInBytes += sizeof( ABP_MsgHeaderType16 );

   iIndex = 0;
   while( iSizeInBytes > 0 )
   {
      UINT16   iData;

      iData = iLeTOi( ((UINT16*)psMsg)[ iIndex ] );

      ABCC_PORT_printf( "%02x", (UINT8)(iData & 0xFF) );
      iSizeInBytes--;
      if( iSizeInBytes > 0 )
      {
         ABCC_PORT_printf( "%02x", (UINT8)(( iData >> 8 ) & 0xFF) );
         iSizeInBytes--;
      }

      iIndex++;
   }
#else
   iSizeInBytes += sizeof( ABP_MsgHeaderType );

   for( iIndex = 0; iIndex < iSizeInBytes; iIndex++ )
   {
      ABCC_PORT_printf( "%02x", ((UINT8*)psMsg)[ iIndex ] );
   }
#endif

   ABCC_PORT_printf( "\n" );
}
#endif


#if ABCC_CFG_DEBUG_HEXDUMP_SPI_ENABLED
void ABCC_DebugHexdumpSpi( char* pcInfo, UINT16* piData, UINT16 iSizeInWords )
{
   ABCC_DebugPrintTimestamp();

   ABCC_PORT_printf( "%s", pcInfo );

   while( iSizeInWords )
   {
      ABCC_PORT_printf( "%04x", iTOiBe( *piData ) );
      piData++;
      iSizeInWords--;
   }

   ABCC_PORT_printf( "\n" );
}
#endif


#if ABCC_CFG_DEBUG_HEXDUMP_UART_ENABLED
void ABCC_DebugHexdumpUart( char* pcInfo, UINT8* pbData, UINT16 iSizeInBytes )
{
   ABCC_DebugPrintTimestamp();

   ABCC_PORT_printf( "%s", pcInfo );

   while( iSizeInBytes )
   {
      ABCC_PORT_printf( "%02x", *pbData );
      pbData++;
      iSizeInBytes--;
   }

   ABCC_PORT_printf( "\n" );
}
#endif
