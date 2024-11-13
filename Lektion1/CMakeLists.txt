cmake_minimum_required(VERSION 3.5)

project(Lektion1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Lektion1 main.cpp)

# Link OpenCV libraries
target_link_libraries(Lektion1 ${OpenCV_LIBS})

install(TARGETS Lektion1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


