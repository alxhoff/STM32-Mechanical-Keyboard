################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/AT24Cxx_stm32_hal.c \
../Src/CLI.c \
../Src/DS3231_stm32_hal.c \
../Src/SN54HC595.c \
../Src/datatypes.c \
../Src/fonts.c \
../Src/freertos.c \
../Src/keyboard.c \
../Src/keymap.c \
../Src/layers.c \
../Src/lookup.c \
../Src/macro.c \
../Src/main.c \
../Src/mouse.c \
../Src/scan.c \
../Src/screen.c \
../Src/shift.c \
../Src/ssd1306.c \
../Src/states.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/system_stm32f4xx.c \
../Src/usb_device.c \
../Src/usbd_conf.c \
../Src/usbd_desc.c \
../Src/visEffect.c \
../Src/ws2812b.c 

OBJS += \
./Src/AT24Cxx_stm32_hal.o \
./Src/CLI.o \
./Src/DS3231_stm32_hal.o \
./Src/SN54HC595.o \
./Src/datatypes.o \
./Src/fonts.o \
./Src/freertos.o \
./Src/keyboard.o \
./Src/keymap.o \
./Src/layers.o \
./Src/lookup.o \
./Src/macro.o \
./Src/main.o \
./Src/mouse.o \
./Src/scan.o \
./Src/screen.o \
./Src/shift.o \
./Src/ssd1306.o \
./Src/states.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/system_stm32f4xx.o \
./Src/usb_device.o \
./Src/usbd_conf.o \
./Src/usbd_desc.o \
./Src/visEffect.o \
./Src/ws2812b.o 

C_DEPS += \
./Src/AT24Cxx_stm32_hal.d \
./Src/CLI.d \
./Src/DS3231_stm32_hal.d \
./Src/SN54HC595.d \
./Src/datatypes.d \
./Src/fonts.d \
./Src/freertos.d \
./Src/keyboard.d \
./Src/keymap.d \
./Src/layers.d \
./Src/lookup.d \
./Src/macro.d \
./Src/main.d \
./Src/mouse.d \
./Src/scan.d \
./Src/screen.d \
./Src/shift.d \
./Src/ssd1306.d \
./Src/states.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/system_stm32f4xx.d \
./Src/usb_device.d \
./Src/usbd_conf.d \
./Src/usbd_desc.d \
./Src/visEffect.d \
./Src/ws2812b.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Inc" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Drivers/CMSIS/Include" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/V2.0/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


