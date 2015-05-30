################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/buscador.cpp \
../lib/indexadorHash.cpp \
../lib/indexadorInformacion.cpp \
../lib/stemmer.cpp \
../lib/tokenizador.cpp 

OBJS += \
./lib/buscador.o \
./lib/indexadorHash.o \
./lib/indexadorInformacion.o \
./lib/stemmer.o \
./lib/tokenizador.o 

CPP_DEPS += \
./lib/buscador.d \
./lib/indexadorHash.d \
./lib/indexadorInformacion.d \
./lib/stemmer.d \
./lib/tokenizador.d 


# Each subdirectory must supply rules for building sources it contributes
lib/%.o: ../lib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"../include" -I"../cute" -I"../boost" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


