/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 0401fde (2024-11-13)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2024-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** ABCC driver error codes
********************************************************************************
*/
#ifndef ABCC_ERROR_CODES_H_
#define ABCC_ERROR_CODES_H_

typedef enum ABCC_ErrorCodeType
{
   ABCC_EC_NO_ERROR,
   ABCC_EC_INTERNAL_ERROR,
   ABCC_EC_LINK_CMD_QUEUE_FULL,
   ABCC_EC_LINK_RESP_QUEUE_FULL,
   ABCC_EC_OUT_OF_MSG_BUFFERS,
   ABCC_EC_TRYING_TO_FREE_NULL_POINTER,
   ABCC_EC_INCORRECT_OPERATING_MODE,
   ABCC_EC_INCORRECT_STATE,
   ABCC_EC_RESP_MSG_E_BIT_SET,
   ABCC_EC_WRPD_SIZE_ERR,
   ABCC_EC_RDPD_SIZE_ERR,
   ABCC_EC_RDMSG_SIZE_ERR,
   ABCC_EC_INVALID_RESP_SOURCE_ID,
   ABCC_EC_MODULE_NOT_DECTECTED,
   ABCC_EC_PARAMETER_NOT_VALID,
   ABCC_EC_MODULE_ID_NOT_SUPPORTED,
   ABCC_EC_DEFAULT_MAP_ERR,
   ABCC_EC_ERROR_IN_READ_MAP_CONFIG,
   ABCC_EC_ERROR_IN_WRITE_MAP_CONFIG,
   ABCC_EC_INTSTATUS_NOT_SUPPORTED_BY_DRV_IMPL,
   ABCC_EC_MODCAP_NOT_SUPPORTED_BY_DRV_IMPL,
   ABCC_EC_LEDSTATUS_NOT_SUPPORTED_BY_DRV_IMPL,
   ABCC_EC_SPI_OP_NOT_ALLOWED_DURING_SPI_TRANSACTION,
   ABCC_EC_WRMSG_SIZE_ERR,
   ABCC_EC_MSG_BUFFER_CORRUPTED,
   ABCC_EC_MSG_BUFFER_ALREADY_FREED,
   ABCC_EC_NO_RESOURCES,
   ABCC_EC_HW_INIT_FAILED,
   ABCC_EC_RCV_CMD_SIZE_EXCEEDS_BUFFER,
   ABCC_EC_RCV_RESP_SIZE_EXCEEDS_BUFFER,
   ABCC_EC_UNEXPECTED_NULL_PTR,
   ABCC_EC_OUT_OF_CMD_SEQ_RESOURCES,
   ABCC_EC_SYS_ADAPTION_ERR,
   ABCC_EC_APPLICATION_SPECIFIC,
   ABCC_EC_ERROR_IN_PD_MAP_CONFIG,
   ABCC_EC_ERROR_RESP_SEGMENTATION,
   ABCC_EC_BAD_ASSEMBLY_INSTANCE,
   ABCC_EC_UNSUPPORTED_DATA_TYPE,
   ABCC_EC_CHECKSUM_MISMATCH,
   ABCC_EC_SETUP_FAILED,
   ABCC_EC_MSG_BUFFER_OVERRUN,
   ABCC_EC_MODULE_NOT_ANSWERING,
   ABCC_EC_CMD_SEQ_RETRY_LIMIT,
   ABCC_EC_UNKNOWN_ENDIAN,
   ABCC_EC_ASSERT_FAILED,
   ABCC_EC_PD_SIZE_MISMATCH,
   ABCC_EC_SET_ENUM_ANSI_SIZE       = 0x7FFF
}
ABCC_ErrorCodeType;

#endif
