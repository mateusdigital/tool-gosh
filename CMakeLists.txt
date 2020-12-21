
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
)

##----------------------------------------------------------------------------##
## Source Files                                                               ##
##----------------------------------------------------------------------------##
set(SOURCE_FILES
    ./src/gosh.cpp

    ./src/BasicTypes.hpp
    ./src/Array.hpp
    ./src/FileUtils.hpp
    ./src/FileUtils.cpp
    ./src/OS.hpp
    ./src/PathUtils.hpp
    ./src/Result.hpp
    ./src/String.hpp
    ./src/Terminal.hpp

)


##----------------------------------------------------------------------------##
## Executable Definitions                                                     ##
##----------------------------------------------------------------------------##
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
