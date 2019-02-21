cmake_minimum_required( VERSION 3.1 )
project( cg-draw )

set(SRCS src/driver.cpp)

add_executable( ${PROJECT_NAME} ${SRCS})

target_include_directories( ${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include)

target_link_libraries( ${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/libyaml-cpp.a )

