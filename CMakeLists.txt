cmake_minimum_required(VERSION 3.10)
project(FileTransfer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "C:/Program Files/boost/boost_1_87_0")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${BOOST_LIBRARYDIR})

add_executable(server server/main.cpp)
add_executable(client client/main.cpp)

target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(client ${Boost_LIBRARIES})
