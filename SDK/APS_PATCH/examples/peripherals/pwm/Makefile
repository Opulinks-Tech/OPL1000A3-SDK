##### ARM GCC Toolchain Development #####
FOLDER  = ./Output-GCC
LD_SCRIPT = ../../../../APS/project/opl1000/link.ld
PATCH_LIB = ../../../../APS/targets/opl1000/Output-GCC/opl1000_app_m3.a
# Output file names
OUTPUT = opl1000_app_m3

# Compiler & Linker
CROSS_COMPILE = arm-none-eabi-
CC      = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy

#############  CFLAGS   ##############

# Options for specific architecture
DEFS  = -DGCC
DEFS += -DARMCM3
DEFS += -DOPL1000_APS
DEFS += -D__noSIMULATOR__
DEFS += -D__BLE__
DEFS += -D__LE_HOST_USE_CMD__
DEFS += -D__LWIP_TASK__
DEFS += -D__WPA_SUPPLICANT__
DEFS += -D__noHW_CRYPTO_ENGINE__
DEFS += -D__WIFI_MAC_TASK__
DEFS += -D__NL1000_An__
DEFS += -D__PMP_ENABLE__
DEFS += -D__PMP_REGION__
DEFS += -D__HEAP_REGION__
DEFS += -DnoLWIP_DYNAMIC_DEBUG_ENABLE
DEFS += -D__AT_CMD_TASK__
DEFS += -D__noRTL_SIMULATION__
DEFS += -D_ARMABI_PURE= 

CFLAGS  = $(DEFS)
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m3
CFLAGS += -mlittle-endian
CFLAGS += -std=c99
CFLAGS += -Os
CFLAGS += -x none
CFLAGS += -include sys_common.h
CFLAGS += -nostartfiles
CFLAGS += -no-pie
CFLAGS += -fdata-sections
CFLAGS += -ffunction-sections
CFLAGS += -specs=nosys.specs
CFLAGS += -Xlinker "--just-symbols=../../../../APS/targets/opl1000/object/symdefs_gcc.o"
CFLAGS += -Wno-implicit-function-declaration
CFLAGS += -Wno-int-conversion

LFLAGS = -T $(LD_SCRIPT)
LFLAGS += -Wl,--gc-sections
LFLAGS += -Wl,--entry=__Patch_EntryPoint
LFLAGS += -Wl,-Map=$(FOLDER)/$(OUTPUT).map

#############   include    ##############
INCDIR  = -I ../../../../APS/driver/chip/opl1000/hal_dbg_uart
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_patch
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_pwm
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_spi
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_system
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_tick
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_tmr
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_uart
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_vic
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_wdt
INCDIR += -I ../../../../APS/driver/CMSIS/Device/opl1000/Include
INCDIR += -I ../../../../APS/driver/CMSIS/Include
INCDIR += -I ../../../../APS/FreeRtos/Source/include
INCDIR += -I ../../../../APS/FreeRtos/Source/portable/GCC/ARM_CM3
INCDIR += -I ../../../../APS/middleware/netlink/at
INCDIR += -I ../../../../APS/middleware/netlink/cli
INCDIR += -I ../../../../APS/middleware/netlink/data_flow
INCDIR += -I ../../../../APS/middleware/netlink/diag_task
INCDIR += -I ../../../../APS/middleware/netlink/msg
INCDIR += -I ../../../../APS/middleware/netlink/mw_fim
INCDIR += -I ../../../../APS/middleware/netlink/mw_ota
INCDIR += -I ../../../../APS/middleware/netlink/ps_task
INCDIR += -I ../../../../APS/project/common
INCDIR += -I ../../../../APS/project/opl1000/boot_sequence
INCDIR += -I ../../../../APS/project/opl1000/include
INCDIR += -I ../../../../APS/project/opl1000/startup
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_dbg_uart
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_i2c
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_patch
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_pwm
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_spi
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_system
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_tick
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_tmr
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_uart
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_vic
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_wdt
INCDIR += -I ../../../../APS_PATCH/driver/CMSIS/Device/opl1000/Include
INCDIR += -I ../../../../APS_PATCH/driver/CMSIS/Include
INCDIR += -I ../../../../APS_PATCH/FreeRtos/Source/include
INCDIR += -I ../../../../APS_PATCH/FreeRtos/Source/portable/Keil/ARM_CM3
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/at
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/cli
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/data_flow
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/diag_task
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/msg
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/mw_fim
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/mw_ota
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/ps_task
INCDIR += -I ../../../../APS_PATCH/project/common
INCDIR += -I ../../../../APS_PATCH/project/opl1000/boot_sequence
INCDIR += -I ../../../../APS_PATCH/project/opl1000/include
INCDIR += -I ../../../../APS_PATCH/project/opl1000/startup
INCDIR += -I ../../../../APS_PATCH/project/opl1000/lib

#############   source    ##############
M_SRC = ./main_patch.c
M_SRC += ./example.c

#############   build   ##############
all:
	@mkdir $(FOLDER)
	@$(CC) -o $(FOLDER)/$(OUTPUT).elf $(LFLAGS) $(CFLAGS) $(INCDIR) $(M_SRC) $(PATCH_LIB)
	$(OBJCOPY) -O binary $(FOLDER)/$(OUTPUT).elf $(FOLDER)/$(OUTPUT).bin

#############   clean   ##############  
clean:
	@rm -rf $(FOLDER)