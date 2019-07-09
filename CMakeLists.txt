cmake_minimum_required(VERSION 2.8)

project(MarketPlayerTestTask)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

set(SRC_LIST 
src/App.h
src/CoordinateConverter.h
src/DoubleToByteTruncater.h
src/GaussianKernelCalculator.h
src/Image.h
src/ImageReader.h
src/ImageWriter.h
src/Kernel.h
src/MultithreadGaussianBlur.h
src/MultithreadRGBToGrayscaleImageConverter.h
src/App.cpp 
src/GaussianKernelCalculator.cpp
src/Image.cpp
src/ImageReader.cpp
src/ImageWriter.cpp
src/main.cpp
src/MultithreadGaussianBlur.cpp
src/MultithreadRGBToGrayscaleImageConverter.cpp
src/stb/stb_image.h
src/stb/stb_image_write.h)

add_executable(${PROJECT_NAME} ${SRC_LIST})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

