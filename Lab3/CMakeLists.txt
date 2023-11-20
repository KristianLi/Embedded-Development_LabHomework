cmake_minimum_required(VERSION 3.26)
project(Keil_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(main2 03_EEPROM_OLED/USER/main.c)

add_executable(test2 03_EEPROM_OLED/Soft_Drive/test_platform/test.c)

include_directories(../Keil_include)

include_directories(03_EEPROM_OLED/Soft_Drive/EEPROM 03_EEPROM_OLED/Soft_Drive/test_platform)
