cmake_minimum_required(VERSION 3.5)

project(Binary_Vision LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Binary_Vision main.cpp)

# Link OpenCV libraries
target_link_libraries(Binary_Vision ${OpenCV_LIBS})

install(TARGETS Binary_Vision
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
