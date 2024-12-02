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
** Defines system specific interface for SPI
********************************************************************************
*/

#ifndef ABCC_SYS_ADAPT_SPI_
#define ABCC_SYS_ADAPT_SPI_
#include "abcc_config.h"
#include "abcc_types.h"

/*------------------------------------------------------------------------------
** Function pointer type for MISO frame ready callback.
** ------------------------------------------------------------------------------
*/
typedef void ( *ABCC_SYS_SpiDataReceivedCbfType )( void );

/*------------------------------------------------------------------------------
** ABCC_SYS_SpiRegDataReceived()
** As soon as the MISO frame is received by the low level SPI hardware driver,
** this registered function shall be called to indicate to the SPI driver that
** the MISO frame could be processed.
**------------------------------------------------------------------------------
** Arguments:
**    pnDataReceived Function to call when MISO frame is received.
**
** Returns:
**
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived );

/*------------------------------------------------------------------------------
** ABCC_SYS_SpiSendReceive()
** This function is called by the SPI driver when the MOSI frame shall be sent.
** Two buffers are provided, one with MOSI data frame to be sent and one buffer
** to store the received MISO frame.
** If the callback registered by ABCC_SYS_SpiRegDataReceived() is invoked within
** this function, indicating that the MISO frame is already received ,the MISO
** frame will be processed by the SPI driver on return of this function.
** Otherwise the MISO frame will be processed the next time the driver is polled
** (ABCC_RunDriver() ) after the MISO frame received callback is invoked.
**------------------------------------------------------------------------------
** Arguments:
**             pxSendDataBuffer     Pointer to MOSI Buffer to send
**             pxReceiveDataBuffer  Pointer to MISO Buffer.
**             iLength              Length of SPI frame ( in bytes )
** Returns:
**          None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength );

#endif  /* inclusion lock */
