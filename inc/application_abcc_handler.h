/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 003e7c1 (2024-11-29)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**    ABCC Driver    0401fde (2024-11-13)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2013-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** This is an example implementation of an application handler to control the
** ABCC module. It has support to initialize, run and reset/shutdown the ABCC.
** The state machine shall be executed cyclically and it returns its current
** status after every execution.
********************************************************************************
*/

#ifndef APPL_ABCC_HANDLER_H
#define APPL_ABCC_HANDLER_H
#include "abcc_types.h"
#include "string.h"

/*------------------------------------------------------------------------------
**  Error codes for example application:
**
**  APPL_NO_ERROR                  - No error
**  APPL_AD_PD_REDA_SIZE_ERR       - Total process data read size too large
**                                   Check ABCC_CFG_MAX_PROCESS_DATA_SIZE
**  APPL_AD_PD_WRITE_SIZE_ERR      - Total process data write too large
**                                   Check ABCC_CFG_MAX_PROCESS_DATA_SIZE
**  APPL_AD_TOO_MANY_READ_MAPPINGS - Read process data map has too many entries
**                                   Check AD_MAX_OF_READ_WRITE_TO_MAP.
**  APPL_AD_TOO_MANY_WRITE_MAPPINGS- Write process data map has too many entries
**                                   Check AD_MAX_OF_WRITE_WRITE_TO_MAP
**  APPL_AD_UNKNOWN_ADI            - Requested ADI could not be found
**------------------------------------------------------------------------------
*/
typedef enum APPL_ErrCode
{
   APPL_NO_ERROR = 0,
   APPL_AD_PD_READ_SIZE_ERR,
   APPL_AD_PD_WRITE_SIZE_ERR,
   APPL_AD_TOO_MANY_READ_MAPPINGS,
   APPL_AD_TOO_MANY_WRITE_MAPPINGS,
   APPL_AD_UNKNOWN_ADI
}
APPL_ErrCodeType;

/*------------------------------------------------------------------------------
**  Comm settings:
**
**  APPL_COMM_SETTING_AUTO         - Auto negotiation
**  APPL_COMM_SETTING_10HDX        - 10 Mbit/s half duplex
**  APPL_COMM_SETTING_10FDX        - 10 Mbit/s full duplex
**  APPL_COMM_SETTING_100HDX       - 100 Mbit/s half duplex
**  APPL_COMM_SETTING_100FDX       - 100 Mbit/s full duplex
**  APPL_COMM_SETTING_AUTO_100FDX  - Auto negotiation with only 100 Mbit/s full
*                                    duplex in advertisment
**  APPL_COMM_SETTING_AUTO_1000FDX - Auto negotiation with only 1 Gbit/s full
*                                    duplex in advertisment
**------------------------------------------------------------------------------
*/
typedef enum APPL_CommSetting
{
   APPL_COMM_SETTING_AUTO = 0,
   APPL_COMM_SETTING_10HDX,
   APPL_COMM_SETTING_10FDX,
   APPL_COMM_SETTING_100HDX,
   APPL_COMM_SETTING_100FDX,
   APPL_COMM_SETTING_AUTO_100FDX,
   APPL_COMM_SETTING_AUTO_1000FDX,
}
APPL_CommSettingType;

/*------------------------------------------------------------------------------
** This function shall be called on cyclic bases from the main loop to handle
** the ABCC. It includes a state machine for handling reset, run, and shutdown
** of the driver.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    ABCC driver error code
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_ErrorCodeType APPL_HandleAbcc( void );

/*------------------------------------------------------------------------------
** This function will force the ABCC handler to shutdown the ABCC module
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_Shutdown( void );

/*------------------------------------------------------------------------------
** This function will force the ABCC handler to reset the ABCC
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_Reset( void );

/*------------------------------------------------------------------------------
** Application has encountered an unexpected error.
** The abcc main state machine will halt and indicate error.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_UnexpectedError( void );

/*------------------------------------------------------------------------------
** Set HW switch 1 value to the application. The value is used to generate the
** the node address or the IP address (192.168.0.X) depending on network.
**
** NOTE: For networks that uses an IP address and if this function is called
**       with address 0 no address will be set to the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**    bSwitchValue/iSwitchValue - Switch 1 value either as an 8 bit or 16 bit
**                                value depending on configuration.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if APPL_USE_16BIT_NODE_ADDR
EXTFUNC void APPL_SetAddress( UINT16 iSwitchValue );
#else
EXTFUNC void APPL_SetAddress( UINT8 bSwitchValue );
#endif

/*------------------------------------------------------------------------------
** Set HW switch 2 value to the application. The value is used to generate the
** baud rate for networks that implements network configuration object instance
** 2.
**------------------------------------------------------------------------------
** Arguments:
**    bSwitchValue - Switch 2 value
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_SetBaudrate( UINT8 bSwitchValue );

/*------------------------------------------------------------------------------
** Set Comm settings (speed/duplex) for ports.
**------------------------------------------------------------------------------
** Arguments:
**
**    eCommSettings1 - Settings for port 1
**    eCommSettings2 - Settings for port 2
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_SetCommSettings( APPL_CommSettingType eCommSettings1,
                                   APPL_CommSettingType eCommSettings2 );

#endif  /* inclusion lock */
