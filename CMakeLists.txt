cmake_minimum_required(VERSION 3.8)
project(cpp_multimouse)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IOKit DEFAULT_MSG IOKIT_LIBRARY)


# CXX WARNING FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}\
        -v")
set(CMAKE_CXX_STANDARD 14)


#source files of solution
set(SOURCE_FILES USBPrivateDataSample.cpp USBPrivateDataSample.h)
add_executable(cpp_multimouse ${SOURCE_FILES})


# Use MAC OS X Compiler Clang and link mac OS libraries for USB devices
if(APPLE)
    set(CMAKE_CXX_COMPILER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++)
    find_library(IOKIT IOKit)
    find_library(CF CoreFoundation)
    include_directories(${IOKIT} ${CF})
    target_link_libraries(cpp_multimouse ${IOKIT} ${CF})
endif(APPLE)

