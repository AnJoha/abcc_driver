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
** This is the public header file for the CRC calculation routines.
********************************************************************************
*/

#ifndef CRC32_H
#define CRC32_H

#include "abcc_config.h"
#include "abcc_types.h"

/*------------------------------------------------------------------------------
** CRC_Crc32()
**
** Calculates a CRC32 checksum on the indicated bytes including transforming
** input bytes from LSB to MSB for the SPI use.
**------------------------------------------------------------------------------
** Inputs:
**    pbBufferStart            - Where to start the calculation.
**    iLength                  - The amount of bytes to include.
**
** Outputs:
**    Returns                  - The calculated CRC32 checksum for the SPI.
**
** Usage:
**    iCrc = CRC_Crc16( pbStart, 20 );
**------------------------------------------------------------------------------
*/
EXTFUNC UINT32 CRC_Crc32( UINT16* piBufferStart, UINT16 iLength );

#endif  /* inclusion lock */
