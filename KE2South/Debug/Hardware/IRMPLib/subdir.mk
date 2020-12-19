################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Hardware/IRMPLib/irmp.c \
../Hardware/IRMPLib/irmpextlog.c 

OBJS += \
./Hardware/IRMPLib/irmp.o \
./Hardware/IRMPLib/irmpextlog.o 

C_DEPS += \
./Hardware/IRMPLib/irmp.d \
./Hardware/IRMPLib/irmpextlog.d 


# Each subdirectory must supply rules for building sources it contributes
Hardware/IRMPLib/irmp.o: ../Hardware/IRMPLib/irmp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L433xx -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I"F:/Files/KE1App/KE2South/Hardware/IRMPLib" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Hardware/IRMPLib/irmp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Hardware/IRMPLib/irmpextlog.o: ../Hardware/IRMPLib/irmpextlog.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L433xx -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I"F:/Files/KE1App/KE2South/Hardware/IRMPLib" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Hardware/IRMPLib/irmpextlog.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

