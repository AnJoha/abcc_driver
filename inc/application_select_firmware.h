/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 390fce4 (2024-10-25)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**    ABCC Driver    edc67ee (2024-10-25)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
*/

#include "abcc_object_config.h"

#ifndef APPL_SELECT_FW_H
#define APPL_SELECT_FW_H

#include "abcc.h"

#if ANB_FSI_OBJ_ENABLE

/*******************************************************************************
** Public defines
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Enable/disable debugging of the select firmware feature.
**------------------------------------------------------------------------------
*/
#ifndef APPL_SELECT_FIRMWARE_DEBUG_ENABLED
#define APPL_SELECT_FIRMWARE_DEBUG_ENABLED 0
#endif

/*******************************************************************************
** Public typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Common Ethernet Loaded firmwares
**------------------------------------------------------------------------------
*/
typedef enum APPL_CommonEtnFirmware
{
    APPL_NW_TYPE_PROFINET = 0,
    APPL_NW_TYPE_ETHERNET_IP,
    APPL_NW_TYPE_ETHERCAT,
    APPL_NW_TYPE_MODBUS_TCP,
    APPL_NW_TYPE_LAST
}
APPL_CommonEtnFirmwareType;

/*------------------------------------------------------------------------------
** Function pointer definition which is used to report the result of the
** select firmware function.
**------------------------------------------------------------------------------
** Arguments:
**    eResult - ABCC error code indicating success or failure.
** Returns:
**    None
**------------------------------------------------------------------------------
*/
typedef void (*APPL_pnSelectFwResultCallback)( ABCC_ErrorCodeType eResult );

/*******************************************************************************
** Public globals
********************************************************************************
*/

/*******************************************************************************
** Public services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Some CompactCom versions are pre-loaded with firmware for the most common
** Ethernet network protocols, also refered to as "Common Ethernet Loaded".
**
** This function is used to copy a pre-loaded firmware to the CompactCom's
** firmware candidate area. The firmware to be copied shall be located in a
** folder named "/Network FW/" with the filename format
** ABCC_40_(EIP|PIR|ECT|EIT)_.*\.hiff. The provided callback is invoked after the
** operation has completed with either a success or error response. In case of
** a success response the Anybus CompactCom has to be restarted for the new
** firmware to be installed.
**------------------------------------------------------------------------------
** Arguments:
**    eFirmware - Firmware to be selected for update
**    pnResultCallback - Callback function to be called with the result of the
**                       operation.
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_SelectFirmware(
   APPL_CommonEtnFirmwareType eFirmware,
   APPL_pnSelectFwResultCallback pnResultCallback );

#endif

#endif /* inclusion lock */
