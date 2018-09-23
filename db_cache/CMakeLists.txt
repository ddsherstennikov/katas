cmake_minimum_required(VERSION 3.12)
project(db_cache)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_HOME_DIRECTORY}/cmake)
set(CMAKE_CXX_STANDARD 17)

add_executable(db_cache main.cpp db_adapter/db_adapter.cpp db_adapter/db_adapter.h db_cache/db_cache.cpp db_cache/db_cache.h)

include_directories(${CMAKE_SOURCE_DIR})

find_package(sqlite3)
find_package(Boost 1.65.0 COMPONENTS filesystem)

if (SQLITE3_FOUND)
	include_directories(${SQLITE3_INCLUDE_DIRS})
	target_link_libraries (db_cache ${SQLITE3_LIBRARIES} ${Boost_LIBRARIES})
endif (SQLITE3_FOUND)