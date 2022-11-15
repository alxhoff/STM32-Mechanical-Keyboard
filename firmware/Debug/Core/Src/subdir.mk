################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LEDs.c \
../Core/Src/freertos.c \
../Core/Src/keyboard.c \
../Core/Src/keymap.c \
../Core/Src/lookup.c \
../Core/Src/main.c \
../Core/Src/rgb.c \
../Core/Src/send.c \
../Core/Src/sm.c \
../Core/Src/sn54hc595.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/ws2812b.c 

OBJS += \
./Core/Src/LEDs.o \
./Core/Src/freertos.o \
./Core/Src/keyboard.o \
./Core/Src/keymap.o \
./Core/Src/lookup.o \
./Core/Src/main.o \
./Core/Src/rgb.o \
./Core/Src/send.o \
./Core/Src/sm.o \
./Core/Src/sn54hc595.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/ws2812b.o 

C_DEPS += \
./Core/Src/LEDs.d \
./Core/Src/freertos.d \
./Core/Src/keyboard.d \
./Core/Src/keymap.d \
./Core/Src/lookup.d \
./Core/Src/main.d \
./Core/Src/rgb.d \
./Core/Src/send.d \
./Core/Src/sm.d \
./Core/Src/sn54hc595.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/ws2812b.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/LEDs.d ./Core/Src/LEDs.o ./Core/Src/LEDs.su ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/keyboard.d ./Core/Src/keyboard.o ./Core/Src/keyboard.su ./Core/Src/keymap.d ./Core/Src/keymap.o ./Core/Src/keymap.su ./Core/Src/lookup.d ./Core/Src/lookup.o ./Core/Src/lookup.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/rgb.d ./Core/Src/rgb.o ./Core/Src/rgb.su ./Core/Src/send.d ./Core/Src/send.o ./Core/Src/send.su ./Core/Src/sm.d ./Core/Src/sm.o ./Core/Src/sm.su ./Core/Src/sn54hc595.d ./Core/Src/sn54hc595.o ./Core/Src/sn54hc595.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/ws2812b.d ./Core/Src/ws2812b.o ./Core/Src/ws2812b.su

.PHONY: clean-Core-2f-Src

