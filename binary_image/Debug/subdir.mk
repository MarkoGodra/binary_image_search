################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../binary_image.c \
../main.c 

OBJS += \
./binary_image.o \
./main.o 

C_DEPS += \
./binary_image.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/rtrk/Desktop/workspace/queue" -include/home/rtrk/Desktop/workspace/queue/queue.h -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -D LIB_DEBUG -D BIN_DEBUG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


