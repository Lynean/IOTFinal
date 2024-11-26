################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/FlashMem/flashmem.c 

OBJS += \
./Src/FlashMem/flashmem.o 

C_DEPS += \
./Src/FlashMem/flashmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/FlashMem/%.o Src/FlashMem/%.su Src/FlashMem/%.cyclo: ../Src/FlashMem/%.c Src/FlashMem/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/button" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/Src/FlashMem/std" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/Src/FlashMem" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/buzzer" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/led" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/sensor" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/serial" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/ucglib" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Utilities" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/lib_stm" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Drivers/CMSIS/Include" -I"C:/Funix/myWorkspace/Target/FinalProject_STM_V1_0_0/SDK_1.0.3_NUCLEO-F401RE/shared/Drivers/STM32F401RE_StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-FlashMem

clean-Src-2f-FlashMem:
	-$(RM) ./Src/FlashMem/flashmem.cyclo ./Src/FlashMem/flashmem.d ./Src/FlashMem/flashmem.o ./Src/FlashMem/flashmem.su

.PHONY: clean-Src-2f-FlashMem

