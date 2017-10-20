cmake_minimum_required(VERSION 3.7)

project(MipUtils)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -DNDEBUG -DIL_STD")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_extensions/)

find_package(Cplex)
include_directories(${CPLEX_INCLUDE_DIRS})


file(
	GLOB_RECURSE
	source_files
	src/*
)

add_executable(
	mip-utils
	${source_files}
)

target_link_libraries(mip-utils ${CPLEX_LIBRARIES})
