project(boost_server)

set(CMAKE_CXX_STANDARD 20)
set(BOOST_ROOT  "/home/alex/Загрузки/boost_1_75_0/stage")
set (Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package (Boost 1.75.0 COMPONENTS system iostreams filesystem)

include_directories (${Boost_INCLUDE_DIRS})
include_directories (${OpenSSL_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp server.cpp main.cpp server.h)

link_directories(boost_server ${Boost_LIBRARY_DIRS})
target_link_libraries (boost_server ${Boost_LIBRARIES})

