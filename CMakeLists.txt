
cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "gosh-core")

project(${PROJECT_NAME})


##----------------------------------------------------------------------------##
## Compiler Definitions                                                       ##
##----------------------------------------------------------------------------##
set(CMAKE_CXX_STANDARD          14)
set(CMAKE_CXX_STANDARD_REQUIRED on)


##----------------------------------------------------------------------------##
## Include Directories                                                        ##
##----------------------------------------------------------------------------##
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/fmt/include

)

##----------------------------------------------------------------------------##
## Source Files                                                               ##
##----------------------------------------------------------------------------##
set(SOURCE_FILES
    ./src/gosh.cpp

    ./src/Algo.hpp
    ./src/Array.hpp
    ./src/Assert.hpp
    ./src/BasicTypes.hpp
    ./src/BuildInfo.hpp
    ./src/CodeUtils.hpp
    ./src/CommandLine.hpp
        ./src/CommandLine.cpp
    ./src/Debug.hpp
    ./src/FileUtils.hpp
    ./src/FileUtils.cpp
    ./src/gosh.cpp
    ./src/Macros.hpp
    ./src/Math.hpp
    ./src/OS.hpp
    ./src/PathUtils.hpp
        ./src/Win32_PathUtils.cpp
    ./src/Result.hpp
    ./src/String.hpp
        ./src/String.cpp
    ./src/Terminal.hpp
        ./src/Terminal.cpp

    ./libs/fmt/include/fmt/core.h
    ./libs/fmt/include/fmt/format.h
    ./libs/fmt/include/fmt/format-inl.h

    ./libs/fmt/src/format.cc
)


##----------------------------------------------------------------------------##
## Executable Definitions                                                     ##
##----------------------------------------------------------------------------##
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
