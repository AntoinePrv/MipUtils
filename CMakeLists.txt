cmake_minimum_required(VERSION 3.7)

project(MipUtils)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -DNDEBUG -DIL_STD")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_extensions/)

find_package(Cplex)
include_directories(${CPLEX_INCLUDE_DIRS})

find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

file(
	GLOB_RECURSE
	source_files
	src/lib/*
)

add_executable(
	mip-coefs
	${source_files}
	src/scripts/mip-coefs.cpp
)

add_executable(
	mip-convert
	src/scripts/mip-convert.cpp
)

target_link_libraries(mip-coefs ${CPLEX_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(mip-convert ${CPLEX_LIBRARIES})
