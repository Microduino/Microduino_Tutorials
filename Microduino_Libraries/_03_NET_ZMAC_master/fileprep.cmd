set MAC_INSTALL=C:\Atmel\MAC_v_2_6_1
set GAWK=gawk.exe

set APP=App_1_Nobeacon_Device
set APP_SRC_DIR=App_1_Nobeacon/Device
mkdir -p examples/%APP%

%GAWK% -f fileprep.awk %MAC_INSTALL%/Applications/MAC_Examples/%APP_SRC_DIR%/Src/main.c > examples/%APP%/example_main.c

%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mcps_purge_conf.c > examples/%APP%/usr_mcps_purge_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mcps_data_ind.c > examples/%APP%/usr_mcps_data_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_beacon_notify_ind.c > examples/%APP%/usr_mlme_beacon_notify_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_conf.c > examples/%APP%/usr_mlme_disassociate_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_ind.c > examples/%APP%/usr_mlme_disassociate_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_set_conf.c > examples/%APP%/usr_mlme_set_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_get_conf.c > examples/%APP%/usr_mlme_get_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_orphan_ind.c > examples/%APP%/usr_mlme_orphan_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_poll_conf.c > examples/%APP%/usr_mlme_poll_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_rx_enable_conf.c > examples/%APP%/usr_mlme_rx_enable_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_sync_loss_ind.c > examples/%APP%/usr_mlme_sync_loss_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_associate_ind.c > examples/%APP%/usr_mlme_associate_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_comm_status_ind.c > examples/%APP%/usr_mlme_comm_status_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_start_conf.c > examples/%APP%/usr_mlme_start_conf.c

set APP=App_1_Nobeacon_Coordinator
set APP_SRC_DIR=App_1_Nobeacon/Coordinator
mkdir -p examples/%APP%

%GAWK% -f fileprep.awk %MAC_INSTALL%/Applications/MAC_Examples/%APP_SRC_DIR%/Src/main.c > examples/%APP%/example_main.c

%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mcps_data_conf.c > examples/%APP%/usr_mcps_data_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mcps_purge_conf.c > examples/%APP%/usr_mcps_purge_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_associate_conf.c > examples/%APP%/usr_mlme_associate_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_beacon_notify_ind.c > examples/%APP%/usr_mlme_beacon_notify_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_conf.c > examples/%APP%/usr_mlme_disassociate_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_ind.c > examples/%APP%/usr_mlme_disassociate_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_get_conf.c > examples/%APP%/usr_mlme_get_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_orphan_ind.c > examples/%APP%/usr_mlme_orphan_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_poll_conf.c > examples/%APP%/usr_mlme_poll_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_rx_enable_conf.c > examples/%APP%/usr_mlme_rx_enable_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_sync_loss_ind.c > examples/%APP%/usr_mlme_sync_loss_ind.c

set APP=App_2_Nobeacon_Indirect_Traffic_Coordinator
set APP_SRC_DIR=App_2_Nobeacon_Indirect_Traffic/Coordinator
mkdir -p examples/%APP%

%GAWK% -f fileprep.awk %MAC_INSTALL%/Applications/MAC_Examples/%APP_SRC_DIR%/Src/main.c > examples/%APP%/example_main.c

%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mcps_purge_conf.c > examples/%APP%/usr_mcps_purge_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mcps_data_ind.c > examples/%APP%/usr_mcps_data_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_beacon_notify_ind.c > examples/%APP%/usr_mlme_beacon_notify_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_conf.c > examples/%APP%/usr_mlme_disassociate_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_ind.c > examples/%APP%/usr_mlme_disassociate_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_associate_conf.c > examples/%APP%/usr_mlme_associate_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_get_conf.c > examples/%APP%/usr_mlme_get_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_orphan_ind.c > examples/%APP%/usr_mlme_orphan_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_poll_conf.c > examples/%APP%/usr_mlme_poll_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_rx_enable_conf.c > examples/%APP%/usr_mlme_rx_enable_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_sync_loss_ind.c > examples/%APP%/usr_mlme_sync_loss_ind.c

