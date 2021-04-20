project(client_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "/home/alex/Qt/5.15.2/gcc_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core WebSockets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::WebSockets)

add_executable(${PROJECT_NAME} main.cpp client.cpp client.h)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})