################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/bsp_elog.c \
../Drivers/BSP/bsp_key.c \
../Drivers/BSP/bsp_led.c 

OBJS += \
./Drivers/BSP/bsp_elog.o \
./Drivers/BSP/bsp_key.o \
./Drivers/BSP/bsp_led.o 

C_DEPS += \
./Drivers/BSP/bsp_elog.d \
./Drivers/BSP/bsp_key.d \
./Drivers/BSP/bsp_led.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/%.o Drivers/BSP/%.su Drivers/BSP/%.cyclo: ../Drivers/BSP/%.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP -I../Middlewares/SEGGER_RTT -I../Middlewares/easylogger -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP

clean-Drivers-2f-BSP:
	-$(RM) ./Drivers/BSP/bsp_elog.cyclo ./Drivers/BSP/bsp_elog.d ./Drivers/BSP/bsp_elog.o ./Drivers/BSP/bsp_elog.su ./Drivers/BSP/bsp_key.cyclo ./Drivers/BSP/bsp_key.d ./Drivers/BSP/bsp_key.o ./Drivers/BSP/bsp_key.su ./Drivers/BSP/bsp_led.cyclo ./Drivers/BSP/bsp_led.d ./Drivers/BSP/bsp_led.o ./Drivers/BSP/bsp_led.su

.PHONY: clean-Drivers-2f-BSP

