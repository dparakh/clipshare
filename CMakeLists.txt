#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(clipshare)

#List the source files here
if(APPLE)
set(PROJECT_SOURCES clipshare.mm)
set(CMAKE_CXX_FLAGS "-Wall -g -pg")
else(APPLE)
set(PROJECT_SOURCES clipshare.cpp)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
#We do this to use statically linked MSVCRT libs.
set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()
endif (APPLE)

#Set the project name here
set(EXE_NAME ClipShare)

#For MacOS, add the required frameworks here
if(APPLE)
   find_library(COCOA_LIBRARY Cocoa)
   set(EXTRA_LIBS ${COCOA_LIBRARY})
endif (APPLE)

add_executable(${EXE_NAME} WIN32 ${PROJECT_SOURCES})
target_link_libraries(${EXE_NAME} ${EXTRA_LIBS} )
