# The CMake command include_guard is compatible with CMake version 3.10 and greater.
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.10.0")
# Preventing this file to be included more than once.
    include_guard(GLOBAL)
endif()

# Complete list of source (.c) files inside the Anybus CompactCom Driver. 
set(abcc_driver_SRCS
    ${ABCC_DRIVER_DIR}/src/application_select_firmware.c
    ${ABCC_DRIVER_DIR}/src/application_abcc_handler.c
    ${ABCC_DRIVER_DIR}/src/abcc_command_sequencer.c
    ${ABCC_DRIVER_DIR}/src/abcc_command_handler.c
    ${ABCC_DRIVER_DIR}/src/abcc_command_sequencer.c
    ${ABCC_DRIVER_DIR}/src/abcc_copy.c
    ${ABCC_DRIVER_DIR}/src/abcc_log.c
    ${ABCC_DRIVER_DIR}/src/abcc_handler.c
    ${ABCC_DRIVER_DIR}/src/abcc_link.c
    ${ABCC_DRIVER_DIR}/src/abcc_memory.c
    ${ABCC_DRIVER_DIR}/src/abcc_remap.c
    ${ABCC_DRIVER_DIR}/src/abcc_segmentation.c
    ${ABCC_DRIVER_DIR}/src/abcc_setup.c
    ${ABCC_DRIVER_DIR}/src/abcc_timer.c
    ${ABCC_DRIVER_DIR}/src/par/abcc_handler_parallel.c
    ${ABCC_DRIVER_DIR}/src/par/abcc_parallel_driver.c
    ${ABCC_DRIVER_DIR}/src/serial/abcc_crc16.c
    ${ABCC_DRIVER_DIR}/src/serial/abcc_handler_serial.c
    ${ABCC_DRIVER_DIR}/src/serial/abcc_serial_driver.c
    ${ABCC_DRIVER_DIR}/src/spi/abcc_crc32.c
    ${ABCC_DRIVER_DIR}/src/spi/abcc_handler_spi.c
    ${ABCC_DRIVER_DIR}/src/spi/abcc_spi_driver.c
    ${ABCC_DRIVER_DIR}/src/host_objects/application_data_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/anybus_file_system_interface_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/application_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/assembly_mapping_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/ethernet_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/mqtt_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/bacnet_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/cclink_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/cclink_ie_field_network_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/canopen_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/devicenet_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/profibus_dpv1_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/ethercat_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/ethernetip_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/ethernet_powerlink_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/modbus_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/network_objects/profinet_io_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/opcua_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/safety_object.c
    ${ABCC_DRIVER_DIR}/src/host_objects/sync_object.c
)

