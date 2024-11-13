cmake_minimum_required(VERSION 3.5)

project(Lek8_Feature LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenCV REQUIRED)

add_executable(Lek8_Feature main.cpp)

install(TARGETS Lek8_Feature
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(Lek8_Feature PRIVATE ${OpenCV_LIBS})
