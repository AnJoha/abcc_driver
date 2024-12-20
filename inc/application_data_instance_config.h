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
** Copyright 2019-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Header file for the Application data instances setup and the default map
********************************************************************************
*/

#ifndef APPL_ADI_CONFIG_H_
#define APPL_ADI_CONFIG_H_

#include "abcc_types.h"
#include "abp.h"
#include "abcc_config.h"
#include "abcc_application_data_interface.h"
#include "abcc_object_config.h"

#if ASM_OBJ_ENABLE
   #include "assembly_mapping_object.h"
#endif

/*------------------------------------------------------------------------------
** There are 6 examples of ADI mappings:
**
**         #define APPL_ADI_SETUP_BOARD_SPECIFIC
**         #define APPL_ADI_SETUP_SIMPLE_16
**         #define APPL_ADI_SETUP_SEPARATE_16
**         #define APPL_ADI_SETUP_ALL_TYPES
**         #define APPL_ADI_SETUP_SYNC
**         #define APPL_ADI_SETUP_ASM
**         #define APPL_ADI_SETUP_ECT_SEMI (Available as separate addon).
**
**         The active setup is chosen by:
**         #define APPL_ACTIVE_ADI_SETUP
**
**         Example: #define APPL_ACTIVE_ADI_SETUP APPL_ADI_SETUP_SIMPLE_16
**
**         Below is a description of each example:
**
**         For a more detailed description of ADI:s and ADI mapping,
**         see abcc_drv/inc/abcc_application_data_interface.h.
**         ---------------------------------------------------------------------
**         APPL_ADI_SETUP_BOARD_SPECIFIC :  Designed for the HW platform of this
**                                          SW project.
**         ---------------------------------------------------------------------
**         APPL_ADI_SETUP_SIMPLE_16:
**         Implemented in application_data_instance_mapping_simple16.c
**
**         AD instance 1: An array of 32 ABP_UINT16 (64 octets mapped to write process data)
**         AD instance 2: An array of 32 ABP_UINT16 (64 octets mapped to read process data)
**         Data is looped since the ADI:s are refering to the same variable.
**         ---------------------------------------------------------------------
**         APPL_ADI_SETUP_SEPARATE_16 ( Get/Set ADI callback example )
**         Implemented in application_data_instance_mapping_separate16.c
**         AD instance 10: An array of 32 ABP_UINT16 (64 octets mapped to read process data)
**         AD instance 11: An array of 32 ABP_UINT16 (64 octets mapped to write process data)
**         AD instance 12: ABP_UINT16 (not mapped to process data)
**
**         ADI 10 triggers a callback each time a new value is written from the network.
**         The callback copies the data to ADI 11 ( looped to write process data )
**
**         ADI 11 triggers a callback each time the value is read by the network.
**         The callback also increments ADI 12.
**         ---------------------------------------------------------------------
**         APPL_ADI_SETUP_ALL_TYPES  ( structured ADI:s and bit data types )
**
**         Implemented in application_data_instance_mapping_alltypes.c
**
**         AD instance 20:  ABP_UINT32  ( Mapped to read process data )
**         AD instance 21:  ABP_UINT32  ( Mapped to write process data, loops ADI 20 )
**         AD instance 22:  ABP_SINT32  ( Mapped to read process data )
**         AD instance 23:  ABP_SINT32  ( Mapped to write process data, loops ADI 22  )
**         AD instance 24:  ABP_UINT16  ( Mapped to read process data )
**         AD instance 25:  ABP_UINT16  ( Mapped to write process data , loops ADI 24 )
**         AD instance 26:  ABP_SINT16  ( Mapped to read process data )
**         AD instance 27:  ABP_SINT16  ( Mapped to write process data, loops ADI 26  )
**         AD instance 28:  ABP_BITS16  ( Mapped to read process data )
**         AD instance 29:  ABP_BITS16  ( Mapped to write process data, loops ADI 28  )
**         AD instance 30:  ABP_UINT8   ( Mapped to read process data )
**         AD instance 31:  ABP_UINT8   ( Mapped to write process data, loops ADI 30  )
**         AD instance 32:  ABP_SINT8   ( Mapped to read process data )
**         AD instance 33:  ABP_SINT8   ( Mapped to write process data, loops ADI 32  )
**         AD instance 34:  ABP_UINT8   ( Mapped to read process data )
**         AD instance 35:  ABP_UINT8   ( Mapped to write process data, loops ADI 34  )
**         AD instance 36:  ABP_BIT7    ( Mapped to read process data )
**         AD instance 37:  ABP_BIT7    ( Mapped to write process data, loops ADI 36  )
**         AD instance 38:  Structure   ( Mapped to read process data )
**         AD instance 39:  Structure   ( Mapped to write process data, loops ADI 38  )
**
**         THe structure has the following members:
**
**         Index 0: ABP_UINT32
**         Index 1: ABP_SINT32
**         Index 2: ABP_UINT16
**         Index 3: ABP_SINT16
**         Index 4: ABP_BITS16
**         Index 5: ABP_UINT8
**         Index 6: ABP_SINT8
**         Index 7: ABP_BITS8
**         Index 8: ABP_PAD8
**         Index 9: ABP_BIT1
**         Index 10: ABP_BIT2
**         Index 11: ABP_BIT3
**         Index 12: ABP_BIT4
**         Index 13: ABP_BIT5
**         Index 14: ABP_BIT6
**         Index 15: ABP_BIT7
**         Index 16: ABP_PAD4
**
**
**         Note!
**            - The content of the PAD bits are undefined.
**         ---------------------------------------------------------------------
**         APPL_ADI_SETUP_SYNC : Shows an example of how to handle the ADIs when
**                              running a sync application.
**            - The sync example requires the following setup in
**              abcc_driver_config.h
**                Using sync signal as source:
**                #define ABCC_CFG_SYNC_ENABLED                       1
**                #define ABCC_CFG_USE_ABCC_SYNC_SIGNAL_ENABLED       1
**                or
**                Using ABCC interrupt:
**                #define ABCC_CFG_SYNC_ENABLED                       1
**                #define ABCC_CFG_USE_ABCC_SYNC_SIGNAL_ENABLED       0
**                #define ABCC_CFG_INT_ENABLED                        1
**
**         ---------------------------------------------------------------------
**         APPL_ADI_SETUP_SPEED_EXAMPLE :
**         Shows a simple example using two 16 bit ADIs "Speed" and "refSpeed".
**         The example also shows how to manipulate the ADIs using the
**         APPL_CyclicalProcessing() function.
**
**         AD instance 1: "Speed"    ABP_UINT16 (mapped to read process data)
**         AD instance 2: "refSpeed" ABP_UINT16 (mapped to write process data)
**         ---------------------------------------------------------------------
**         APPL_ADI_SETUP_ASM :
**
**         Implemented in application_data_instance_mapping_asm.c
**
**         This is an example of how to implement assembly mapping instances.
**
**         The following ADI's are defined:
**         AD instance 1: ABP_UINT32
**         AD instance 2: ABP_SINT32
**         AD instance 3: ABP_UINT16
**         AD instance 4: ABP_SINT16
**
**         The following write assemblies are defined:
**         "Write mappable assembly 1": Mappable assembly containing ADI 1 and 3
**         "Write mappable assembly 2": Mappable assembly containing ADI 1
**         "Write mappable assembly 3": Mappable assembly containing ADI 3
**         "Write non-mappable assembly": Non-mappable assembly containing ADI 1
**
**         The following read assemblies are defined:
**         "Read mappable assembly 1": Mappable assembly containing ADI 2 and 4
**         "Read mappable assembly 2": Mappable assembly containing ADI 2
**         "Read non-mappable assembly": Non-mappable assembly containing ADI 2
**         ---------------------------------------------------------------------
**         APPL_ADI_SETUP_ECT_SEMI:
**         Example implementation of the EtherCAT Semiconductor "Common Device
**         Profile" specified by the EtherCAT Technology Group. This example is
**         available as a separate addon from HMS.
**         ---------------------------------------------------------------------
**         APPL_ADI_SETUP_BACNET :
**
**         Implemented in "application_data_instance_mapping_bacnet.c"
**
**         The default Process Data map in this example is appropriate for a
**         BACnet CompactCom since it does not do any RDPD mapping.
**
**------------------------------------------------------------------------------
*/
#define APPL_ADI_SETUP_BOARD_SPECIFIC  0
#define APPL_ADI_SETUP_SIMPLE_16       1
#define APPL_ADI_SETUP_SEPARATE_16     2
#define APPL_ADI_SETUP_ALL_TYPES       3
#define APPL_ADI_SETUP_SYNC            4
#define APPL_ADI_SETUP_SPEED_EXAMPLE   5
#define APPL_ADI_SETUP_ASM             6
#define APPL_ADI_SETUP_ECT_SEMI        7
#define APPL_ADI_SETUP_BACNET          8

