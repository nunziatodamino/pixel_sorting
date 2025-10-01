cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(pixSort)

include_directories(include)

find_package(OpenCV REQUIRED)

set(SOURCES
    src/main.cpp
    src/sortingAlgos.cpp
    src/cliConfig.cpp
)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( pixSort ${SOURCES} )
target_link_libraries( pixSort ${OpenCV_LIBS} )