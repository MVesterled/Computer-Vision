cmake_minimum_required(VERSION 3.5)

project(Lektion5_Geometric LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenCV REQUIRED)

add_executable(Lektion5_Geometric main.cpp)

install(TARGETS Lektion5_Geometric
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
target_link_libraries(Lektion5_Geometric PRIVATE ${OpenCV_LIBS})
