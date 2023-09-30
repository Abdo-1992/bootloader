# Specify the cross compiler tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-as)
set(CMAKE_AR arm-none-eabi-ar CACHE FILEPATH "Archiver")

# Adding compiler flags
#-mcpu=cortex-m4
#-mthumb
#-mfpu=fpv4-sp-d16
#-mfloat-abi=hard
#-fno-unwind-tables
#-fno-asynchronous-unwind-tables
#-fomit-frame-pointer
#-nostdlib
#-fno-exceptions
#-fno-rtti
#-O0
#-g (for dubug)
#-fno-use-cxa-atexit to remove "Dynamic Shared Object Handling	__dso_handle"
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fno-unwind-tables -fno-asynchronous-unwind-tables -fomit-frame-pointer -nostdlib -g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -fomit-frame-pointer -nostdlib -fno-rtti -fno-use-cxa-atexit -g -O0")

#define gcc compiler
add_compile_definitions(gcc PART_TM4C123GH6PM) # some code code are projected with ifdef(gcc)

