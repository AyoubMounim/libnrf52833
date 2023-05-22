
cmake_minimum_required(VERSION 3.0)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING TRUE)

project("nrf52833" C)

set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles -nostdlib -g -Wall -mcpu=cortex-m4 -mthumb -ffreestanding")

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/clock.c src/gpio.c src/uarte.c src/utils.c
)

set(HEADERS_DIR include/nrf52833)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${HEADERS_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

message(
    STATUS
    "Default: ${CMAKE_INSTALL_PREFIX}\n"
    "set to: ${CMAKE_SOURCE_DIR}/install"    
)
set(CMAKE_INSTALL_PREFIX
    "${CMAKE_SOURCE_DIR}/install"
    CACHE PATH "Where the lib is installed" FORCE
)

set(public_headers
    ${HEADERS_DIR}/clock.h ${HEADERS_DIR}/gpio.h ${HEADERS_DIR}/macros.h ${HEADERS_DIR}/nvmc.h ${HEADERS_DIR}/uarte.h
)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT "${PROJECT_NAME}Targets"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT "${PROJECT_NAME}Targets"
    FILE "${PROJECT_NAME}Targets.cmake"
    NAMESPACE ${namespace}::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "0.1"
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION cmake
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION cmake
)

export(EXPORT "${PROJECT_NAME}Targets"
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake"
    NAMESPACE ${namespace}::
)
