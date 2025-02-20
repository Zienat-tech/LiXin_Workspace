################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/SEGGER_RTT/SEGGER_RTT.c \
../Middlewares/SEGGER_RTT/SEGGER_RTT_printf.c 

S_UPPER_SRCS += \
../Middlewares/SEGGER_RTT/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./Middlewares/SEGGER_RTT/SEGGER_RTT.o \
./Middlewares/SEGGER_RTT/SEGGER_RTT_ASM_ARMv7M.o \
./Middlewares/SEGGER_RTT/SEGGER_RTT_printf.o 

S_UPPER_DEPS += \
./Middlewares/SEGGER_RTT/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./Middlewares/SEGGER_RTT/SEGGER_RTT.d \
./Middlewares/SEGGER_RTT/SEGGER_RTT_printf.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/SEGGER_RTT/%.o Middlewares/SEGGER_RTT/%.su Middlewares/SEGGER_RTT/%.cyclo: ../Middlewares/SEGGER_RTT/%.c Middlewares/SEGGER_RTT/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP -I../Middlewares/SEGGER_RTT -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/SEGGER_RTT/%.o: ../Middlewares/SEGGER_RTT/%.S Middlewares/SEGGER_RTT/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Middlewares-2f-SEGGER_RTT

clean-Middlewares-2f-SEGGER_RTT:
	-$(RM) ./Middlewares/SEGGER_RTT/SEGGER_RTT.cyclo ./Middlewares/SEGGER_RTT/SEGGER_RTT.d ./Middlewares/SEGGER_RTT/SEGGER_RTT.o ./Middlewares/SEGGER_RTT/SEGGER_RTT.su ./Middlewares/SEGGER_RTT/SEGGER_RTT_ASM_ARMv7M.d ./Middlewares/SEGGER_RTT/SEGGER_RTT_ASM_ARMv7M.o ./Middlewares/SEGGER_RTT/SEGGER_RTT_printf.cyclo ./Middlewares/SEGGER_RTT/SEGGER_RTT_printf.d ./Middlewares/SEGGER_RTT/SEGGER_RTT_printf.o ./Middlewares/SEGGER_RTT/SEGGER_RTT_printf.su

.PHONY: clean-Middlewares-2f-SEGGER_RTT

