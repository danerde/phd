################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Object.cpp \
../src/Pack.cpp \
../src/Pose.cpp \
../src/Robot.cpp \
../src/ThreadPool.cpp \
../src/Time.cpp \
../src/V2d.cpp \
../src/World.cpp \
../src/rmob.cpp 

OBJS += \
./src/Object.o \
./src/Pack.o \
./src/Pose.o \
./src/Robot.o \
./src/ThreadPool.o \
./src/Time.o \
./src/V2d.o \
./src/World.o \
./src/rmob.o 

CPP_DEPS += \
./src/Object.d \
./src/Pack.d \
./src/Pose.d \
./src/Robot.d \
./src/ThreadPool.d \
./src/Time.d \
./src/V2d.d \
./src/World.d \
./src/rmob.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLOW_DRAW -DTHREAD_COUNT=1 -I/opt/ros/hydro/include/opencv -I"/home/dan/workspace/rmob/includes" -I/opt/ros/hydro/include -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


