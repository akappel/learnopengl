cmake_minimum_required(VERSION 3.8.0)
project(learnopengl)

# configure where to drop the exes
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH})

# configure where the path to look for includes
set(THIRD_PARTY_INCLUDE ${CMAKE_BINARY_DIR}/third-party/include)
set(CMAKE_INCLUDE_PATH ${THIRD_PARTY_INCLUDE})
MESSAGE( STATUS "CMAKE_INCLUDE_PATH: " ${CMAKE_INCLUDE_PATH} )

# same for the libs
set(THIRD_PARTY_LIB ${CMAKE_BINARY_DIR}/third-party/lib)
set(CMAKE_LIB_PATH ${THIRD_PARTY_LIB})
MESSAGE( STATUS "CMAKE_LIB_PATH: " ${CMAKE_LIB_PATH} )

# build external libraries

# applications
add_subdirectory(learnopengl.hellotriangle)
