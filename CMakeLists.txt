# The minimum required version of CMake
cmake_minimum_required(VERSION 3.10)
# The toolchain
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/config/toolchain.cmake)

# To avoid CMake compiler check 
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Name the project
project(Bootloader)


#define gcc compiler
add_compile_definitions(gcc) # some code code are projected with ifdef(gcc)

# List all source files
set(SOURCES
    src/main.cpp
    src/startup_gcc.c
    src/bootloader/bootloader.cpp
    src/drivers/driverlib/gpio.c
    src/drivers/driverlib/aes.c
    src/drivers/driverlib/cpu.c
    src/drivers/driverlib/crc.c
    src/drivers/driverlib/i2c.c
    src/drivers/driverlib/uart.c
    src/drivers/driverlib/flash.c
    src/drivers/driverlib/timer.c
    src/drivers/driverlib/eeprom.c
    src/drivers/driverlib/shamd5.c
    src/drivers/driverlib/sysctl.c
    src/drivers/driverlib/sysexc.c
    src/drivers/driverlib/systick.c
    src/drivers/driverlib/watchdog.c
    src/drivers/driverlib/hibernate.c
    src/drivers/driverlib/interrupt.c
    # ... add all other source files
)

# Include directories based on the proposed folder structure
include_directories(src/drivers/driverlib/inc include/bootloader src/drivers/driverlib src/drivers/ include/)

# The libs directory to link
link_directories(${CMAKE_SOURCE_DIR}/libs)

# Create the executable
add_executable(bootloader ${SOURCES})

# Link the project with the standard library libdriver.a and libc.a
target_link_libraries(bootloader driver c)

# Specify the output filename
set_target_properties(bootloader PROPERTIES OUTPUT_NAME bootloader.elf)


# Compiler and linker flags (Modify these based on your MCU and SDK specifics)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_SOURCE_DIR}/config/basicLinker.ld -mfloat-abi=hard -mfpu=fpv4-sp-d16" )

# Define the custom command to run objcopy after building the bootloader target
add_custom_command(
  TARGET bootloader
  POST_BUILD
  COMMAND arm-none-eabi-objcopy -O binary $<TARGET_FILE:bootloader> bootloader.bin
  COMMENT "Converting ELF to binary"
)
