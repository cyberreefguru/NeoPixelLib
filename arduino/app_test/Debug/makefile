################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
AppTest.lss \

FLASH_IMAGE += \
AppTest.hex \

SIZEDUMMY += \
sizedummy \

AVRDUDEDUMMY += \
avrdudedummy \


# All Target
all: AppTest.elf secondary-outputs

# Tool invocations
AppTest.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc --cref -s -Os -o"AppTest.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm -Wl,-Map,AppTest.map,--cref -L"/Users/tom/Development/arduino/avrlib/Release" -L"/Users/tom/Development/arduino/IoShieldLib/Debug" -L"/Users/tom/Development/arduino/bcmprogmem/Debug" -L"/Users/tom/Development/arduino/animation_player/Debug" -mmcu=atmega328p
	@echo 'Finished building target: $@'
	@echo ' '

AppTest.lss: AppTest.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S AppTest.elf  >"AppTest.lss"
	@echo 'Finished building: $@'
	@echo ' '

AppTest.hex: AppTest.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex AppTest.elf  "AppTest.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: AppTest.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p AppTest.elf
	@echo 'Finished building: $@'
	@echo ' '

avrdudedummy: AppTest.elf
	@echo 'Invoking: AVRDude'
	/usr/local/CrossPack-AVR-20100115/bin/avrdude -pm328p -carduino -P/dev/tty.usbserial-A600aftj -b57600 -Uflash:w:AppTest.hex:a
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ASM_DEPS)$(ELFS)$(AVRDUDEDUMMY)$(S_UPPER_DEPS)$(C++_DEPS)$(LSS)$(S_DEPS)$(CC_DEPS)$(CPP_DEPS)$(CXX_DEPS)$(OBJS)$(FLASH_IMAGE)$(C_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) AppTest.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY) $(AVRDUDEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
