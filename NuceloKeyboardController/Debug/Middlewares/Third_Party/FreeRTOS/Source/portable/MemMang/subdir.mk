################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Inc" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Drivers/CMSIS/Include" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"/home/alxhoff/git/GitHub/stm32_mech_keyboard/NuceloKeyboardController/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


