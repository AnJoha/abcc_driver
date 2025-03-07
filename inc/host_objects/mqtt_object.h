/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version edc67ee (2024-10-25)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2018-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** This is the public header file for the MQTT object.
********************************************************************************
*/
#ifndef MQTT_OBJ_H
#define MQTT_OBJ_H

#if MQTT_CMD_PUBLISH_CFG_ENABLE
/*------------------------------------------------------------------------------
** Retrieves the Publish configuration of a requested dataset.
**------------------------------------------------------------------------------
** Arguments:
**    eDatasetType       - Dataset type.
**    iDatasetIdentifier - Dataset identifier
**    pfRetainBit        - Pointer to retain bit buffer. Assign to configured
**                         Retain bit state of the specified dataset.
**    ppcTopicString     - Pointer to pointer to reference to a topic string.
**                         Assign to topic string configured for the specified
**                         dataset.
**
** Returns:
**    ABP_ERR_NO_ERROR if the dataset was identified and a publish configuration
**    exists (pfRetainBit and ppcTopicString assigned).
**    For unknown or not supported dataset types, respond with
**    ABP_ERR_INV_CMD_EXT_0. If the dataset identifier is unkown or not
**    supported respond with ABP_ERR_OUT_OF_RANGE.
**------------------------------------------------------------------------------
*/
EXTFUNC ABP_MsgErrorCodeType MQTT_GetPublishConfigurationCfn(
   ABP_AppDatasetType eDatasetType,
   UINT16 iDatasetIdentifier,
   BOOL* pfRetainBit,
   const char** ppcTopicString );
#endif /* #if MQTT_CMD_PUBLISH_CFG_ENABLE */

/*------------------------------------------------------------------------------
** The function that processes the commands sent to the MQTT object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void MQTT_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#endif  /* inclusion lock */
