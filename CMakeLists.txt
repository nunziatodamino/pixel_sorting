cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project( DisplayImage )

include_directories(include)


find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

set(SOURCES
    src/main.cpp
    src/sortingAlgos.cpp
    src/cliConfig.cpp
)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( pixelSort ${SOURCES} )
target_link_libraries( pixelSort ${OpenCV_LIBS} OpenMP::OpenMP_CXX)