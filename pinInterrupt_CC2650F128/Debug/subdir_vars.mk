################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../CC2650STK_BLE.cmd 

CFG_SRCS += \
../pinInterrupt.cfg 

C_SRCS += \
../Board.c \
../pinInterrupt.c 

OBJS += \
./Board.obj \
./pinInterrupt.obj 

C_DEPS += \
./Board.pp \
./pinInterrupt.pp 

GEN_MISC_DIRS += \
./configPkg/ 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_OPTS += \
./configPkg/compiler.opt 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

C_DEPS__QUOTED += \
"Board.pp" \
"pinInterrupt.pp" 

OBJS__QUOTED += \
"Board.obj" \
"pinInterrupt.obj" 

C_SRCS__QUOTED += \
"../Board.c" \
"../pinInterrupt.c" 


