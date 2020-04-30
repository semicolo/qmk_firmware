
# Repurposed fax panel that includes a 16x1 LCD display, 17 buttons, 3 LEDs and a speaker

Keyboard using a BluePill controller (generic Chinese STM32F103C8T6) and based on the [KC64 of Xydane](https://github.com/Xydane/qmk_firmware), used for a bunch of macros.

Keyboard Maintainer: [semicolo](https://github.com/semicolo)
Hardware Supported: Bluepill STM32F103C8T6 for the controller.
Hardware Availability: Close to none, was taken from an old fax machine.

Make example for BluePill70 (after setting up your build environment):

    make repurposed/faxpanel:default

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

Read everything before using it.

## Introduction

First of all thanks to [Xydane](https://github.com/Xydane) at [GeekHack](https://geekhack.org/) for his repository, without it you couldn't be reading this.

## Resistor fix

If you want to use the Bluepill USB port, it's very likely that you need to solder a 1.8 kΩ between PA12 and 3.3V. That depends on the PC's motherboard but I needed, more info [here](https://wiki.stm32duino.com/index.php?title=Blue_Pill), sometimes you get boards that have the right resistor and you don't need to fix it.

## No bootloader

Neither Xydane or I used the bootloader, the code for it is commented and you can use it if you want. I didn't try to run it and just used an ST-LINK programmer.

## Pinout

That fax panel needs 8 pins for input/output + 3 output pins.
The 8 I/Os connect to the output of a 74244 and the input of a 74374, one of the extra outputs drives the 74244 gate, another drives the 74374 clock, the last one drives the speaker.
I used PB8-PB15 for the 8 I/Os and PA0-PA2

Simplified reverse engineered schematic:
![Schematic](https://i.imgur.com/ZRyPguO.png)

