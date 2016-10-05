cmake_minimum_required(VERSION 3.5)
project(TestServerBoost)

set(BOOST_ROOT "/home/igor/boost_1_60_0")
find_package(Boost REQUIRED system thread timer chrono signals)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES main.cpp Client.cpp Client.h Server.cpp Server.h Message.cpp Message.h TextBuffer.cpp TextBuffer.h InputLine.cpp InputLine.h)
add_executable(TestServerBoost ${SOURCE_FILES})
target_link_libraries(TestServerBoost ${Boost_LIBRARIES})
