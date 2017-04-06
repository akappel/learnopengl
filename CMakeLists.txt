cmake_minimum_required(VERSION 3.8.0)
project(learnopengl)

# configure different path targets
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH})

# build external libraries

# applications
add_subdirectory(learnopengl.hellotriangle)
