cmake_minimum_required(VERSION 3.21)
project(ImgSegQtDemo)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS
            Core
            Gui
            Widgets
            REQUIRED)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(PADDLE_LIB "/Users/yukaimin/Downloads/Paddle-develop/build/paddle_inference_install_dir")
include_directories("${PADDLE_LIB}/")
link_directories("${PADDLE_LIB}/paddle/lib")
set(DEPS ${PADDLE_LIB}/paddle/lib/libpaddle_inference${CMAKE_SHARED_LIBRARY_SUFFIX})

add_executable(ImgSegQtDemo main.cpp MainWindow.cpp MainWindow.h predictor.cpp predictor.h)
target_link_libraries(ImgSegQtDemo ${DEPS} ${OpenCV_LIBS}
        Qt::Core Qt::Gui Qt::Widgets)
