#BOOTMAGIC_ENABLE = yes	# Virtual DIP switch configuration
#EXTRAKEY_ENABLE = yes	# Audio control and System control
#CONSOLE_ENABLE = yes	# Console for debug
#COMMAND_ENABLE = yes    # Commands for debug and configuration
#SLEEP_LED_ENABLE = yes  # Breathing sleep LED during USB suspend
#NKRO_ENABLE = yes	    # USB Nkey Rollover
CUSTOM_MATRIX = yes 	# Custom matrix file
MOUSEKEY_ENABLE = no
#AUDIO_ENABLE = yes

DEFAULT_FOLDER = handwired/faxpanel17

# MCU name
MCU = STM32F103

# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

# project specific files
SRC = LiquidCrystal.c hardware.c matrix.c led.c customBehaviour.c

