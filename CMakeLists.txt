cmake_minimum_required(VERSION 3.17)
set(CMAKE_TOOLCHAIN_FILE $ENV{DEVKITPPC}/wii.toolchain)

set(PROJECT_NAME gc-ascii-keyboard)

project(${PROJECT_NAME} C ASM)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_C_STANDARD 99)

add_executable(${PROJECT_NAME}.elf src/main.c)

target_link_directories(${PROJECT_NAME}.elf PRIVATE ${DEVKITPRO}/libogc/lib/wii)
target_link_libraries(${PROJECT_NAME}.elf PRIVATE db wiiuse bte ogc m)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${DEVKITPRO}/libogc/include)

#add_subdirectory(${DEVKITPRO}/src/libogc-2.1.0 libogc)
#target_link_libraries(
#        ${PROJECT_NAME}.elf
#        ${DEVKITPRO}/src/libogc-2.1.0/lib/wii/libdb.a
#        ${DEVKITPRO}/src/libogc-2.1.0/lib/wii/libwiiuse.a
#        ${DEVKITPRO}/src/libogc-2.1.0/lib/wii/libbte.a
#        ${DEVKITPRO}/src/libogc-2.1.0/lib/wii/libogc.a
#        m
#)
#target_include_directories(${PROJECT_NAME}.elf PRIVATE ${DEVKITPRO}/src/libogc-2.1.0/gc)

target_make_dol(${PROJECT_NAME}.elf)
add_wiiload_target(wiiload ${PROJECT_NAME}.elf)
add_dolphin_target(dolphin ${PROJECT_NAME}.elf)
