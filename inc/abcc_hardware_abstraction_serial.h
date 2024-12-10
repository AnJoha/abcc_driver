/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 0401fde (2024-11-13)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2013-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** file_description
********************************************************************************
*/

#ifndef ABCC_SYS_ADAPT_SER_
#define ABCC_SYS_ADAPT_SER_
#include "abcc_config.h"
#include "abcc_types.h"

/*------------------------------------------------------------------------------
** Function pointer type for RX telegram ready callback.
** **------------------------------------------------------------------------------
*/
typedef void ( *ABCC_SYS_SerDataReceivedCbfType )( void );

/*------------------------------------------------------------------------------
** ABCC_SYS_SerRegDataReceived()
** Registers a callback function that indicates that new data has been received
** on the serial channel. The received data shall be located in the buffer
** provided when ABCC_SYS_SerSendRecieved() was called.
** The data in the buffer is valid until next time ABCC_SYS_SerSendReceive() is
** called.
**------------------------------------------------------------------------------
** Arguments:
**    pnDataReceived: Function to call when RX telegram (pong) is received.
**
** Returns:
**          -
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_SerRegDataReceived( ABCC_SYS_SerDataReceivedCbfType pnDataReceived );

/*------------------------------------------------------------------------------
** Send TX telegram and prepare for RX telegram reception. The the RX telegram
** will be stored in the pbRxDataBuffer.
** Note the data pbRxDataBuffer is not valid upon return of this function. When
** valid data RX data is available the callback function registered in
** ABCC_SYS_SerRegDataReceived() will be called.
**------------------------------------------------------------------------------
** Arguments:
**             pxTxDataBuffer       Pointer to TX telegram
**             pxRxDataBuffer       Pointer to RX telegram.
**             iTxSize              Length of TX telegram
**             iRxSize              Expected length of RX telegram (in bytes)
** Returns:
**          None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_SerSendReceive( void* pxTxDataBuffer, void* pxRxDataBuffer,
                                      UINT16 iTxSize, UINT16 iRxSize );


/*------------------------------------------------------------------------------
** ABCC_SYS_SerRestart()
** Restart the serial driver. Typically used when telegram has timed out.
** Flush all buffers and restart communication and start waiting for RX telegram
** with length of latest provided RX telegram length
** (provided in ABCC_SYS_SerSendReceive()
**------------------------------------------------------------------------------
** Arguments:
**          None
** Returns:
**          None
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_SerRestart( void );


#endif  /* inclusion lock */
