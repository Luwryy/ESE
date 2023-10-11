################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Prod/Src/bmp280.c 

OBJS += \
./Prod/Src/bmp280.o 

C_DEPS += \
./Prod/Src/bmp280.d 


# Each subdirectory must supply rules for building sources it contributes
Prod/Src/%.o Prod/Src/%.su Prod/Src/%.cyclo: ../Prod/Src/%.c Prod/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Prod/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Prod-2f-Src

clean-Prod-2f-Src:
	-$(RM) ./Prod/Src/bmp280.cyclo ./Prod/Src/bmp280.d ./Prod/Src/bmp280.o ./Prod/Src/bmp280.su

.PHONY: clean-Prod-2f-Src

