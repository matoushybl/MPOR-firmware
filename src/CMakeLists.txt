cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE USER_SOURCES ./*.c ./*.cpp)

add_executable(${PROJECT_NAME}.elf ${USER_SOURCES} printf.h printf.c)

target_link_libraries(${PROJECT_NAME}.elf opencm3_stm32f0)

add_bin_from_elf(${PROJECT_NAME}.bin ${PROJECT_NAME}.elf )