add_library(
	upng
	STATIC
	upng.h
	upng.c
)

target_include_directories(
	upng
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
