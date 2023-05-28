
cmake_minimum_required(VERSION 3.22)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING TRUE)

project("nrf52833" C)

set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles -nostdlib -g -Wall -mcpu=cortex-m4 -mthumb -ffreestanding")

add_library(${PROJECT_NAME} STATIC)

set(HEADERS_PATH include/nrf52833)
set(HEADERS ${HEADERS_PATH}/clock.h ${HEADERS_PATH}/gpio.h ${HEADERS_PATH}/nvmc.h ${HEADERS_PATH}/uarte.h)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/clock.c src/gpio.c src/uarte.c src/utils.c
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib/${PROJECT_NAME})
install(FILES ${HEADERS} DESTINATION /usr/include/${PROJECT_NAME})
