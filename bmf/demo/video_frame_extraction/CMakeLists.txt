cmake_minimum_required(VERSION 3.12)
project(jpeg_encoder)
# language setting
# CXX
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

# You may need to add this option for BabitMF version 0.0.9 or lower to solve an ABI missing symbol issue in the output `libjpeg_encoder.so` dynamic library
# add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

if (NOT DEFINED CUDA_ROOT)
    set(CUDA_ROOT /usr/local/cuda)
endif()
set(CUDA_INCLUDE_DIR ${CUDA_ROOT}/include)
set(CUDA_LIBRARY_DIR ${CUDA_ROOT}/lib64)
message(STATUS "detect cuda root: " ${CUDA_INCLUDE_DIR})

SET(MODULE_SRCS ${PROJECT_SOURCE_DIR}/jpeg_encoder.cpp)

add_library(${PROJECT_NAME} SHARED ${MODULE_SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/../../../output/bmf/include
                           ${CUDA_INCLUDE_DIR}
                           /usr/local/include)
target_link_libraries(${PROJECT_NAME} PRIVATE nvjpeg cudart
                      bmf_module_sdk)
target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/../../../output/bmf/lib
                        ${CUDA_LIBRARY_DIR})


