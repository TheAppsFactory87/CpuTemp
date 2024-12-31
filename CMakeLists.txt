cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CpuTemp)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(CpuTemp src/main.cpp src/Temperature.cpp)

target_include_directories(CpuTemp PRIVATE include)

# Add Windows-specific libraries if building on Windows
if(WIN32)
    target_link_libraries(CpuTemp wbemuuid)
endif()
