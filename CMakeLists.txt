cmake_minimum_required(VERSION 3.22)
project(net_demo C)

set(CMAKE_C_STANDARD 99)


if (BUILD_PLATFORM STREQUAL client)
    message("this is client target build ")
    add_executable(net_demo_client  client.c net.h net.c)
else()
    message("this is server target build ")
    add_executable(net_demo_server server.c net.h net.c)
endif ()


#add_executable(net_demo main.c  net.h net.c)
#add_executable(net_demo_client  client.c net.h net.c)