set APP=App_2_Nobeacon_Indirect_Traffic_Device
set APP_SRC_DIR=App_2_Nobeacon_Indirect_Traffic/Device
mkdir -p examples/%APP%

%GAWK% -f fileprep.awk %MAC_INSTALL%/Applications/MAC_Examples/%APP_SRC_DIR%/Src/main.c > examples/%APP%/example_main.c

%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mcps_purge_conf.c > examples/%APP%/usr_mcps_purge_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mcps_data_conf.c > examples/%APP%/usr_mcps_data_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_beacon_notify_ind.c > examples/%APP%/usr_mlme_beacon_notify_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_conf.c > examples/%APP%/usr_mlme_disassociate_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_ind.c > examples/%APP%/usr_mlme_disassociate_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_associate_ind.c > examples/%APP%/usr_mlme_associate_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_comm_status_ind.c > examples/%APP%/usr_mlme_comm_status_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_start_conf.c > examples/%APP%/usr_mlme_start_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_get_conf.c > examples/%APP%/usr_mlme_get_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_set_conf.c > examples/%APP%/usr_mlme_set_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_orphan_ind.c > examples/%APP%/usr_mlme_orphan_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_rx_enable_conf.c > examples/%APP%/usr_mlme_rx_enable_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_sync_loss_ind.c > examples/%APP%/usr_mlme_sync_loss_ind.c

set APP=Star_Nobeacon
set APP_SRC_DIR=%APP%
mkdir -p examples/%APP%

%GAWK% -f fileprep.awk %MAC_INSTALL%/Applications/MAC_Examples/%APP_SRC_DIR%/Src/main.c > examples/%APP%/example_main.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/Applications/MAC_Examples/%APP_SRC_DIR%/Inc/app_config.h > ./app_config.h

%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mcps_purge_conf.c > examples/%APP%/usr_mcps_purge_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_beacon_notify_ind.c > examples/%APP%/usr_mlme_beacon_notify_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_conf.c > examples/%APP%/usr_mlme_disassociate_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_disassociate_ind.c > examples/%APP%/usr_mlme_disassociate_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_get_conf.c > examples/%APP%/usr_mlme_get_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_orphan_ind.c > examples/%APP%/usr_mlme_orphan_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_poll_conf.c > examples/%APP%/usr_mlme_poll_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_rx_enable_conf.c > examples/%APP%/usr_mlme_rx_enable_conf.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/usr_mlme_sync_loss_ind.c > examples/%APP%/usr_mlme_sync_loss_ind.c