/*------------------------------------------------------------------------------
** Active ADI setup to use
**------------------------------------------------------------------------------
*/
#ifndef APPL_ACTIVE_ADI_SETUP
#ifdef USE_BOARD_SPECIFIC_ADI_SETUP
#define APPL_ACTIVE_ADI_SETUP   APPL_ADI_SETUP_BOARD_SPECIFIC
#else
#define APPL_ACTIVE_ADI_SETUP   APPL_ADI_SETUP_SPEED_EXAMPLE
#endif
#endif

/*------------------------------------------------------------------------------
** If structured data types is used i.e. psStruct != NULL (see example below),
** this define could be used to indicate that the ABP_TYPE member in
** AD_AdiEntryType is don't care.
**
** { 0x60, "Struct", DONT_CARE,   17, DESC, { { NULL, NULL } }, psStruct }
**------------------------------------------------------------------------------
*/
#define DONT_CARE  0

/*------------------------------------------------------------------------------
** Configuration indicating if a 16 bit node address should be set towards
** instance 1 of the Network Configuration Object when the connected module
** supports a node address.
**------------------------------------------------------------------------------
*/
#ifndef APPL_USE_16BIT_NODE_ADDR
#define APPL_USE_16BIT_NODE_ADDR    0
#endif

/*------------------------------------------------------------------------------
** List of Application data instances
**
** NOTE: The entries in the ADI list cannot be placed in arbitrary order, they
** must be sorted in ascending order for all lookup functions in the driver and
** the Application Data Object to work as intended!
**
** PORTING ALERT!
**
** If the ADI structure is defined at system startup rather than at compile-time
** the 'const' below should be removed. This would be the case when the ADIs
** are defined by a local configuration file or by the modules plugged into the
** local backplane. Note that the ADI structures MUST have been initialised with
** valid data before the driver makes the call to AD_Init() in
** './example_application/application_abcc_handler.c'.
**
** With a fixed ADI structure that can go into ROM the 'const' should be left in
** place.
**------------------------------------------------------------------------------
*/
EXTVAR const AD_AdiEntryType APPL_asAdiEntryList[];

