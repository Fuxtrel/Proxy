cmake_minimum_required(VERSION 3.19)
project(proxy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "/home/alex/qt5/5.15.2/gcc_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(QT_VERSION 5)
set(REQUIRED_LIBS Core WebSockets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::WebSockets)

add_executable(${PROJECT_NAME} main.cpp proxy.cpp proxy.h)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
