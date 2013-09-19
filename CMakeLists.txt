#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(clipshare)

#List the source files here
set(PROJECT_SOURCES clipshare.mm)
#Set the project name here
set(EXE_NAME ClipShare)

set(CMAKE_CXX_FLAGS "-Wall -g -pg")

#For MacOS, add the required frameworks here
if(APPLE)
   find_library(COCOA_LIBRARY Cocoa)
   set(EXTRA_LIBS ${COCOA_LIBRARY})
endif (APPLE)

add_executable(${EXE_NAME} ${PROJECT_SOURCES})
target_link_libraries(${EXE_NAME} ${EXTRA_LIBS} )

