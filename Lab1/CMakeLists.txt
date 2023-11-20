cmake_minimum_required(VERSION 3.26)
project(Keil_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(main2 02_GPIO_Runing_LED-demo/USER/main.c)

add_executable(test2 02_GPIO_Runing_LED-demo/Soft_Drive/platformtest/test.c)

include_directories(../Keil_include)
