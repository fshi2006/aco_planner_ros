cmake_minimum_required(VERSION 2.8.3)
project(acoplannerudp)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 ${CMAKE_CXX_FLAGS}")

## find ros packages and combine them with catkin
set(catkin_deps
        roscpp
	nav_msgs
)
find_package(catkin REQUIRED COMPONENTS
        ${catkin_deps}
        )

## ros specific cmake command
catkin_package(
        INCLUDE_DIRS
        CATKIN_DEPENDS ${catkin_deps}
)

include_directories(
        ${catkin_INCLUDE_DIRS}
)

#####################################################################
####   add a ros node executable file
#####################################################################
add_executable(acoplannerudp
        src/acoplannerudp.cpp
        )
target_link_libraries(acoplannerudp
        ${catkin_LIBRARIES}
        ${IPOPT_LIBRARIES}
        ${LINKER_LIBS}
        )