# Complete list of source (.c) files inside the Anybus CompactCom Driver. 
set(abcc_driver_INCS
    ${ABCC_DRIVER_DIR}/src/abcc_command_sequencer.h
    ${ABCC_DRIVER_DIR}/src/abcc_command_handler.h
    ${ABCC_DRIVER_DIR}/src/abcc_driver_interface.h
    ${ABCC_DRIVER_DIR}/src/abcc_handler.h
    ${ABCC_DRIVER_DIR}/src/abcc_link.h
    ${ABCC_DRIVER_DIR}/src/abcc_memory.h
    ${ABCC_DRIVER_DIR}/src/abcc_segmentation.h
    ${ABCC_DRIVER_DIR}/src/abcc_setup.h
    ${ABCC_DRIVER_DIR}/src/abcc_timer.h
    ${ABCC_DRIVER_DIR}/src/par/abcc_driver_parallel_interface.h
    ${ABCC_DRIVER_DIR}/src/spi/abcc_crc32.h
    ${ABCC_DRIVER_DIR}/src/spi/abcc_driver_spi_interface.h
    ${ABCC_DRIVER_DIR}/src/serial/abcc_crc16.h
    ${ABCC_DRIVER_DIR}/src/serial/abcc_driver_serial_interface.h
    ${ABCC_DRIVER_DIR}/inc/abcc.h
    ${ABCC_DRIVER_DIR}/inc/abcc_error_codes.h
    ${ABCC_DRIVER_DIR}/inc/abcc_host_attributes_lookup.h
    ${ABCC_DRIVER_DIR}/inc/abcc_log.h
    ${ABCC_DRIVER_DIR}/inc/abcc_application_data_interface.h
    ${ABCC_DRIVER_DIR}/inc/abcc_command_sequencer_interface.h
    ${ABCC_DRIVER_DIR}/inc/abcc_config.h
    ${ABCC_DRIVER_DIR}/inc/abcc_identification.h
    ${ABCC_DRIVER_DIR}/inc/abcc_object_config.h
    ${ABCC_DRIVER_DIR}/inc/abcc_port.h
    ${ABCC_DRIVER_DIR}/inc/abcc_hardware_abstraction.h
    ${ABCC_DRIVER_DIR}/inc/abcc_hardware_abstraction_parallel.h
    ${ABCC_DRIVER_DIR}/inc/abcc_hardware_abstraction_serial.h
    ${ABCC_DRIVER_DIR}/inc/abcc_hardware_abstraction_spi.h
    ${ABCC_DRIVER_DIR}/inc/application_abcc_handler.h
    ${ABCC_DRIVER_DIR}/inc/application_data_instance_config.h
    ${ABCC_DRIVER_DIR}/inc/application_select_firmware.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/anybus_file_system_interface_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/application_data_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/application_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/assembly_mapping_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/ethernet_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/mqtt_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/opcua_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/safety_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/sync_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/bacnet_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/canopen_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/cclink_ie_field_network_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/cclink_ie_tsn_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/cclink_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/devicenet_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/ethercat_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/ethernet_powerlink_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/ethernetip_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/modbus_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/profibus_dpv1_object.h
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects/profinet_io_object.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_add.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_asm.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_bac.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_ccl.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_cfn.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_ciet.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_cipid.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_cnt.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_cop.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_cpc.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_cpn.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_dev.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_dpv0di.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_dpv1.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_eco.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_ect.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_eip.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_eme.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_epl.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_er.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_etn.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_fsi.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_fusm.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_mdd.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_mod.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_mqtt.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_nwccl.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_nwcfn.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_nwdpv1.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_nwetn.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_nwpnio.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_opcua.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_pnam.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_pnio.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_safe.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_smtp.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_soc.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_src3.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_sync.h
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc/abp_time.h
)

# Creating a library target containing the Anybus CompactCom Driver.
add_library(abcc_driver EXCLUDE_FROM_ALL ${abcc_driver_SRCS} ${abcc_driver_INCS})

# Keeping the file and directory tree structure of the Anybus CompactCom Driver when 
# generating IDE projects.
source_group(TREE ${ABCC_DRIVER_DIR} PREFIX "abcc_driver" FILES ${abcc_driver_SRCS} ${abcc_driver_INCS})

# The user shall already have created the ABCC_DRIVER_INCLUDE_DIRS variable and added 
# directories to user specific include (.h) files outside the Anybus CompactCom Driver
# such as abcc_config.h.
### TODO: Fix because it's not working yet. Supposed to be some sort of sanity check 
### for the user's abcc_configuration.h file.
# if(NOT EXISTS ${ABCC_DRIVER_INCLUDE_DIRS}/abcc_configuration.h) # Probably wrong syntax...
#   message(WARNING "abcc_configuration.h is missing.\n
#   Use the Anybus CompactCom SDK Configuration GUI to generate a customized configuration file.")
# endif()

# Directories inside the Anybus CompactCom Driver containing include (.h) files to be 
# externally accessible is appended to the list ABCC_DRIVER_INCLUDE_DIRS.
list(APPEND ABCC_DRIVER_INCLUDE_DIRS 
    ${ABCC_DRIVER_DIR}/inc
    ${ABCC_DRIVER_DIR}/inc/abcc_abp/inc
    ${ABCC_DRIVER_DIR}/src
    ${ABCC_DRIVER_DIR}/inc/host_objects/network_objects
    ${ABCC_DRIVER_DIR}/inc/host_objects
)

# Adding all the Anybus CompactCom Driver related include (.h) files to the user host 
# Anybus CompactCom Driver library target.
target_include_directories(abcc_driver PRIVATE ${ABCC_DRIVER_INCLUDE_DIRS})