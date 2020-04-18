cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE USER_SOURCES ./*.c ./*.cpp)


add_executable(${PROJECT_NAME}.elf ${USER_SOURCES} printf.h printf.c NRF24.cpp NRF24.h Logger.cpp Logger.h SysTick.cpp SysTick.h ServoTimer.cpp ServoTimer.h ../libs/RF24/RF24.cpp ../libs/RF24/RFSPI.cpp ../libs/RF24/RFSPI.h)

target_include_directories(${PROJECT_NAME}.elf PRIVATE libs/RF24)

target_link_libraries(${PROJECT_NAME}.elf opencm3_stm32f0)

add_bin_from_elf(${PROJECT_NAME}.bin ${PROJECT_NAME}.elf )