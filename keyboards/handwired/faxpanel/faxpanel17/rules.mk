# MCU name
MCU = STM32F103

# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

# project specific files
SRC = LiquidCrystal.c hardware.c matrix.c led.c