%GAWK% -f fileprep.awk %MAC_INSTALL%/Applications/Helper_Files/SIO_Support/Src/sio_handler.c > ./sio_handler.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/Resources/Buffer_Management/Src/bmm.c > ./bmm.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/Resources/Queue_Management/Src/qmm.c > ./qmm.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal_pwr_mgmt.c > ./tal_pwr_mgmt.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal_init.c > ./tal_init.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal_pib.c > ./tal_pib.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal_rx.c > ./tal_rx.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal.c > ./tal.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal_rx_enable.c > ./tal_rx_enable.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal_tx.c > ./tal_tx.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal_ed.c > ./tal_ed.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal_slotted_csma.c > ./tal_slotted_csma.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Src/tal_irq_handler.c > ./tal_irq_handler.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/TFA/ATMEGARF_TAL_1/Src/tfa.c > ./tfa.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/Generic/Src/pal_flash.c > ./pal_flash.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/Generic/Src/pal_utils.c > ./pal_utils.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/Generic/Src/pal.c > ./pal.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/Generic/Src/pal_uart.c > ./pal_uart.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/ATMEGA128RFA1/Src/pal_irq.c > ./pal_irq.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/ATMEGA128RFA1/Src/pal_timer.c > ./pal_timer.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/ATMEGA128RFA1/Src/pal_usb_ftdi.c > ./pal_usb_ftdi.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/ATMEGA128RFA1/Src/pal_mcu_generic.c > ./pal_mcu_generic.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/ATMEGA128RFA1/Src/pal_sio_hub.c > ./pal_sio_hub.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_callback_wrapper.c > ./mac_callback_wrapper.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_tx_coord_realignment_command.c > ./mac_tx_coord_realignment_command.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac.c > ./mac.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_orphan.c > ./mac_orphan.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_poll.c > ./mac_poll.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_associate.c > ./mac_associate.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_dispatcher.c > ./mac_dispatcher.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_start.c > ./mac_start.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_beacon.c > ./mac_beacon.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_sync.c > ./mac_sync.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_data_ind.c > ./mac_data_ind.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_process_beacon_frame.c > ./mac_process_beacon_frame.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_rx_enable.c > ./mac_rx_enable.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_misc.c > ./mac_misc.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_api.c > ./mac_api.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_disassociate.c > ./mac_disassociate.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_mcps_data.c > ./mac_mcps_data.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_data_req.c > ./mac_data_req.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_process_tal_tx_frame_status.c > ./mac_process_tal_tx_frame_status.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_pib.c > ./mac_pib.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_security.c > ./mac_security.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Src/mac_scan.c > ./mac_scan.c
%GAWK% -f fileprep.awk %MAC_INSTALL%/Applications/Helper_Files/SIO_Support/Inc/sio_handler.h > ./sio_handler.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/Include/vendor_nwk_config.h > ./vendor_nwk_config.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/Include/return_val.h > ./return_val.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/Include/vendor_stack_config.h > ./vendor_stack_config.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/Include/mac_build_config.h > ./mac_build_config.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/Include/stack_config.h > ./stack_config.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/Include/ieee_const.h > ./ieee_const.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/Resources/Buffer_Management/Inc/bmm.h > ./bmm.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/Resources/Queue_Management/Inc/qmm.h > ./qmm.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Inc/mac.h > ./mac.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Inc/mac_internal.h > ./mac_internal.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Inc/mac_data_structures.h > ./mac_data_structures.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Inc/mac_api.h > ./mac_api.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Inc/mac_msg_types.h > ./mac_msg_types.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Inc/mac_msg_const.h > ./mac_msg_const.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/MAC/Inc/mac_config.h > ./mac_config.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/Inc/tal.h > ./tal.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/Inc/tal_types.h > ./tal_types.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Inc/tal_slotted_csma.h > ./tal_slotted_csma.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Inc/tal_pib.h > ./tal_pib.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Inc/atmega128rfa1.h > ./atmega128rfa1.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Inc/tal_irq_handler.h > ./tal_irq_handler.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Inc/tal_config.h > ./tal_config.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Inc/tal_tx.h > ./tal_tx.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Inc/tal_constants.h > ./tal_constants.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Inc/tal_internal.h > ./tal_internal.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TAL/ATMEGARF_TAL_1/Inc/tal_rx.h > ./tal_rx.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/TFA/Inc/tfa.h > ./tfa.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/Inc/armtypes.h > ./armtypes.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/Inc/avrtypes.h > ./avrtypes.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/Inc/avr32types.h > ./avr32types.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/Inc/mcu_types.h > ./mcu_types.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/Inc/pal.h > ./pal.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/Inc/pal_types.h > ./pal_types.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/Generic/Inc/pal_flash.h > ./pal_flash.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/Generic/Inc/pal_uart.h > ./pal_uart.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/Generic/Inc/pal_internal.h > ./pal_internal.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/Generic/Inc/pal_timer.h > ./pal_timer.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/ATMEGA128RFA1/Boards/pal_boardtypes.h > ./pal_boardtypes.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/ATMEGA128RFA1/Inc/pal_usb.h > ./pal_usb.h
%GAWK% -f fileprep.awk %MAC_INSTALL%/PAL/MEGA_RF/ATMEGA128RFA1/Inc/pal_mcu_generic.h > ./pal_mcu_generic.h

