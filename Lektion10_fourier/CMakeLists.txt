cmake_minimum_required(VERSION 3.5)

project(Lektion10_fourier LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenCV REQUIRED)

add_executable(Lektion10_fourier main.cpp)

install(TARGETS Lektion10_fourier
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(Lektion10_fourier PRIVATE ${OpenCV_LIBS})
