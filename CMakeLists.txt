cmake_minimum_required( VERSION 3.1 )
project( rasterinha )

set(COMP_FLAGS "-std=c++11")

set(SRCS src/driver.cpp src/canvas.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMP_FLAGS}")

add_executable( ${PROJECT_NAME} ${SRCS})

target_include_directories( ${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include)

target_link_libraries( ${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/libyaml-cpp.a )

