add_executable(
	piced
	ResourceManager.h
	Logger.h
	Logger.cpp
	utils/strutils.h
	utils/strutils.cpp
	utils/fileutils.h
	utils/fileutils.cpp
	Future.h
	main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(piced LINK_PUBLIC upng)
target_link_libraries(piced LINK_PUBLIC json)
