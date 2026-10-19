TOOLCHAIN ?= arm-gcc-m7f
ION_KEYBOARD_LAYOUT = layout_B3
PCB_LATEST = 343 # PCB version 3.43

# Defined only for the firmware that runs in a bootloader slot (not for n0100 / legacy n0110)
SFLAGS += -DDEVICE_BOOTLOADER
