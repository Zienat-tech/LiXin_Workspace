################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/easylogger/elog.c \
../Middlewares/easylogger/elog_async.c \
../Middlewares/easylogger/elog_buf.c \
../Middlewares/easylogger/elog_port.c \
../Middlewares/easylogger/elog_utils.c 

OBJS += \
./Middlewares/easylogger/elog.o \
./Middlewares/easylogger/elog_async.o \
./Middlewares/easylogger/elog_buf.o \
./Middlewares/easylogger/elog_port.o \
./Middlewares/easylogger/elog_utils.o 

C_DEPS += \
./Middlewares/easylogger/elog.d \
./Middlewares/easylogger/elog_async.d \
./Middlewares/easylogger/elog_buf.d \
./Middlewares/easylogger/elog_port.d \
./Middlewares/easylogger/elog_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/easylogger/%.o Middlewares/easylogger/%.su Middlewares/easylogger/%.cyclo: ../Middlewares/easylogger/%.c Middlewares/easylogger/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP -I../Middlewares/SEGGER_RTT -I../Middlewares/easylogger -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-easylogger

clean-Middlewares-2f-easylogger:
	-$(RM) ./Middlewares/easylogger/elog.cyclo ./Middlewares/easylogger/elog.d ./Middlewares/easylogger/elog.o ./Middlewares/easylogger/elog.su ./Middlewares/easylogger/elog_async.cyclo ./Middlewares/easylogger/elog_async.d ./Middlewares/easylogger/elog_async.o ./Middlewares/easylogger/elog_async.su ./Middlewares/easylogger/elog_buf.cyclo ./Middlewares/easylogger/elog_buf.d ./Middlewares/easylogger/elog_buf.o ./Middlewares/easylogger/elog_buf.su ./Middlewares/easylogger/elog_port.cyclo ./Middlewares/easylogger/elog_port.d ./Middlewares/easylogger/elog_port.o ./Middlewares/easylogger/elog_port.su ./Middlewares/easylogger/elog_utils.cyclo ./Middlewares/easylogger/elog_utils.d ./Middlewares/easylogger/elog_utils.o ./Middlewares/easylogger/elog_utils.su

.PHONY: clean-Middlewares-2f-easylogger

