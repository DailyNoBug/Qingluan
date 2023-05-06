cmake_minimum_required(VERSION 3.10)

# 项目名称
project(led_blink C ASM)

# 添加启动文件
set(STARTUP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32f401xe.s)

# 添加链接器脚本文件
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Script/STM32F401RETx_FLASH.ld)

# 添加源文件
set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Src/main.c)

# 设置编译器和链接器
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_LINKER arm-none-eabi-ld)

# 设置编译选项
set(CMAKE_C_FLAGS "-mthumb -mcpu=cortex-m4 -g -O0 -Wall -fdata-sections -ffunction-sections")
set(CMAKE_CXX_FLAGS "-mthumb -mcpu=cortex-m4 -g -O0 -Wall -fdata-sections -ffunction-sections")
set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m4 -g -x assembler-with-cpp")

# 设置链接选项
set(CMAKE_EXE_LINKER_FLAGS "-mthumb -mcpu=cortex-m4 -specs=nano.specs -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map -T${LINKER_SCRIPT}")

# 添加可执行文件
add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES} ${STARTUP_FILE})

# 添加目标文件
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}.elf COMMAND arm-none-eabi-objcopy -Obinary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin)
# 烧录程序
add_custom_target(flash COMMAND st-flash write ${PROJECT_NAME}.bin 0x08000000 WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
