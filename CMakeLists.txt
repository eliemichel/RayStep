cmake_minimum_required(VERSION 3.6)
project(piced)

add_subdirectory(src)

# Define piced as startup project in VS
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT piced)

#file(COPY share DESTINATION .)
