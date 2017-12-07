cmake_minimum_required(VERSION 3.6)
project(RayStep)

set(CMAKE_CXX_STANDARD 11)

include(cmake/utils.cmake)
add_subdirectory(src)

# Define RayStep as startup project in VS
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RayStep)

#file(COPY share DESTINATION .)
