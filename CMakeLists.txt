cmake_minimum_required(VERSION 3.10)
project(TCPServerClient)

set(CMAKE_CXX_STANDARD 17)

find_package(MySQL REQUIRED COMPONENTS cppconn)

add_executable(client client.cpp)
add_executable(server server.cpp)

target_link_libraries(client PRIVATE mysqlcppconn)
target_link_libraries(server PRIVATE mysqlcppconn)