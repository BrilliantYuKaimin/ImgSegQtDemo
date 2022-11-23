cmake_minimum_required(VERSION 3.21)
project(ImgSegQtDemo)

set(CMAKE_CXX_STANDARD 17)

set(PADDLE_LIB "/Users/yukaimin/Downloads/Paddle-Inference-Demo/c++/lib/paddle_inference")
include_directories("${PADDLE_LIB}/")
link_directories("${PADDLE_LIB}/paddle/lib")
set(DEPS ${PADDLE_LIB}/paddle/lib/libpaddle_inference${CMAKE_SHARED_LIBRARY_SUFFIX})

add_executable(ImgSegQtDemo main.cpp)
target_link_libraries(ImgSegQtDemo ${DEPS})
