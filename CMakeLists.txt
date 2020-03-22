cmake_minimum_required(VERSION 3.14)
project(client)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost REQUIRED COMPONENTS system filesystem thread)

add_executable(client main.cpp)
target_link_libraries(client ${Boost_LIBRARIES})
set (CMAKE_CXX_FLAGS "-g -Wall -DBOOST_LOG_DYN_LINK")