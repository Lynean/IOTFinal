################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/FlashMem/std/stm32f4xx_flash.c 

OBJS += \
./Src/FlashMem/std/stm32f4xx_flash.o 

C_DEPS += \
./Src/FlashMem/std/stm32f4xx_flash.d 


# Each subdirectory must supply rules for building sources it contributes
Src/FlashMem/std/%.o Src/FlashMem/std/%.su Src/FlashMem/std/%.cyclo: ../Src/FlashMem/std/%.c Src/FlashMem/std/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/button" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/Src/FlashMem/std" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/Src/FlashMem" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/buzzer" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/led" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/sensor" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/serial" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/ucglib" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Utilities" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/lib_stm" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Drivers/CMSIS/Include" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Drivers/STM32F401RE_StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-FlashMem-2f-std

clean-Src-2f-FlashMem-2f-std:
	-$(RM) ./Src/FlashMem/std/stm32f4xx_flash.cyclo ./Src/FlashMem/std/stm32f4xx_flash.d ./Src/FlashMem/std/stm32f4xx_flash.o ./Src/FlashMem/std/stm32f4xx_flash.su

.PHONY: clean-Src-2f-FlashMem-2f-std