/*------------------------------------------------------------------------------
** Default process data map
**
** PORTING ALERT!
**
** If the default PD map is defined during system startup rather than at
** compile-time the 'const' below should be removed. Note that the default PD
** map structures MUST have been initialised with valid data before the driver
** makes the call to AD_Init() in
** './example_application/application_abcc_handler.c'.
**
** With a fixed PD map that can go into ROM the 'const' should be left in place.
**------------------------------------------------------------------------------
*/
EXTVAR const AD_MapType APPL_asAdObjDefaultMap[];

/*------------------------------------------------------------------------------
** A function called every cycle after read and write data have been updated.
** The purpose of it is to have a function that can operate on ADIs when they
** are updated. This function is made to be modifiable and is a good place for
** user code.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_CyclicalProcessing( void );

/*------------------------------------------------------------------------------
** Returns the number of defined Application data instances
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    UINT16 - Number of Application data instances
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 APPL_GetNumAdi( void );

/*------------------------------------------------------------------------------
** Initializes the application.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_Init( void );

/*------------------------------------------------------------------------------
** Interrupt service routine for sync event. There are 2 ways to detect a sync
** event.
** 1. Set the SYNCIEN in the ABCC interrupt mask register and call this
**    function from the ABCC_CbfEvent() callback function.
** 2. Use the the separate sync signal available on the ABCC host connector
**    interface connected to an external interrupt. This is recommended for high
**    performance applications.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if SYNC_OBJ_ENABLE
EXTFUNC void APPL_SyncIsr( void );
#endif

#if ASM_OBJ_ENABLE
/*------------------------------------------------------------------------------
** Get an array of all assembly instances. The instances returned will be
** assigned an instance number by the Assembly mapping object according to the
** index in the array (starting from 1).
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Pointer to array of assembly instances.
**------------------------------------------------------------------------------
*/
EXTFUNC const ASM_InstanceType** APPL_GetAsmInstances( void );

/*------------------------------------------------------------------------------
** Get number of assembly instances.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    Number of assembly instances.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 APPL_GetNumAsmInstances( void );
#endif

#endif  /* inclusion lock */
