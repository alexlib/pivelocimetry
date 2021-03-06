cmake_minimum_required(VERSION 2.8)
project(bppiv)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
	set(warnings "-Wall -Wextra -Werror")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} -std=c++14 -Ofast -m64 -march=native")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(warnings "-Wall -Wextra -Werror")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} -std=c++14 -Ofast -m64 -march=native -fdiagnostics-color=auto -g -fdiagnostics-show-template-tree")
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} -std=c++14 -01 -m64 -march=native -fdiagnostics-color=auto -g -fdiagnostics-show-template-tree -fsanitize=address")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(warnings "/W4 /WX /EHsc")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 ")

endif()

# Define sources and executable name
set(EXECUTABLE_NAME "piv")

include_directories(include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

set(SOURCES
		./src/find_peaks.cpp
		./src/utils.cpp
		./src/Disp.cpp
		./src/DoPiv.cpp
		./src/DoPost.cpp
		./src/Grid.cpp
		./src/IntMap.cpp
		./src/GtkImage.cpp
		./src/PivOptions.cpp
		./src/PivPoint.cpp
		./src/SubPix.cpp
		./src/PivVector.cpp
		./src/XCorr2.cpp
		./src/main.cpp
		./src/PivViewGtk.cpp
		./src/MyAreaD.cpp
		./src/cli_args.cpp
	)

add_executable(${EXECUTABLE_NAME} ${SOURCES} )

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

target_link_libraries(${EXECUTABLE_NAME} ${GTKMM_LIBRARIES} -lpthread)
