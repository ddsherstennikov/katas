cmake_minimum_required(VERSION 3.12)
project(db_cache)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_HOME_DIRECTORY}/cmake)
set(CMAKE_CXX_STANDARD 17)

add_executable(db_cache main.cpp db_adapter/db_adapter.cpp db_adapter/db_adapter.h db_cache/db_cache.cpp db_cache/db_cache.h quote_genr/quote_genr.cpp quote_genr/quote_genr.h)

include_directories(${CMAKE_SOURCE_DIR})


set(BOOST_LIBRARYDIR "/usr/lib/x86_64-linux-gnu/")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.65.0 REQUIRED COMPONENTS system thread filesystem)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(db_cache Boost::system Boost::thread)
endif()



find_package(sqlite3)

if (SQLITE3_FOUND)
	include_directories(${SQLITE3_INCLUDE_DIRS})
	target_link_libraries (db_cache ${SQLITE3_LIBRARIES})
endif (SQLITE3_FOUND)