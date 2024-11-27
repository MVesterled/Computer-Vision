cmake_minimum_required(VERSION 3.5)

project(Lek9_objectReg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenCV REQUIRED)

add_executable(Lek9_objectReg main.cpp)

install(TARGETS Lek9_objectReg
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(Lek9_objectReg PRIVATE ${OpenCV_LIBS})
