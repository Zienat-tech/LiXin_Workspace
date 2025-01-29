################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Hardware_Driver/Key.c \
../Drivers/Hardware_Driver/Led.c 

OBJS += \
./Drivers/Hardware_Driver/Key.o \
./Drivers/Hardware_Driver/Led.o 

C_DEPS += \
./Drivers/Hardware_Driver/Key.d \
./Drivers/Hardware_Driver/Led.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Hardware_Driver/%.o Drivers/Hardware_Driver/%.su Drivers/Hardware_Driver/%.cyclo: ../Drivers/Hardware_Driver/%.c Drivers/Hardware_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/Hardware_Driver -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Hardware_Driver

clean-Drivers-2f-Hardware_Driver:
	-$(RM) ./Drivers/Hardware_Driver/Key.cyclo ./Drivers/Hardware_Driver/Key.d ./Drivers/Hardware_Driver/Key.o ./Drivers/Hardware_Driver/Key.su ./Drivers/Hardware_Driver/Led.cyclo ./Drivers/Hardware_Driver/Led.d ./Drivers/Hardware_Driver/Led.o ./Drivers/Hardware_Driver/Led.su

.PHONY: clean-Drivers-2f-Hardware_Driver

