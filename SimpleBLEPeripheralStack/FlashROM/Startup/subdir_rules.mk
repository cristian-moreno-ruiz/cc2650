################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Startup/CommonROM_Init.obj: C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/ROM/CommonROM_Init.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/../../../../../config/buildComponents.opt" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/buildConfig.opt"  -mv7M3 --code_state=16 --abi=eabi -me -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/Source/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/CC2650TIRTOS" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/_common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/osal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/saddr" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/icall/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/controller/CC26xx/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/ROM" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/hci" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/host" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/aes/CC26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/npi" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/npi/npi_np/CC26xx/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/ICall/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Profiles/Roles" --include_path="C:/ti/tirtos_simplelink_2_13_00_06/products/cc26xxware_2_21_01_15600" --define=USE_ICALL --define=OSAL_SNV=2 --define=FLASH_ROM_BUILD --define=POWER_SAVING --define=GATT_NO_CLIENT --define=INCLUDE_AES_DECRYPT --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --define=OSAL_CBTIMER_NUM_TASKS=1 --define=xDEBUG --define=HALNODEBUG --define=xDEBUG_GPIO --define=xDEBUG_ENC --define=xDEBUG_SW_TRACE --define=NEAR_FUNC= --define=DATA= --define=CC26XXWARE --define=CC26XX --diag_wrap=off --diag_suppress=48 --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="Startup/CommonROM_Init.pp" --obj_directory="Startup" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Startup/ICall_startup.obj: C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/cc26xx/ICall_startup.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/../../../../../config/buildComponents.opt" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/buildConfig.opt"  -mv7M3 --code_state=16 --abi=eabi -me -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/Source/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/CC2650TIRTOS" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/_common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/osal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/saddr" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/icall/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/controller/CC26xx/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/ROM" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/hci" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/host" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/aes/CC26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/npi" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/npi/npi_np/CC26xx/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/ICall/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Profiles/Roles" --include_path="C:/ti/tirtos_simplelink_2_13_00_06/products/cc26xxware_2_21_01_15600" --define=USE_ICALL --define=OSAL_SNV=2 --define=FLASH_ROM_BUILD --define=POWER_SAVING --define=GATT_NO_CLIENT --define=INCLUDE_AES_DECRYPT --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --define=OSAL_CBTIMER_NUM_TASKS=1 --define=xDEBUG --define=HALNODEBUG --define=xDEBUG_GPIO --define=xDEBUG_ENC --define=xDEBUG_SW_TRACE --define=NEAR_FUNC= --define=DATA= --define=CC26XXWARE --define=CC26XX --diag_wrap=off --diag_suppress=48 --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="Startup/ICall_startup.pp" --obj_directory="Startup" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Startup/OSAL_ICallBle.obj: C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/Source/Stack/OSAL_ICallBle.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/../../../../../config/buildComponents.opt" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/buildConfig.opt"  -mv7M3 --code_state=16 --abi=eabi -me -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/Source/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/CC2650TIRTOS" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/_common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/osal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/saddr" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/icall/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/controller/CC26xx/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/ROM" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/hci" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/host" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/aes/CC26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/npi" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/npi/npi_np/CC26xx/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/ICall/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Profiles/Roles" --include_path="C:/ti/tirtos_simplelink_2_13_00_06/products/cc26xxware_2_21_01_15600" --define=USE_ICALL --define=OSAL_SNV=2 --define=FLASH_ROM_BUILD --define=POWER_SAVING --define=GATT_NO_CLIENT --define=INCLUDE_AES_DECRYPT --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --define=OSAL_CBTIMER_NUM_TASKS=1 --define=xDEBUG --define=HALNODEBUG --define=xDEBUG_GPIO --define=xDEBUG_ENC --define=xDEBUG_SW_TRACE --define=NEAR_FUNC= --define=DATA= --define=CC26XXWARE --define=CC26XX --diag_wrap=off --diag_suppress=48 --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="Startup/OSAL_ICallBle.pp" --obj_directory="Startup" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Startup/ROM_Init.obj: C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/ROM/ROM_Init.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/../../../../../config/buildComponents.opt" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/buildConfig.opt"  -mv7M3 --code_state=16 --abi=eabi -me -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/Source/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/CC2650TIRTOS" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/_common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/osal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/saddr" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/icall/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/controller/CC26xx/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/ROM" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/hci" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/host" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/aes/CC26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/npi" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/npi/npi_np/CC26xx/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/ICall/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Profiles/Roles" --include_path="C:/ti/tirtos_simplelink_2_13_00_06/products/cc26xxware_2_21_01_15600" --define=USE_ICALL --define=OSAL_SNV=2 --define=FLASH_ROM_BUILD --define=POWER_SAVING --define=GATT_NO_CLIENT --define=INCLUDE_AES_DECRYPT --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --define=OSAL_CBTIMER_NUM_TASKS=1 --define=xDEBUG --define=HALNODEBUG --define=xDEBUG_GPIO --define=xDEBUG_ENC --define=xDEBUG_SW_TRACE --define=NEAR_FUNC= --define=DATA= --define=CC26XXWARE --define=CC26XX --diag_wrap=off --diag_suppress=48 --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="Startup/ROM_Init.pp" --obj_directory="Startup" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Startup/bleUserConfig.obj: C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/ICall/Stack/bleUserConfig.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/../../../../../config/buildComponents.opt" --cmd_file="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/CCS/SimpleBLEPeripheralStack/../../IAR/Stack/CC2640/buildConfig.opt"  -mv7M3 --code_state=16 --abi=eabi -me -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/SimpleBLEPeripheral/CC26xx/Source/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/CC2650TIRTOS" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/target/_common/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/hal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/osal/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv/cc26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/nv" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/saddr" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/icall/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/controller/CC26xx/include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/ROM" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/hci" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/ble/host" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/services/aes/CC26xx" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Components/npi" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/common/npi/npi_np/CC26xx/Stack" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/ICall/Include" --include_path="C:/ti/simplelink/ble_cc26xx_2_01_01_44627/Projects/ble/Profiles/Roles" --include_path="C:/ti/tirtos_simplelink_2_13_00_06/products/cc26xxware_2_21_01_15600" --define=USE_ICALL --define=OSAL_SNV=2 --define=FLASH_ROM_BUILD --define=POWER_SAVING --define=GATT_NO_CLIENT --define=INCLUDE_AES_DECRYPT --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --define=OSAL_CBTIMER_NUM_TASKS=1 --define=xDEBUG --define=HALNODEBUG --define=xDEBUG_GPIO --define=xDEBUG_ENC --define=xDEBUG_SW_TRACE --define=NEAR_FUNC= --define=DATA= --define=CC26XXWARE --define=CC26XX --diag_wrap=off --diag_suppress=48 --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="Startup/bleUserConfig.pp" --obj_directory="Startup" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


