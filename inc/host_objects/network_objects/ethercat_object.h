/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 0401fde (2024-11-13)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** This is the public header file for the EtherCat object.
********************************************************************************
*/
#ifndef ECT_H
#define ECT_H

/*------------------------------------------------------------------------------
** EtherCAT states
**------------------------------------------------------------------------------
*/
#if ECT_IA_ETHERCAT_STATE_ENABLE

typedef enum ECT_State
{
  ECT_STATE_INIT    = 0x01,
  ECT_STATE_PREOP   = 0x02,
  ECT_STATE_BOOT    = 0x03,
  ECT_STATE_SAFEOP  = 0x04,
  ECT_STATE_OP      = 0x08
}
ECT_StateType;

#endif

/*------------------------------------------------------------------------------
** void ECT_ProcessCmdMsg()
** Processes commands sent to the ECT object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       ECT_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
EXTFUNC void ECT_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#if ECT_IA_ETHERCAT_STATE_ENABLE
/*------------------------------------------------------------------------------
** ECT_StateType ECT_GetActualEctState()
** Retreives the last EtherCAT state set by the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       The last EtherCAT state set by ABCC.
**
** Usage:
**       ECT_GetActualEctState( );
**------------------------------------------------------------------------------
*/
EXTFUNC ECT_StateType ECT_GetActualEctState( void );
#endif

#endif  /* inclusion lock */
