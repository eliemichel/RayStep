# Automatically generate moc files for Qt
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
if(Qt5Widgets_FOUND)
	message("Using Qt ${Qt5Widgets_VERSION} from ${Qt5Widgets_INCLUDE_DIRS}")
endif()

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

target_link_libraries(piced upng)
target_link_libraries(piced json)

target_link_libraries_and_dll(piced Qt5::Core)
target_link_libraries_and_dll(piced Qt5::Gui)
target_link_libraries_and_dll(piced Qt5::Widgets)
