
# Fix For MACOS compability
if (APPLE)
    set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
endif()

PROJECT(STM32F1_bluepill)

set(CMAKE_BUILD_TYPE Debug)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
ENABLE_LANGUAGE(ASM)

# Remove -rdynamic option from linking that is not supported by arm-none-eabi
# Also remove "undefined reference to `_sbrk'" error
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-specs=nano.specs -specs=nosys.specs")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(STM32_CHIP STM32F103xB)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/gcc_stm32.cmake)
include(${CMAKE_TOOLCHAIN_FILE})

set(CMAKE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/lib)

FIND_PACKAGE(Git REQUIRED)
set(CUBE_SOURCE "${CMAKE_LIBRARY_PATH}/STM32F1-Cube-Firmware")
if(EXISTS ${CUBE_SOURCE})
    message("Cube FW directory exists")
else()
    message("Getting Cube Firmware")
    set(GIT_REPOSITORY "https://github.com/alxhoff/STM32F1-Cube-Firmware.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone ${GIT_REPOSITORY} ${CUBE_SOURCE})
endif()

set(STM32Cube_DIR ${CUBE_SOURCE})


SET(FREERTOS_HEAP_IMPL 4)
# SET(USBDevice_HID_CONFIG_DIR "${PROJECT_SOURCE_DIR}/HID_config")

FIND_PACKAGE(FreeRTOS REQUIRED)
FIND_PACKAGE(CMSIS REQUIRED)
FIND_PACKAGE(STM32HAL COMPONENTS rcc  gpio pcd REQUIRED)
FIND_PACKAGE(STM32LL COMPONENTS usb REQUIRED)
FIND_PACKAGE(USBDevice COMPONENTS HID REQUIRED)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/Inc"
    ${FreeRTOS_INCLUDE_DIRS}
    ${STM32HAL_INCLUDE_DIR}
    ${STM32LL_INCLUDE_DIR}
    ${CMSIS_INCLUDE_DIRS}
    ${USBDevice_INCLUDE_DIR}
    )

file(GLOB USER_SOURCES "Src/*.c")

SET(STM32_LINKER_SCRIPT ${CMSIS_LINKER_SCRIPT})

SET(PROJECT_ELF_FILE ${CMAKE_PROJECT_NAME}.elf)
SET(PROJECT_BIN_FILE ${CMAKE_PROJECT_NAME}.bin)

SET(PROJECT_SOURCES
    ${USER_SOURCES}
    ${STM32HAL_SOURCES}
    ${FreeRTOS_SOURCES}
    ${STM32LL_SOURCES}
    ${CMSIS_SOURCES}
    ${USBDevice_SOURCES}
    )

MESSAGE(STATUS "USB INCS ${USBDevice_INCLUDE_DIR}")
MESSAGE(STATUS "USB SRCS ${USBDevice_SOURCES}")

ADD_EXECUTABLE(
    ${PROJECT_ELF_FILE}
    ${PROJECT_SOURCES}
    )

target_link_libraries(${PROJECT_ELF_FILE} m)

ADD_CUSTOM_TARGET(
    flash
    COMMAND openocd  -c 'source [find interface/stlink-v2.cfg]' -c 'transport select hla_swd' -c 'source [find target/stm32f1x.cfg]' -c 'program ${PROJECT_ELF_FILE} verify reset exit'
    )

ADD_DEPENDENCIES(flash ${PROJECT_ELF_FILE})

ADD_CUSTOM_TARGET(clean_cmake
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND git clean -d -f -x
    )

ADD_CUSTOM_TARGET(clean_all
    COMMAND git clean -d ${PROJECT_SOURCE_DIR} -f -f -x
    )

ADD_CUSTOM_TARGET(
    debug
    COMMAND openocd  -c 'source [find interface/stlink-v2.cfg]' -c 'transport select hla_swd' -c 'source [find target/stm32f1x_stlink.cfg]' -c 'reset_config srst_nogate' >/dev/null 2&1 & sleep 2
    COMMAND ${TARGET_TRIPLET}-gdb -tui -command=${CMAKE_CURRENT_LIST_DIR}/GDBCommands -se ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
    COMMAND killall -15 openocd
    )

ADD_DEPENDENCIES(debug ${PROJECT_ELF_FILE})

STM32_SET_TARGET_PROPERTIES(${PROJECT_ELF_FILE})
STM32_ADD_HEX_BIN_TARGETS(${PROJECT_ELF_FILE})
STM32_PRINT_SIZE_OF_TARGETS(${PROJECT_ELF_FILE})

ADD_CUSTOM_TARGET(
    clean_lib
    DEPENDS cleanlib
    )

add_custom_command(OUTPUT cleanlib
    COMMAND ${CMAKE_COMMAND} -P "cmake/cleanall.cmake"
    )
