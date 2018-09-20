cmake_minimum_required(VERSION 3.12)
project(initi)

set(CMAKE_CXX_STANDARD 17)

set(project_source_dir ${CMAKE_HOME_DIRECTORY})
set(shared_dir ../${CMAKE_HOME_DIRECTORY})

add_executable(initi main.cpp)