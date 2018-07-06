################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../corpo.cpp 

OBJS += \
./source/corpo.o 

CPP_DEPS += \
./source/corpo.d 


# Each subdirectory must supply rules for building sources it contributes
source/corpo.o: C:/Users/madso/Desktop/ProjetoFinal/corpo.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DAESL_TB -D__llvm__ -D__llvm__ -IC:/Xilinx2/Vivado/2018.1/include/etc -IC:/Xilinx2/Vivado/2018.1/include -IC:/Xilinx2/Vivado/2018.1/include/ap_sysc -IC:/Users/madso/Desktop -IC:/Xilinx2/Vivado/2018.1/win64/tools/auto_cc/include -IC:/Xilinx2/Vivado/2018.1/win64/tools/systemc/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


