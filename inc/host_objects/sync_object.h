/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version edc67ee (2024-10-25)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Sync Object 0xEE - Public interfaces
********************************************************************************
*/
#ifndef SYNC_OBJ_H
#define SYNC_OBJ_H

#include "abcc_types.h"
#include "abp.h"
#include "abcc_application_data_interface.h"

/*------------------------------------------------------------------------------
** Structure describing the sync modes of attribute 7, 'Sync mode'.
**------------------------------------------------------------------------------
*/
typedef enum SYNC_SyncMode
{
   SYNC_MODE_NONSYNCHRONOUS = 0,
   SYNC_MODE_SYNCHRONOUS = 1
}
SYNC_SyncModeType;

/*------------------------------------------------------------------------------
** Get the currently configured cycle time
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Cycle time in nanoseconds
**------------------------------------------------------------------------------
*/
EXTFUNC UINT32 SYNC_GetCycleTime( void );

/*------------------------------------------------------------------------------
** Get the currently configured input capture time
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Input capture time in nanoseconds
**------------------------------------------------------------------------------
*/
EXTFUNC UINT32 SYNC_GetInputCaptureTime( void );

/*------------------------------------------------------------------------------
** Get the currently configured sync mode
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Sync mode of type SYNC_SyncModeType.
**------------------------------------------------------------------------------
*/
EXTFUNC SYNC_SyncModeType SYNC_GetMode( void );

/*------------------------------------------------------------------------------
** Get the currently configured output valid time
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Output valid time in nanoseconds
**------------------------------------------------------------------------------
*/
EXTFUNC UINT32 SYNC_GetOutputValidTime( void );

/*------------------------------------------------------------------------------
** Process an object request against the SYNC Object.
**------------------------------------------------------------------------------
** Arguments:
**    psMsgBuffer      - Pointer to ABCC command message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void SYNC_ProcessCmdMsg( ABP_MsgType* psNewMessage );

/*------------------------------------------------------------------------------
** Updates the input processing time reported to the ABCC upon request.
** This value may be changed in runtime to reflect the timing required for the
** current process data map.
**------------------------------------------------------------------------------
** Arguments:
**    lInputProcTimeNs - Input processing time in nanoseconds
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void SYNC_SetInputProcessingTime( UINT32 lInputProcTimeNs );

/*------------------------------------------------------------------------------
** Updates the minimum cycle time reported to the ABCC upon request.
** This value may be changed in runtime to reflect the timing required for the
** current process data map.
**------------------------------------------------------------------------------
** Arguments:
**    lMinCycleTimeNs - Minimum cycle time in nanoseconds
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void SYNC_SetMinCycleTime( UINT32 lMinCycleTimeNs );

/*------------------------------------------------------------------------------
** Updates the output processing time reported to the ABCC upon request.
** This value may change in runtime to reflect the timing required for the
** current process data map.
**------------------------------------------------------------------------------
** Arguments:
**    lOutputProcTimeNs - Output processing time in nanoseconds
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void SYNC_SetOutputProcessingTime( UINT32 lOutputProcTimeNs );

#endif
