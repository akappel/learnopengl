cmake_minimum_required(VERSION 3.8.0)
project(learnopengl)

# configure where to drop the exes, includes, libs
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(THIRD_PARTY_PATH ${CMAKE_BINARY_DIR}/third-party)
set(THIRD_PARTY_INCLUDE ${THIRD_PARTY_PATH}/include)
set(THIRD_PARTY_LIB ${THIRD_PARTY_PATH}/lib)

include_directories(${THIRD_PARTY_INCLUDE})
link_directories(${THIRD_PARTY_LIB})

# build external libraries
include(CMakeLists-ExternalProjects.txt)

# applications
add_subdirectory(learnopengl.hellotriangle)
