################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/arkanoPiLib.c \
../src/arkanoPi_6.c \
../src/fsm.c \
../src/kbhit.c \
../src/tmr.c 

OBJS += \
./src/arkanoPiLib.o \
./src/arkanoPi_6.o \
./src/fsm.o \
./src/kbhit.o \
./src/tmr.o 

C_DEPS += \
./src/arkanoPiLib.d \
./src/arkanoPi_6.d \
./src/fsm.d \
./src/kbhit.d \
./src/tmr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"C:\SysGCC\Raspberry\include" -I"D:\wiringPi-5edd177\devLib" -I"C:\SysGCC\Raspberry\include\wiringPi" -O0 -g3 -Wall -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


