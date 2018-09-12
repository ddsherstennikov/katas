cmake_minimum_required(VERSION 3.12)
project(db_cache)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_HOME_DIRECTORY}/cmake)
set(CMAKE_CXX_STANDARD 17)

add_executable(db_cache main.cpp db_adapter/db_adapter.cpp db_adapter/db_adapter.h)

find_package(sqlite3)

if (SQLITE3_FOUND)
	include_directories(${SQLITE3_INCLUDE_DIRS})
	target_link_libraries (db_cache ${SQLITE3_LIBRARIES})
endif (SQLITE3_FOUND)