cmake_minimum_required(VERSION 2.8)
project( PR_DETECTION )

#set (OpenCV_DIR "/home/yjy/LibAndSoftware/opencv_linux_20190511")

find_package( OpenCV REQUIRED )

set (source_dir "src/")
set (include_dir "headers/")
set (source_ext ".cpp")
set (target_ext ".app")

#Release Debug
set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to Debug")  

#/Od /Zi /arch /Og /Ot /Ox
list( APPEND CMAKE_CXX_FLAGS " -g ${CMAKE_CXX_FLAGS}")

include_directories(${include_dir})
#add_subdirectory(include)

set(file_names 
    "main" )

#-----------------------target for main.app--------------------------
add_executable( main.app
 # ${source_dir}palmprint_dectection.cpp
 # ${source_dir}pp_detect_batch.cpp
 ${source_dir}ROIExtract.cpp
)
#msvcrt
#target_link_libraries( main.app ${OpenCV_LIBS} pdcurses json11)
target_link_libraries( main.app ${OpenCV_LIBS})