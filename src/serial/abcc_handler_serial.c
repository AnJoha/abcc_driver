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
*/
#include "abcc_config.h"

#if ABCC_CFG_DRV_SERIAL_ENABLED

#include "abcc_types.h"
#include "../abcc_driver_interface.h"
#include "abp.h"
#include "abcc.h"
#include "../abcc_link.h"
#include "abcc_system_adaptation.h"
#include "abcc_log.h"
#include "../abcc_handler.h"
#include "../abcc_timer.h"
#include "../abcc_command_sequencer.h"


/*------------------------------------------------------------------------------
** ABCC_RunDriver()
**------------------------------------------------------------------------------
*/
ABCC_ErrorCodeType ABCC_SerRunDriver( void )
{
   ABCC_MainStateType eMainState;

   eMainState = ABCC_GetMainState();

   if( eMainState < ABCC_DRV_SETUP )
   {
      if( eMainState != ABCC_DRV_ERROR )
      {
         ABCC_LOG_ERROR( ABCC_EC_INCORRECT_STATE,
            (UINT32)eMainState,
            "ABCC_RunDriver() called in incorrect state (%d)\n",
            eMainState );
      }
      return( ABCC_EC_INCORRECT_STATE );
   }

   ABCC_LinkRunDriverRx();
   ABCC_TriggerRdPdUpdate();
   ABCC_TriggerAnbStatusUpdate();
   ABCC_TriggerReceiveMessage();
#if ABCC_CFG_DRV_CMD_SEQ_ENABLED
   ABCC_CmdSequencerExec();
#endif
   ABCC_CheckWrPdUpdate();
   ABCC_LinkCheckSendMessage();
   pnABCC_DrvRunDriverTx();

   return( ABCC_EC_NO_ERROR );
}
#endif /* End of #if ABCC_CFG_DRV_SERIAL_ENABLED */
