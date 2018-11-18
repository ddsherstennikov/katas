cmake_minimum_required(VERSION 3.12)
project(rooks)

set(CMAKE_CXX_STANDARD 17)

add_executable(rooks main.cpp)

# pthread
find_package (Threads)
target_link_libraries (rooks ${CMAKE_THREAD_LIBS_INIT})